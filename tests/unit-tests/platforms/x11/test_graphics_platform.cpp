/*
 * Copyright © 2015 Canonical Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 or 3 as
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
 * Authored by: Thomas Guest <thomas.guest@canonical.com>
 *              Kevin DuBois <kevin.dubois@canonical.com>
 */

#include "mir/graphics/platform.h"
#include "mir/graphics/graphic_buffer_allocator.h"
#include "mir/graphics/buffer_properties.h"
#include "mir/test/doubles/mock_egl.h"
#include "mir/test/doubles/mock_gl.h"
#include "mir/test/doubles/null_emergency_cleanup.h"
#include "src/server/report/null/display_report.h"
#include "mir/test/doubles/null_console_services.h"
#include "mir/options/program_option.h"
#include "src/platforms/x11/graphics/platform.h"
#include "src/platforms/x11/x11_resources.h"
#include "mir/test/doubles/mock_x11.h"

#include "mir/logging/dumb_console_logger.h"

#include <gtest/gtest.h>

namespace mg = mir::graphics;
namespace ml = mir::logging;
namespace geom = mir::geometry;
namespace mtd = mir::test::doubles;
namespace mo = mir::options;

class GraphicsPlatform : public ::testing::Test
{
public:
    GraphicsPlatform() : logger(std::make_shared<ml::DumbConsoleLogger>())
    {
        using namespace testing;

        ON_CALL(mock_x11, XNextEvent(_, _))
            .WillByDefault(
                DoAll(
                    Invoke(
                        [](auto, XEvent* ev)
                        {
                            ev->type = Expose;
                        }),
                    Return(true)));
        ON_CALL(mock_egl, eglQueryString(_, EGL_EXTENSIONS))
            .WillByDefault(Return(""));
    }

    std::shared_ptr<mg::Platform> create_platform()
    {
        return std::make_shared<mg::X::Platform>(
            std::make_shared<mir::X::X11Connection>(nullptr, nullptr),
            std::vector<mg::X::X11OutputConfig>{{{1280, 1024}}},
            std::make_shared<mir::report::null::DisplayReport>());
    }

    std::shared_ptr<ml::Logger> logger;

    ::testing::NiceMock<mtd::MockEGL> mock_egl;
    ::testing::NiceMock<mtd::MockGL> mock_gl;
    ::testing::NiceMock<mtd::MockX11> mock_x11;
};

#include "../test_graphics_platform.h"
