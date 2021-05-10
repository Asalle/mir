/*
 * Copyright © 2021 Canonical Ltd.
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
 * Authored by: Christopher James Halse Rogers <christopher.halse.rogers@canonical.com>
 */

#ifndef MIR_PLATFORM_TEST_HARNESS_H
#define MIR_PLATFORM_TEST_HARNESS_H

#include "mir/shared_library.h"
#include <memory>

namespace mir
{
namespace test
{
class PlatformTestHarness
{
public:
    PlatformTestHarness() = default;
    virtual ~PlatformTestHarness() = default;

    virtual void setup() = 0;
    virtual void teardown() = 0;
    virtual auto platform_module() -> std::shared_ptr<mir::SharedLibrary> = 0;
};
}
}

#endif //MIR_PLATFORM_TEST_HARNESS_H
