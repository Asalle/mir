/*
 * Copyright © 2013 Canonical Ltd.
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License version 3,
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * Authored by: Alexandros Frantzis <alexandros.frantzis@canonical.com>
 */

#ifndef MIR_GRAPHICS_DISPLAY_BUFFER_H_
#define MIR_GRAPHICS_DISPLAY_BUFFER_H_

#include <mir/geometry/rectangle.h>

#include <list>
#include <memory>

namespace mir
{
namespace graphics
{

class Buffer;
class Renderable;

/**
 * Interface to an output framebuffer.
 */
class DisplayBuffer
{
public:
    virtual ~DisplayBuffer() {}

    /** The area the DisplayBuffer occupies in the virtual screen space. */
    virtual geometry::Rectangle view_area() const = 0;
    /** Makes the DisplayBuffer the current GL rendering target. */
    virtual void make_current() = 0;
    /** Releases the current GL rendering target. */
    virtual void release_current() = 0;
    /** Posts the DisplayBuffer to the screen. */
    virtual void post_update() = 0;

    virtual bool can_bypass() const = 0;
    virtual void post_update(std::shared_ptr<Buffer> /* bypass_buf */) {}

    /* give the display buffer a chance to filter_out_optimized_renderables the draw.
       The display buffer may remove items from the list. If items are removed,
       then the display buffer will guarantee that the removed items will appear on the
       screen after calling post_update().
       Items not removed must still be drawn to the GL rendering target */
    virtual void filter_out_optimized_renderables(std::list<std::shared_ptr<Renderable>>& list) = 0; 

protected:
    DisplayBuffer() = default;
    DisplayBuffer(DisplayBuffer const& c) = delete;
    DisplayBuffer& operator=(DisplayBuffer const& c) = delete;
};

}
}

#endif /* MIR_GRAPHICS_DISPLAY_BUFFER_H_ */
