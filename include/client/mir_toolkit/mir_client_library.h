/*
 * Copyright © 2012-2013 Canonical Ltd.
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License version 3,
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
 */

#ifndef MIR_CLIENT_LIBRARY_H
#define MIR_CLIENT_LIBRARY_H

#include "mir_toolkit/input/event.h"

#ifdef __cplusplus
/** The C client API
 */
namespace mir_toolkit {
extern "C" {
#endif

/* This header defines the Mir client library's C API. */

/* Display server connection API */
typedef void* MirEGLNativeWindowType;
typedef void* MirEGLNativeDisplayType;
typedef struct MirConnection MirConnection;
typedef struct MirSurface MirSurface;

/**
 * Returned by asynchronous functions. Must not be freed by callers. See the
 * individual function documentation for information on the lifetime of wait
 * handles.
 */
typedef struct MirWaitHandle MirWaitHandle;

/**
 * Callback to be passed when issuing a mir_connect request.
 *   \param [in] connection   The new connection
 *   \param [in,out] context  Context provided by client in calling mir_connect
 */
typedef void (*mir_connected_callback)(MirConnection *connection, void *context);

/**
 * Callback to be passed when calling mir_surface_create,
 * mir_surface_next_buffer or mir_surface_release.
 *   \param [in] surface      The surface being updated
 *   \param [in,out] context  Context provided by client
 */
typedef void (*mir_surface_lifecycle_callback)(MirSurface *surface, void *context);

/**
 * Callback member of MirEventDelegate for handling of input events.
 *   \param [in] surface The surface on which an event has occurred
 *   \param [in] event   The event to be handled
 *   \param [in,out] context Context provided by client in MirEventDelegate
 */
typedef void (*mir_event_delegate_handle_input_callback)(MirSurface* surface, MirEvent* ev, void* context);

/**
 * The order of components in a format enum matches the order of the
 * components as they would be written in an integer representing a pixel
 * value of that format.
 *
 * For example, abgr_8888 corresponds to 0xAABBGGRR, which will end up as
 * R,G,B,A in memory on a little-endian system, and as A,B,G,R on a
 * big-endian system.
 */
typedef enum MirPixelFormat
{
    mir_pixel_format_invalid,
    mir_pixel_format_abgr_8888,
    mir_pixel_format_xbgr_8888,
    mir_pixel_format_argb_8888,
    mir_pixel_format_xrgb_8888,
    mir_pixel_format_bgr_888
} MirPixelFormat;

/**
 * MirBufferUsage specifies how a surface can and will be used. A "hardware"
 * surface can be used for OpenGL accelerated rendering. A "software" surface
 * is one that can be addressed in main memory and blitted to directly.
 */
typedef enum MirBufferUsage
{
    mir_buffer_usage_hardware = 1,
    mir_buffer_usage_software
} MirBufferUsage;

/**
 * MirSurfaceParameters is the structure of minimum required information that
 * you must provide to Mir in order to create a surface.
 */
typedef struct MirSurfaceParameters
{
    char const *name;
    int width;
    int height;
    MirPixelFormat pixel_format;
    MirBufferUsage buffer_usage;
} MirSurfaceParameters;

enum { mir_platform_package_max = 32 };

/** TODO */
typedef struct MirPlatformPackage
{
    int data_items;
    int fd_items;

    int data[mir_platform_package_max];
    int fd[mir_platform_package_max];
} MirPlatformPackage;

enum { mir_buffer_package_max = 32 };

/** TODO */
typedef struct MirBufferPackage
{
    int data_items;
    int fd_items;

    int data[mir_buffer_package_max];
    int fd[mir_buffer_package_max];

    int stride;
} MirBufferPackage;

/**
 * Retrieved information about a MirSurface. This is most useful for learning
 * how and where to write to a 'mir_buffer_usage_software' surface.
 */
typedef struct MirGraphicsRegion
{
    int width;
    int height;
    int stride;
    MirPixelFormat pixel_format;
    char *vaddr;
} MirGraphicsRegion;

/**
 *  MirEventDelegate may be used to specify (at surface creation time) callbacks for
 *  handling of input events
 */
typedef struct MirEventDelegate
{
    mir_event_delegate_handle_input_callback handle_input;
    void *context;
} MirEventDelegate;

enum { mir_supported_pixel_format_max = 32 };

/**
 * MirDisplayInfo provides details of the graphics environment.
 */
typedef struct MirDisplayInfo
{
    int width;
    int height;
    int supported_pixel_format_items;
    MirPixelFormat supported_pixel_format[mir_supported_pixel_format_max];
} MirDisplayInfo;

/**
 * Request a connection to the Mir server. The supplied callback is called when
 * the connection is established, or fails. The returned wait handle remains
 * valid until the connection has been released.
 *   \param [in] server       A name identifying the server
 *   \param [in] app_name     A name referring to the application
 *   \param [in] callback     Callback function to be invoked when request
 *                            completes
 *   \param [in,out] context  User data passed to the callback function
 *   \return                  A handle that can be passed to mir_wait_for
 */
MirWaitHandle *mir_connect(
    char const *server,
    char const *app_name,
    mir_connected_callback callback,
    void *context);

/**
 * Perform a mir_connect() but also wait for and return the result.
 *   \param [in] server    A name identifying the server
 *   \param [in] app_name  A name referring to the application
 *   \return               The resulting MirConnection
 */
MirConnection *mir_connect_sync(char const *server, char const *app_name);

/**
 * Test for a valid connection
 * \param [in] connection  The connection
 * \return                 A non-zero value if the supplied connection is
 *                         valid, 0 otherwise
 */
int mir_connection_is_valid(MirConnection *connection);

/**
 * Retrieve a text description of the last error. The returned string is owned
 * by the library and remains valid until the connection has been released.
 *   \param [in] connection  The connection
 *   \return                 A text description of any error resulting in an
 *                           invalid connection, or the empty string "" if the
 *                           connection is valid.
 */
char const *mir_connection_get_error_message(MirConnection *connection);

/**
 * Release a connection to the Mir server
 *   \param [in] connection  The connection
 */
void mir_connection_release(MirConnection *connection);

/**
 * Query platform-specific data and/or file descriptors that are required to
 * initialize GL/EGL features.
 *   \param [in]  connection        The connection
 *   \param [out] platform_package  Structure to be populated
 */
void mir_connection_get_platform(MirConnection *connection, MirPlatformPackage *platform_package);

/**
 * Query the display
 *   \param [in]  connection    The connection
 *   \param [out] display_info  Structure to be populated
 */
void mir_connection_get_display_info(MirConnection *connection, MirDisplayInfo *display_info);

/**
 * Get a display type that can be used for OpenGL ES 2.0 acceleration.
 *   \param [in] connection  The connection
 *   \return                 An EGLNativeDisplayType that the client can use
 */
MirEGLNativeDisplayType mir_connection_get_egl_native_display(MirConnection *connection);

/**
 * Request a new Mir surface on the supplied connection with the supplied
 * parameters. The returned handle remains valid until the surface has been
 * released.
 *   \param [in] connection          The connection
 *   \param [in] surface_parameters  Request surface parameters
 *   \param [in] callback            Callback function to be invoked when
 *                                   request completes
 *   \param [in,out] context         User data passed to the callback function
 *   \return                         A handle that can be passed to
 *                                   mir_wait_for
 */
MirWaitHandle *mir_surface_create(
    MirConnection *connection,
    MirSurfaceParameters const *surface_parameters,
    mir_surface_lifecycle_callback callback,
    void *context);

/**
 * Create a surface like in mir_surface_create(), but also wait for creation
 * to complete and return the resulting surface.
 *   \param [in] connection  The connection
 *   \param [in] params      Parameters describing the desired surface
 *   \return                 The resulting surface
 */
MirSurface *mir_surface_create_sync(MirConnection *connection,
                                    MirSurfaceParameters const *params);

/**
 * Get a window type that can be used for OpenGL ES 2.0 acceleration.
 *   \param [in] surface  The surface
 *   \return              An EGLNativeWindowType that the client can use
 */
MirEGLNativeWindowType mir_surface_get_egl_native_window(MirSurface *surface);

/**
 * Test for a valid surface
 *   \param [in] surface  The surface
 *   \return              A non-zero value if the supplied surface is valid,
 *                        0 otherwise
 */
int mir_surface_is_valid(MirSurface *surface);

/**
 * Retrieve a text description of the error. The returned string is owned by
 * the library and remains valid until the surface or the associated
 * connection has been released.
 *   \param [in] surface  The surface
 *   \return              A text description of any error resulting in an
 *                        invalid surface, or the empty string "" if the
 *                        connection is valid.
 */
char const *mir_surface_get_error_message(MirSurface *surface);

/**
 * Get a surface's parameters.
 *   \pre                     The surface is valid
 *   \param [in] surface      The surface
 *   \param [out] parameters  Structure to be populated
 */
void mir_surface_get_parameters(MirSurface *surface, MirSurfaceParameters *parameters);

/**
 * Get a surface's buffer in "raw" representation.
 *   \pre                         The surface is valid
 *   \param [in] surface          The surface
 *   \param [out] buffer_package  Structure to be populated
 */
void mir_surface_get_current_buffer(MirSurface *surface, MirBufferPackage *buffer_package);

/**
 * Get a surface's graphics_region, i.e., map the graphics buffer to main
 * memory.
 *   \pre                          The surface is valid
 *   \param [in] surface           The surface
 *   \param [out] graphics_region  Structure to be populated
 */
void mir_surface_get_graphics_region(
    MirSurface *surface,
    MirGraphicsRegion *graphics_region);

/**
 * Advance a surface's buffer. The returned handle remains valid until the next
 * call to mir_surface_next_buffer, until the surface has been released or the
 * connection to the server has been released.
 *   \param [in] surface      The surface
 *   \param [in] callback     Callback function to be invoked when the request
 *                            completes
 *   \param [in,out] context  User data passed to the callback function
 *   \return                  A handle that can be passed to mir_wait_for
 */
MirWaitHandle *mir_surface_next_buffer(
    MirSurface *surface,
    mir_surface_lifecycle_callback callback,
    void *context);

/**
 * Advance a surface's buffer as in mir_surface_next_buffer(), but also wait
 * for the operation to complete.
 *   \param [in] surface  The surface whose buffer to advance
 */
void mir_surface_next_buffer_sync(MirSurface *surface);

/**
 * Release the supplied surface and any associated buffer. The returned wait
 * handle remains valid until the connection to the server is released.
 *   \param [in] surface      The surface
 *   \param [in] callback     Callback function to be invoked when the request
 *                            completes
 *   \param [in,out] context  User data passed to the callback function
 *   \return                  A handle that can be passed to mir_wait_for
 */
MirWaitHandle *mir_surface_release(
    MirSurface *surface,
    mir_surface_lifecycle_callback callback,
    void *context);

/**
 * Release the specified surface like in mir_surface_release(), but also wait
 * for the operation to complete.
 *   \param [in] surface  The surface to be released
 */
void mir_surface_release_sync(MirSurface *surface);

/**
 * Wait on the supplied handle until the associated request has completed.
 *   \param [in] wait_handle  Handle returned by an asynchronous request
 */
void mir_wait_for(MirWaitHandle *wait_handle);

/**
 * Return the ID of a surface (only useful for debug output).
 *   \param [in] surface  The surface
 *   \return              An internal ID that identifies the surface
 */
int mir_debug_surface_id(MirSurface *surface);

#ifdef __cplusplus
}
}
using namespace mir_toolkit;
#endif

#endif /* MIR_CLIENT_LIBRARY_H */
