/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * Authored by: Alan Griffiths <alan@octopull.co.uk>
 */

#ifndef MIR_GEOMETRY_SIZE_H_
#define MIR_GEOMETRY_SIZE_H_

#include <cstdint>

namespace mir
{
namespace geometry
{

namespace detail
{
enum DimensionTag { width, height, x, y, dx, dy };

template<DimensionTag Tag>
class IntWrapper
{
public:
	IntWrapper(uint32_t value) : value(value) {}

	uint32_t as_uint32_t() const { return value; }

private:
	uint32_t value;
};

template<DimensionTag Tag>
inline bool operator == (IntWrapper<Tag> const& lhs, IntWrapper<Tag> const& rhs)
{ return lhs.as_uint32_t() == rhs.as_uint32_t(); }

template<DimensionTag Tag>
inline bool operator != (IntWrapper<Tag> const& lhs, IntWrapper<Tag> const& rhs)
{ return lhs.as_uint32_t() != rhs.as_uint32_t(); }

template<DimensionTag Tag>
inline bool operator <= (IntWrapper<Tag> const& lhs, IntWrapper<Tag> const& rhs)
{ return lhs.as_uint32_t() <= rhs.as_uint32_t(); }

template<DimensionTag Tag>
inline bool operator >= (IntWrapper<Tag> const& lhs, IntWrapper<Tag> const& rhs)
{ return lhs.as_uint32_t() >= rhs.as_uint32_t(); }

template<DimensionTag Tag>
inline bool operator < (IntWrapper<Tag> const& lhs, IntWrapper<Tag> const& rhs)
{ return lhs.as_uint32_t() < rhs.as_uint32_t(); }

template<DimensionTag Tag>
inline bool operator > (IntWrapper<Tag> const& lhs, IntWrapper<Tag> const& rhs)
{ return lhs.as_uint32_t() > rhs.as_uint32_t(); }
} // namespace detail

typedef detail::IntWrapper<detail::width> Width;
typedef detail::IntWrapper<detail::height> Height;

class Size
{
};

}
}

#endif /* MIR_GEOMETRY_SIZE_H_ */
