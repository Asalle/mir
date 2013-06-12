/*
 * Copyright © 2012 Canonical Ltd.
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

#include "mir/compositor/buffer_stream_surfaces.h"

#include "mir_test_doubles/stub_buffer.h"
#include "mir_test_doubles/mock_buffer_bundle.h"
#include "mir_test/gmock_fixes.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace mc = mir::compositor;
namespace geom = mir::geometry;
namespace mtd = mir::test::doubles;
namespace ms = mir::surfaces;

class BufferStreamTest : public ::testing::Test
{
protected:
    virtual void SetUp()
    {
        mock_buffer = std::make_shared<mtd::StubBuffer>();
        mock_director = std::make_shared<mtd::MockBufferBundle>();
    }

    std::shared_ptr<mtd::StubBuffer> mock_buffer;
    std::shared_ptr<mtd::MockBufferBundle> mock_director;
};

TEST_F(BufferStreamTest, size_query)
{
    geom::Size size{geom::Width{4},geom::Height{5}};
    mc::BufferProperties properties {size, geom::PixelFormat::abgr_8888, mc::BufferUsage::hardware};
    EXPECT_CALL(*mock_director, properties())
        .Times(1)
        .WillOnce(testing::Return(properties));

    mc::BufferStreamSurfaces buffer_stream(mock_director);
    auto returned_size = buffer_stream.stream_size();
    EXPECT_EQ(size, returned_size);
}

TEST_F(BufferStreamTest, pixel_format_query)
{
    geom::PixelFormat format{geom::PixelFormat::abgr_8888};
    mc::BufferProperties properties {geom::Size{geom::Width{4},geom::Height{5}},
                                     format, mc::BufferUsage::hardware};
    EXPECT_CALL(*mock_director, properties())
        .Times(1)
        .WillOnce(testing::Return(properties));

    mc::BufferStreamSurfaces buffer_stream(mock_director);
    auto returned_pf = buffer_stream.get_stream_pixel_format();
    EXPECT_EQ(format, returned_pf);
}

TEST_F(BufferStreamTest, force_client_completion_command)
{
    EXPECT_CALL(*mock_director, force_client_completion())
        .Times(1);

    mc::BufferStreamSurfaces buffer_stream(mock_director);
    buffer_stream.force_client_completion();
}

TEST_F(BufferStreamTest, get_buffer_for_compositor_handles_resources)
{
    using namespace testing;

    EXPECT_CALL(*mock_director, compositor_acquire())
        .Times(1)
        .WillOnce(Return(mock_buffer));
    EXPECT_CALL(*mock_director, compositor_release(_))
        .Times(1);

    mc::BufferStreamSurfaces buffer_stream(mock_director);

    buffer_stream.lock_back_buffer();
}

TEST_F(BufferStreamTest, get_buffer_for_compositor_can_lock)
{
    using namespace testing;

    EXPECT_CALL(*mock_director, compositor_acquire())
        .Times(1)
        .WillOnce(Return(mock_buffer));
    EXPECT_CALL(*mock_director, compositor_release(_))
        .Times(1);

    mc::BufferStreamSurfaces buffer_stream(mock_director);

    buffer_stream.lock_back_buffer();
}

TEST_F(BufferStreamTest, get_buffer_for_client_releases_resources)
{
    using namespace testing;

    EXPECT_CALL(*mock_director, client_acquire())
        .Times(1)
        .WillOnce(Return(mock_buffer));
    EXPECT_CALL(*mock_director, client_release(_))
        .Times(1);
    mc::BufferStreamSurfaces buffer_stream(mock_director);

    buffer_stream.secure_client_buffer();
}
