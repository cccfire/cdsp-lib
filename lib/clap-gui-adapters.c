#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>

#include <clap/clap.h>
#include <clap/ext/gui.h>

#include "clap-adapters.h"
#include "clap-gui-adapters.h"
#include "cdsp-debug-utils.h"

#include "app.h"
#include "gui.h"

bool cdsp_gui_clap_is_api_supported(const clap_plugin_t *plugin, const char *api, bool is_floating)
{
  cdsp_log("clap gui is api supported\n");
  cdsp_log(api);
  cdsp_log("\n");
  if (is_floating)
    return true;
#if IS_MAC
  if (strcmp(api, CLAP_WINDOW_API_COCOA) == 0) {
    cdsp_log("api is supported\n");
    return true;
  }
#endif

#if IS_WIN
  cdsp_log("is win!\n");
  if (strcmp(api, CLAP_WINDOW_API_WIN32) == 0) {
    cdsp_log("api is supported\n");
    return true;
  }
#endif

#if IS_LINUX
  if (strcmp(api, CLAP_WINDOW_API_X11) == 0) {
    cdsp_log("api is supported\n");
    return true;
  }
#endif

  return false;
}

bool cdsp_gui_clap_get_preferred_api(const clap_plugin_t *plugin, const char **api, bool *is_floating)
{
  cdsp_log("clap gui get preferred\n");
  *is_floating = false;
#if IS_MAC
  *api = CLAP_WINDOW_API_COCOA;
  return true;
#endif

#if IS_WIN
  *api = CLAP_WINDOW_API_WIN32;
  return true;
#endif

#if IS_LINUX
  *api = CLAP_WINDOW_API_X11;
  return true;
#endif

  return false;
}

bool cdsp_gui_clap_create(const clap_plugin_t *plugin, const char *api, bool is_floating)
{
  cdsp_log("clap gui clap create\n");
  cdsp_app_t* app = (cdsp_app_t*)((cdsp_clap_package_t*)plugin->plugin_data)->app;
  cdsp_gui_t* gui = app->gui;

  cdsp_gui_init(app);

  return true;
}

void cdsp_gui_clap_destroy(const clap_plugin_t *plugin)
{
  cdsp_log("clap gui clap destroy\n");
  cdsp_app_t* app = (cdsp_app_t*)((cdsp_clap_package_t*)plugin->plugin_data)->app;
  cdsp_gui_t* gui = app->gui;

  cdsp_gui_destroy(app);
}

/**
 * not sure how I want to approach scale yet
 */
bool cdsp_gui_clap_set_scale(const clap_plugin_t *plugin, double scale)
{
  cdsp_log("clap gui set scale\n");
  cdsp_app_t* app = (cdsp_app_t*)((cdsp_clap_package_t*)plugin->plugin_data)->app;
  cdsp_gui_t* gui = app->gui;

  assert(app->gui);
  return true;
}

bool cdsp_gui_clap_get_size(const clap_plugin_t *plugin, uint32_t *width, uint32_t *height)
{
  cdsp_log("clap gui get size\n");
  cdsp_app_t* app = (cdsp_app_t*)((cdsp_clap_package_t*)plugin->plugin_data)->app;
  cdsp_gui_t* gui = app->gui;
  cdsp_gui_get_size(app, width, height);

  return true;
}

/**
 * We want to maintain aspect ratio later
 */

bool cdsp_gui_clap_can_resize(const clap_plugin_t *plugin)
{
  cdsp_log("clap gui can resize\n");
  return true;
}

bool cdsp_gui_clap_get_resize_hints(const clap_plugin_t *plugin, clap_gui_resize_hints_t *hints)
{
  cdsp_log("clap gui get resize hints\n");
  cdsp_app_t* app = (cdsp_app_t*)((cdsp_clap_package_t*)plugin->plugin_data)->app;
  cdsp_gui_t* gui = app->gui;

  hints->can_resize_horizontally = true;
  hints->can_resize_vertically = true;
  hints->preserve_aspect_ratio = gui->should_preserve_aspect_ratio;
  hints->aspect_ratio_width = gui->aspect_ratio_width;
  hints->aspect_ratio_height = gui->aspect_ratio_height;

  return true;
}

bool cdsp_gui_clap_adjust_size(const clap_plugin_t *plugin, uint32_t *width, uint32_t *height)
{
  cdsp_log("adjust size\n");
  cdsp_app_t* app = (cdsp_app_t*)((cdsp_clap_package_t*)plugin->plugin_data)->app;
  cdsp_gui_t* gui = app->gui;
  return cdsp_gui_adjust_size(app, width, height);
}

bool cdsp_gui_clap_set_size(const clap_plugin_t *plugin, uint32_t width, uint32_t height)
{
  cdsp_log("clap gui set size\n");
  cdsp_app_t* app = (cdsp_app_t*)((cdsp_clap_package_t*)plugin->plugin_data)->app;
  cdsp_gui_t* gui = app->gui;
  return cdsp_gui_set_size(app, width, height);
}

bool cdsp_gui_clap_set_parent(const clap_plugin_t *plugin, const clap_window_t *window)
{
  cdsp_app_t* app = (cdsp_app_t*)((cdsp_clap_package_t*)plugin->plugin_data)->app;
  cdsp_gui_t* gui = app->gui;
  cdsp_log("clap gui set parent\n");
  if (window->api == CLAP_WINDOW_API_WIN32)
    return cdsp_gui_set_parent(app, (const void*) window->win32);
  else if (window->api == CLAP_WINDOW_API_COCOA)
    return cdsp_gui_set_parent(app, (const void*) window->cocoa);
  else if (window->api == CLAP_WINDOW_API_X11)
    return cdsp_gui_set_parent(app, (const void*) (uintptr_t)window->x11);
  else
    return cdsp_gui_set_parent(app, (const void*) window->ptr);
}

bool cdsp_gui_clap_set_transient(const clap_plugin_t *plugin, const clap_window_t *window)
{
  cdsp_app_t* app = (cdsp_app_t*)((cdsp_clap_package_t*)plugin->plugin_data)->app;
  cdsp_gui_t* gui = app->gui;

  cdsp_log("clap gui set transient parent\n");

  // not a bug here that we call set_parent instead of set_transient!
  // this behaves better
  if (window->api == CLAP_WINDOW_API_WIN32)
    return cdsp_gui_set_parent(app, (const void*) window->win32);
  else if (window->api == CLAP_WINDOW_API_COCOA)
    return cdsp_gui_set_parent(app, (const void*) window->cocoa);
  else if (window->api == CLAP_WINDOW_API_X11)
    return cdsp_gui_set_parent(app, (const void*) (uintptr_t)window->x11);
  else
    return cdsp_gui_set_parent(app, (const void*) window->ptr);
}

void cdsp_gui_clap_suggest_title(const clap_plugin_t *plugin, const char *title)
{
  cdsp_log("clap gui suggest title\n");
  cdsp_app_t* app = (cdsp_app_t*)((cdsp_clap_package_t*)plugin->plugin_data)->app;
  cdsp_gui_t* gui = app->gui;
  cdsp_gui_set_title(app, title);
}

bool cdsp_gui_clap_show(const clap_plugin_t *plugin)
{
  cdsp_log("clap gui show\n");
  cdsp_app_t* app = (cdsp_app_t*)((cdsp_clap_package_t*)plugin->plugin_data)->app;
  cdsp_gui_t* gui = app->gui;
  clap_host_t* host = (clap_host_t*)((cdsp_clap_package_t*)plugin->plugin_data)->host;
  clap_host_gui_t* host_gui = (clap_host_gui_t*)host->get_extension(host, CLAP_EXT_GUI);

  bool stat = host_gui->request_resize(host, app->gui->default_width, app->gui->default_height);
  host->request_callback(host);
  FILE* f = fopen(CDSP_DEBUG_FILE_PATH, "a");
  if (!f) return false;
  fprintf(f, "request resize status (%d) %d, %d\n", (int)stat, app->gui->default_width, app->gui->default_height);
  fflush(f);
  fclose(f);

  return cdsp_gui_show(app);
}

bool cdsp_gui_clap_hide(const clap_plugin_t *plugin)
{
  cdsp_log("clap gui hide\n");
  cdsp_app_t* app = (cdsp_app_t*)((cdsp_clap_package_t*)plugin->plugin_data)->app;
  cdsp_gui_t* gui = app->gui;
  return cdsp_gui_hide(app);
}

void cdsp_init_clap_gui(clap_plugin_gui_t* gui)
{
  gui->is_api_supported = cdsp_gui_clap_is_api_supported;
  gui->get_preferred_api = cdsp_gui_clap_get_preferred_api;
  gui->create = cdsp_gui_clap_create;
  gui->destroy = cdsp_gui_clap_destroy;
  gui->set_scale = cdsp_gui_clap_set_scale;
  gui->get_size = cdsp_gui_clap_get_size;
  gui->can_resize = cdsp_gui_clap_can_resize;
  gui->get_resize_hints = cdsp_gui_clap_get_resize_hints;
  gui->adjust_size = cdsp_gui_clap_adjust_size;
  gui->set_size = cdsp_gui_clap_set_size;
  gui->set_parent = cdsp_gui_clap_set_parent;
  gui->set_transient = cdsp_gui_clap_set_transient;
  gui->suggest_title = cdsp_gui_clap_suggest_title;
  gui->show = cdsp_gui_clap_show;
  gui->hide = cdsp_gui_clap_hide;
}
