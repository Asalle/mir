/*
 * Copyright © 2020 Canonical Ltd.
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
 */

#ifndef MIR_FRONTEND_XWAYLAND_SURFACE_ROLE_SURFACE_H
#define MIR_FRONTEND_XWAYLAND_SURFACE_ROLE_SURFACE_H

#include <memory>
#include <optional>

namespace mir
{
namespace scene
{
class Surface;
}
namespace frontend
{
class WlSurface;

/// The interface used by XWaylandSurfaceRole to talk to the surface object
class XWaylandSurfaceRoleSurface
{
public:
    XWaylandSurfaceRoleSurface() = default;
    virtual ~XWaylandSurfaceRoleSurface() = default;

    virtual void wl_surface_destroyed() = 0;
    virtual auto scene_surface() const -> std::optional<std::shared_ptr<scene::Surface>> = 0;

private:
    XWaylandSurfaceRoleSurface(XWaylandSurfaceRoleSurface const&) = delete;
    XWaylandSurfaceRoleSurface& operator=(XWaylandSurfaceRoleSurface const&) = delete;
};
}
}

#endif // MIR_FRONTEND_XWAYLAND_SURFACE_ROLE_SURFACE_H
