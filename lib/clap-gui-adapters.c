#include <pugl/pugl.h>
#include <pugl/gl.h>
#include <clap/clap.h>
#include <clap/ext/gui.h>
#include <string.h>

#include "app.h"
#include "gui.h"

bool cdsp_gui_clap_is_api_supported(const clap_plugin_t *plugin, const char *api, bool is_floating) 
{
  if (is_floating)
    return false;
#if IS_MAC
  if (strcmp(api, CLAP_WINDOW_API_COCOA) == 0)
    return true;
#endif

#if IS_WIN
  if (strcmp(api, CLAP_WINDOW_API_WIN32) == 0)
    return true;
#endif

#if IS_LINUX
  if (strcmp(api, CLAP_WINDOW_API_X11) == 0)
    return true;
#endif

  return false;
}

bool cdsp_gui_clap_get_preferred_api(const clap_plugin_t *plugin, const char **api, bool *is_floating) 
{
  *is_floating = false;
#if IS_MAC
  api = &CLAP_WINDOW_API_COCOA;
  return true;
#endif

#if IS_WIN
  api = &CLAP_WINDOW_API_WIN32;
  return true;
#endif

#if IS_LINUX
  api = &CLAP_WINDOW_API_X11;
  return true;
#endif

  return false;
}

bool cdsp_gui_clap_create(const clap_plugin_t *plugin, const char *api, bool is_floating)
{
  cdsp_app_t* app = (cdsp_app_t*)plugin->plugin_data;
  cdsp_gui_t* gui = app->gui;

  cdsp_init_gui(app, gui);

  return true;
}

void cdsp_gui_clap_destroy(const clap_plugin_t *plugin)
{
  cdsp_app_t* app = (cdsp_app_t*)plugin->plugin_data;
  cdsp_gui_t* gui = app->gui;

  cdsp_destroy_gui(app, gui);
}

/**
 * not sure how I want to approach scale yet
 */
bool cdsp_gui_clap_set_scale(const clap_plugin_t *plugin, double scale)
{
  cdsp_app_t* app = (cdsp_app_t*)plugin->plugin_data;
  cdsp_gui_t* gui = app->gui;

  assert(app->gui);
  return false;
}

bool cdsp_gui_clap_get_size(const clap_plugin_t *plugin, uint32_t *width, uint32_t *height)
{
  cdsp_app_t* app = (cdsp_app_t*)plugin->plugin_data;
  cdsp_gui_t* gui = app->gui;
  cdsp_gui_get_size(app, gui, width, height);

  return false;
}

/**
 * We want to maintain aspect ratio later
 */

bool cdsp_gui_clap_can_resize(const clap_plugin_t *plugin)
{
  return true;
}

bool cdsp_gui_clap_get_resize_hints(const clap_plugin_t *plugin, clap_gui_resize_hints_t *hints)
{
  cdsp_app_t* app = (cdsp_app_t*)plugin->plugin_data;
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
  cdsp_app_t* app = (cdsp_app_t*)plugin->plugin_data;
  cdsp_gui_t* gui = app->gui;

  return cdsp_gui_adjust_size(app, gui, width, height);
}

bool cdsp_gui_clap_set_size(const clap_plugin_t *plugin, uint32_t width, uint32_t height)
{
  cdsp_app_t* app = (cdsp_app_t*)plugin->plugin_data;
  cdsp_gui_t* gui = app->gui;
  return cdsp_gui_set_size(app, gui, width, height);
}

bool cdsp_gui_clap_set_parent(const clap_plugin_t *plugin, const clap_window_t *window)
{
  cdsp_app_t* app = (cdsp_app_t*)plugin->plugin_data;
  cdsp_gui_t* gui = app->gui;
  return cdsp_gui_set_parent(app, gui, (const void*) window);
}

bool cdsp_gui_clap_set_transient(const clap_plugin_t *plugin, const clap_window_t *window)
{
  cdsp_app_t* app = (cdsp_app_t*)plugin->plugin_data;
  cdsp_gui_t* gui = app->gui;
  return cdsp_gui_set_transient(app, gui, (const void*) window);
}

void cdsp_gui_clap_suggest_title(const clap_plugin_t *plugin, const char *title)
{
  cdsp_app_t* app = (cdsp_app_t*)plugin->plugin_data;
  cdsp_gui_t* gui = app->gui;
  cdsp_gui_suggest_title(app, gui, title);
}

bool cdsp_gui_clap_show(const clap_plugin_t *plugin)
{
  cdsp_app_t* app = (cdsp_app_t*)plugin->plugin_data;
  cdsp_gui_t* gui = app->gui;
  return cdsp_gui_show(app, gui);
}

bool cdsp_gui_clap_hide(const clap_plugin_t *plugin)
{
  cdsp_app_t* app = (cdsp_app_t*)plugin->plugin_data;
  cdsp_gui_t* gui = app->gui;
  return cdsp_gui_hide(app, gui);
}

/*

   const void *Plugin<h, l>::clapExtension(const clap_plugin *plugin, const char *id) noexcept {
   auto &self = from(plugin);
   self.ensureInitialized("extension");

   if (!strcmp(id, CLAP_EXT_STATE) && self.implementsState())(app, gui);(app, gui);


   return &_pluginState;
   if (!strcmp(id, CLAP_EXT_STATE_CONTEXT) && self.implementsStateContext() && self.implementsState())
   return &_pluginStateContext;
   if ((!strcmp(id, CLAP_EXT_PRESET_LOAD) || !strcmp(id, CLAP_EXT_PRESET_LOAD_COMPAT)) &&
   self.implementsPresetLoad())
   return &_pluginPresetLoad;
   if (!strcmp(id, CLAP_EXT_RENDER) && self.implementsRender())
   return &_pluginRender;
   if ((!strcmp(id, CLAP_EXT_TRACK_INFO) || !strcmp(id, CLAP_EXT_TRACK_INFO_COMPAT)) &&
   self.implementsTrackInfo())
   return &_pluginTrackInfo;
   if (!strcmp(id, CLAP_EXT_LATENCY) && self.implementsLatency())
   return &_pluginLatency;
   if (!strcmp(id, CLAP_EXT_AUDIO_PORTS) && self.implementsAudioPorts())
   return &_pluginAudioPorts;
   if ((!strcmp(id, CLAP_EXT_AUDIO_PORTS_ACTIVATION) ||
   !strcmp(id, CLAP_EXT_AUDIO_PORTS_ACTIVATION_COMPAT)) &&
   self.implementsAudioPorts())
   return &_pluginAudioPortsActivation;
   if (!strcmp(id, CLAP_EXT_AUDIO_PORTS_CONFIG) && self.implementsAudioPortsConfig())
   return &_pluginAudioPortsConfig;
   if (!strcmp(id, CLAP_EXT_PARAMS) && self.implementsParams())
   return &_pluginParams;
   if ((!strcmp(id, CLAP_EXT_PARAM_INDICATION) ||
   !strcmp(id, CLAP_EXT_PARAM_INDICATION_COMPAT)) &&
   self.implementsParamIndication())
   return &_pluginParamIndication;
   if ((!strcmp(id, CLAP_EXT_REMOTE_CONTROLS) || !strcmp(id, CLAP_EXT_REMOTE_CONTROLS_COMPAT)) &&
   self.implementRemoteControls())
   return &_pluginRemoteControls;
   if (!strcmp(id, CLAP_EXT_NOTE_PORTS) && self.implementsNotePorts())
   return &_pluginNotePorts;
   if (!strcmp(id, CLAP_EXT_NOTE_NAME) && self.implementsNoteName())
   return &_pluginNoteName;
   if (!strcmp(id, CLAP_EXT_THREAD_POOL) && self.implementsThreadPool())
   return &_pluginThreadPool;
   if (!strcmp(id, CLAP_EXT_TIMER_SUPPORT) && self.implementsTimerSupport())
   return &_pluginTimerSupport;
   if (!strcmp(id, CLAP_EXT_POSIX_FD_SUPPORT) && self.implementsPosixFdSupport())
   return &_pluginPosixFdSupport;
   if (!strcmp(id, CLAP_EXT_GUI) && self.implementsGui())
   return &_pluginGui;
   if (!strcmp(id, CLAP_EXT_VOICE_INFO) && self.implementsVoiceInfo())
   return &_pluginVoiceInfo;
   if (!strcmp(id, CLAP_EXT_TAIL) && self.implementsTail())
   return &_pluginTail;
   if ((!strcmp(id, CLAP_EXT_CONTEXT_MENU) || !strcmp(id, CLAP_EXT_CONTEXT_MENU_COMPAT)) &&
   self.implementsContextMenu())
   return &_pluginContextMenu;


*/
