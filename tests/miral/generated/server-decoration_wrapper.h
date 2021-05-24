/*
 * AUTOGENERATED - DO NOT EDIT
 *
 * This file is generated from server-decoration.xml
 * To regenerate, run the “refresh-wayland-wrapper” target.
 */

#ifndef MIR_FRONTEND_WAYLAND_SERVER_DECORATION_XML_WRAPPER
#define MIR_FRONTEND_WAYLAND_SERVER_DECORATION_XML_WRAPPER

#include <experimental/optional>

#include "mir/fd.h"
#include <wayland-server-core.h>

#include "mir/wayland/wayland_base.h"

namespace mir
{
namespace wayland
{

class ServerDecorationManager;
class ServerDecoration;

class ServerDecorationManager : public Resource
{
public:
    static char const constexpr* interface_name = "org_kde_kwin_server_decoration_manager";

    static ServerDecorationManager* from(struct wl_resource*);

    ServerDecorationManager(struct wl_resource* resource, Version<1>);
    virtual ~ServerDecorationManager();

    void send_default_mode_event(uint32_t mode) const;

    void destroy_and_delete() const;

    struct wl_client* const client;
    struct wl_resource* const resource;

    struct Mode
    {
        static uint32_t const None = 0;
        static uint32_t const Client = 1;
        static uint32_t const Server = 2;
    };

    struct Opcode
    {
        static uint32_t const default_mode = 0;
    };

    struct Thunks;

    static bool is_instance(wl_resource* resource);

    class Global : public wayland::Global
    {
    public:
        Global(wl_display* display, Version<1>);

        auto interface_name() const -> char const* override;

    private:
        virtual void bind(wl_resource* new_org_kde_kwin_server_decoration_manager) = 0;
        friend ServerDecorationManager::Thunks;
    };

private:
    virtual void create(struct wl_resource* id, struct wl_resource* surface) = 0;
};

class ServerDecoration : public Resource
{
public:
    static char const constexpr* interface_name = "org_kde_kwin_server_decoration";

    static ServerDecoration* from(struct wl_resource*);

    ServerDecoration(struct wl_resource* resource, Version<1>);
    virtual ~ServerDecoration();

    void send_mode_event(uint32_t mode) const;

    struct wl_client* const client;
    struct wl_resource* const resource;

    struct Mode
    {
        static uint32_t const None = 0;
        static uint32_t const Client = 1;
        static uint32_t const Server = 2;
    };

    struct Opcode
    {
        static uint32_t const mode = 0;
    };

    struct Thunks;

    static bool is_instance(wl_resource* resource);

private:
    virtual void request_mode(uint32_t mode) = 0;
};

}
}

#endif // MIR_FRONTEND_WAYLAND_SERVER_DECORATION_XML_WRAPPER
