#include <clap/clap.h>
#include <clap/ext/gui.h>

#include "app.h"
#include "gui.h"

bool cdsp_gui_clap_is_api_supported(const clap_plugin_t *plugin, const char *api, bool is_floating);

bool cdsp_gui_clap_get_preferred_api(const clap_plugin_t *plugin, const char **api, bool *is_floating);

bool cdsp_gui_clap_create(const clap_plugin_t *plugin, const char *api, bool is_floating);

void cdsp_gui_clap_destroy(const clap_plugin_t *plugin);

bool cdsp_gui_clap_set_scale(const clap_plugin_t *plugin, double scale);

bool cdsp_gui_clap_get_size(const clap_plugin_t *plugin, uint32_t *width, uint32_t *height);

bool cdsp_gui_clap_can_resize(const clap_plugin_t *plugin);

bool cdsp_gui_clap_get_resize_hints(const clap_plugin_t *plugin, clap_gui_resize_hints_t *hints);

bool cdsp_gui_clap_adjust_size(const clap_plugin_t *plugin, uint32_t *width, uint32_t *height);

bool cdsp_gui_clap_set_size(const clap_plugin_t *plugin, uint32_t width, uint32_t height);

bool cdsp_gui_clap_set_parent(const clap_plugin_t *plugin, const clap_window_t *window);

bool cdsp_gui_clap_set_transient(const clap_plugin_t *plugin, const clap_window_t *window);

void cdsp_gui_clap_suggest_title(const clap_plugin_t *plugin, const char *title);

bool cdsp_gui_clap_show(const clap_plugin_t *plugin);

bool cdsp_gui_clap_hide(const clap_plugin_t *plugin);

void cdsp_init_clap_gui(clap_plugin_gui_t* gui);
