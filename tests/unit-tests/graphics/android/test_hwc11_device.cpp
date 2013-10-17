/*
 * Copyright © 2013 Canonical Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Authored by: Kevin DuBois <kevin.dubois@canonical.com>
 */

#include "src/server/graphics/android/hwc11_device.h"
#include "src/server/graphics/android/hwc_layerlist.h"
#include "mir_test_doubles/mock_display_commander.h"
#include "mir_test_doubles/mock_hwc_composer_device_1.h"
#include "mir_test_doubles/mock_hwc_layerlist.h"
#include "mir_test_doubles/mock_buffer.h"
#include "mir_test_doubles/mock_hwc_vsync_coordinator.h"
#include "mir_test_doubles/mock_egl.h"
#include <gtest/gtest.h>
#include <stdexcept>

namespace mg=mir::graphics;
namespace mga=mir::graphics::android;
namespace mtd=mir::test::doubles;

class HWC11Device : public ::testing::Test
{
protected:
    virtual void SetUp()
    {
        mock_device = std::make_shared<testing::NiceMock<mtd::MockHWCComposerDevice1>>();
        mock_display_support_provider = std::make_shared<testing::NiceMock<mtd::MockDisplayCommander>>();
        mock_hwc_layers = std::make_shared<testing::NiceMock<mtd::MockHWCLayerList>>();
        mock_vsync = std::make_shared<testing::NiceMock<mtd::MockVsyncCoordinator>>();
        mock_egl.silence_uninteresting();

        empty_list.numHwLayers = 0;
        empty_list.retireFenceFd = -1;
        ON_CALL(*mock_hwc_layers, native_list())
            .WillByDefault(testing::Return(&empty_list));
    }

    std::shared_ptr<mtd::MockVsyncCoordinator> mock_vsync;
    std::shared_ptr<mtd::MockHWCLayerList> mock_hwc_layers;
    std::shared_ptr<mtd::MockHWCComposerDevice1> mock_device;
    std::shared_ptr<mtd::MockDisplayCommander> mock_display_support_provider;
    EGLDisplay dpy;
    EGLSurface surf;
    mtd::MockEGL mock_egl;
    hwc_display_contents_1_t empty_list;
};

namespace
{
struct HWCDummyLayer : public mga::HWCDefaultLayer
{
    HWCDummyLayer()
     : HWCDefaultLayer({})
    {
    }
};
}

TEST_F(HWC11Device, test_hwc_gles_set_empty_layerlist)
{
    using namespace testing;

    mga::HWC11Device device(mock_device, mock_hwc_layers, mock_display_support_provider, mock_vsync);

    EXPECT_CALL(*mock_device, set_interface(mock_device.get(), 1, _))
        .Times(1);
    device.commit_frame(dpy, surf);

    EXPECT_EQ(empty_list.numHwLayers, mock_device->display0_set_content.numHwLayers);
    EXPECT_EQ(-1, mock_device->display0_set_content.retireFenceFd);
}

TEST_F(HWC11Device, test_hwc_gles_set_error)
{
    using namespace testing;

    mga::HWC11Device device(mock_device, mock_hwc_layers, mock_display_support_provider, mock_vsync);

    EXPECT_CALL(*mock_device, set_interface(mock_device.get(), 1, _))
        .Times(1)
        .WillOnce(Return(-1));

    EXPECT_THROW({
        device.commit_frame(dpy, surf);
    }, std::runtime_error);
}

TEST_F(HWC11Device, test_hwc_gles_commit_swapbuffers_failure)
{
    using namespace testing;
    EXPECT_CALL(mock_egl, eglSwapBuffers(dpy,surf))
        .Times(1)
        .WillOnce(Return(EGL_FALSE));

    mga::HWC11Device device(mock_device, mock_hwc_layers, mock_display_support_provider, mock_vsync);

    EXPECT_THROW({
        device.commit_frame(dpy, surf);
    }, std::runtime_error);
}

TEST_F(HWC11Device, test_hwc_commit_order_with_vsync)
{
    using namespace testing;

    mga::HWC11Device device(mock_device, mock_hwc_layers, mock_display_support_provider, mock_vsync);

    //the order here is very important. eglSwapBuffers will alter the layerlist,
    //so it must come before assembling the data for set
    InSequence seq;
    EXPECT_CALL(*mock_hwc_layers, native_list())
        .Times(1);
    EXPECT_CALL(*mock_device, prepare_interface(mock_device.get(), 1, _))
        .Times(1);
    EXPECT_CALL(mock_egl, eglSwapBuffers(dpy,surf))
        .Times(1);
    EXPECT_CALL(*mock_device, set_interface(mock_device.get(), 1, _))
        .Times(1);

    device.commit_frame(dpy, surf);

    EXPECT_EQ(empty_list.numHwLayers, mock_device->display0_prepare_content.numHwLayers);
    EXPECT_EQ(-1, mock_device->display0_prepare_content.retireFenceFd);
    EXPECT_EQ(empty_list.numHwLayers, mock_device->display0_set_content.numHwLayers);
    EXPECT_EQ(-1, mock_device->display0_set_content.retireFenceFd);
}

TEST_F(HWC11Device, hwc_device_set_next_frontbuffer_adds_to_layerlist)
{
    std::shared_ptr<mg::Buffer> mock_buffer = std::make_shared<mtd::MockBuffer>();
    EXPECT_CALL(*this->mock_hwc_layers, set_fb_target(mock_buffer))
        .Times(1);
 
    mga::HWC11Device device(mock_device, mock_hwc_layers, mock_display_support_provider, mock_vsync);
    device.set_next_frontbuffer(mock_buffer);
}
