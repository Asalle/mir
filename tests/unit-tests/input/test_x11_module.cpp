/*
 * Copyright © Canonical Ltd.
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
 */

#include "mir/shared_library.h"
#include "mir/input/platform.h"

#include "mir/test/doubles/mock_x11.h"
#include "mir/test/doubles/mock_option.h"
#include "mir_test_framework/executable_path.h"
#include "mir/test/doubles/stub_console_services.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace mtd = mir::test::doubles;
namespace mtf = mir_test_framework;
namespace mo = mir::options;
using namespace ::testing;
namespace
{
auto get_x11_platform()
{
    auto path = mtf::server_platform("server-x11");
    return std::make_shared<mir::SharedLibrary>(path);
}

char const probe_input_platform_symbol[] = "probe_input_platform";

struct X11Platform : Test
{

    NiceMock<mtd::MockOption> options;
    NiceMock<mtd::MockX11> mock_x11;
    std::shared_ptr<mir::SharedLibrary> library{get_x11_platform()};
    mtd::StubConsoleServices console;
};

}

TEST_F(X11Platform, probes_as_unsupported_without_display)
{
    ON_CALL(mock_x11, XOpenDisplay(_))
        .WillByDefault(Return(nullptr));

    auto probe_fun = library->load_function<mir::input::ProbePlatform>(probe_input_platform_symbol);
    EXPECT_THAT(probe_fun(options, console), Eq(mir::input::PlatformPriority::unsupported));
}
