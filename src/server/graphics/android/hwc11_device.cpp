/*
 * Copyright © 2013 Canonical Ltd.
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 3,
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Authored by:
 *   Kevin DuBois <kevin.dubois@canonical.com>
 */

#include "hwc11_device.h"
#include "hwc_layerlist.h"
#include "hwc_vsync_coordinator.h"
#include "mir/graphics/android/sync_fence.h"
#include <boost/throw_exception.hpp>
#include <stdexcept>

namespace mg = mir::graphics;
namespace mga=mir::graphics::android;

mga::HWC11Device::HWC11Device(std::shared_ptr<hwc_composer_device_1> const& hwc_device,
                              std::shared_ptr<HWCLayerList> const& layer_list,
                              std::shared_ptr<DisplayCommander> const& fbdev,
                              std::shared_ptr<HWCVsyncCoordinator> const& coordinator)
    : HWCCommonCommand(hwc_device, coordinator),
      layer_list(layer_list),
      fb_device(fbdev), 
      sync_ops(std::make_shared<mga::RealSyncFileOps>())
{
}

mga::HWC11Device::~HWC11Device() noexcept
{
}

void mga::HWC11Device::set_next_frontbuffer(std::shared_ptr<mg::Buffer> const& buffer)
{
    layer_list->set_fb_target(buffer);
}

void mga::HWC11Device::commit_frame(EGLDisplay dpy, EGLSurface sur)
{
    auto lg = lock_unblanked();

    //note, although we only have a primary display right now,
    //      set the second display to nullptr, as exynos hwc always derefs displays[1]
    hwc_display_contents_1_t* displays[HWC_NUM_DISPLAY_TYPES] {layer_list->native_list(), nullptr};

    if (hwc_device->prepare(hwc_device.get(), 1, displays))
    {
        BOOST_THROW_EXCEPTION(std::runtime_error("error during hwc prepare()"));
    }

    /* note, swapbuffers will go around through the driver and call
       set_next_frontbuffer, updating the fb target before committing */
    if (eglSwapBuffers(dpy, sur) == EGL_FALSE)
    {
        BOOST_THROW_EXCEPTION(std::runtime_error("error during eglSwapBuffers"));
    }

    if (hwc_device->set(hwc_device.get(), 1, displays))
    {
        BOOST_THROW_EXCEPTION(std::runtime_error("error during hwc set()"));
    }

    mga::SyncFence fence(sync_ops, displays[HWC_DISPLAY_PRIMARY]->retireFenceFd);
    fence.wait();
}

void mga::HWC11Device::sync_to_display(bool)
{
    //TODO return error code, running not synced to vsync is not supported
}
