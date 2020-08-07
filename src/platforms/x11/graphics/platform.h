/*
 * Copyright © 2015 Canonical Ltd.
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License version 2 or 3,
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Authored by: Cemil Azizoglu <cemil.azizoglu@canonical.com>
 */

#ifndef MIR_GRAPHICS_X_PLATFORM_H_
#define MIR_GRAPHICS_X_PLATFORM_H_

#include "mir/graphics/display_report.h"
#include "mir/graphics/platform.h"
#include "mir/geometry/size.h"
#include "mir/renderer/gl/egl_platform.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>

namespace mir
{
namespace graphics
{
namespace X
{
struct X11OutputConfig
{
    inline X11OutputConfig(geometry::Size const& size)
        : size{size},
          scale{1.0f}
    {
    }

    inline X11OutputConfig(geometry::Size const& size, float scale)
        : size{size},
          scale{scale}
    {
    }

    ~X11OutputConfig() = default;

    geometry::Size size;
    float scale;
};

class Platform : public graphics::Platform,
                 public mir::renderer::gl::EGLPlatform
{
public:
    // Parses colon separated list of sizes in the form WIDTHxHEIGHT^SCALE (^SCALE is optional)
    static auto parse_output_sizes(std::string output_sizes) -> std::vector<X11OutputConfig>;

    explicit Platform(std::shared_ptr<::Display> const& conn,
                      std::vector<X11OutputConfig> output_sizes,
                      std::shared_ptr<DisplayReport> const& report);
    ~Platform() = default;

    /* From Platform */
    UniqueModulePtr<GraphicBufferAllocator>
        create_buffer_allocator(graphics::Display const& output) override;

    UniqueModulePtr<graphics::Display> create_display(
        std::shared_ptr<DisplayConfigurationPolicy> const& initial_conf_policy,
        std::shared_ptr<GLConfig> const& gl_config) override;

    EGLNativeDisplayType egl_native_display() const override;
private:
    std::shared_ptr<::Display> const x11_connection;
    std::shared_ptr<DisplayReport> const report;
    std::vector<X11OutputConfig> const output_sizes;
};

}
}
}
#endif /* MIR_GRAPHICS_X_PLATFORM_H_ */
