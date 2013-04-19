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
 * Authored by: Robert Carr <robert.carr@canonical.com>
 */

#ifndef MIR_INPUT_NULL_INPUT_MANAGER_H_
#define MIR_INPUT_NULL_INPUT_MANAGER_H_

#include "mir/input/input_manager.h"

namespace mir
{
namespace input
{

class NullInputManager : public InputManager
{
public:
    NullInputManager() {};
    virtual ~NullInputManager() {}

    void start() {}
    void stop() {}

    std::shared_ptr<InputChannel> make_input_channel()
    {
        return std::shared_ptr<InputChannel>();
    }

    virtual void set_input_focus_to(std::shared_ptr<input::SessionTarget> const& /* session */,
                                    std::shared_ptr<input::SurfaceTarget> const& /* surface */)
    {
    }

protected:
    NullInputManager(const NullInputManager&) = delete;
    NullInputManager& operator=(const NullInputManager&) = delete;
};

}
}

#endif // MIR_INPUT_NULL_INPUT_MANAGER
