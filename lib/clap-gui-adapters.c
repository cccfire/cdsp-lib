#include <pugl/pugl.h>
#include <pugl/gl.h>
#include <clap/clap.h>
#include <clap/ext/gui.h>
#include <string.h>

#include "app.h"
#include "gui.h"

bool cdsp_gui_create(const clap_plugin_t *plugin, const char *api, bool is_floating)
{
  cdsp_app_t* app = (cdsp_app_t*)plugin->plugin_data;
  cdsp_gui_t* gui = app->gui;

  cdsp_init_gui(app, gui);
  return true;
}

void cdsp_gui_destroy(const clap_plugin_t *plugin)
{
  cdsp_app_t* app = (cdsp_app_t*)plugin->plugin_data;
  cdsp_gui_t* gui = app->gui;
  cdsp_destroy_gui(app, gui);
}

bool cdsp_gui_is_api_supported(const clap_plugin_t *plugin, const char *api, bool is_floating) 
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

bool cdsp_gui_get_preferred_api(const clap_plugin_t *plugin, const char **api, bool *is_floating) 
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

/*

   const void *Plugin<h, l>::clapExtension(const clap_plugin *plugin, const char *id) noexcept {
      auto &self = from(plugin);
      self.ensureInitialized("extension");

      if (!strcmp(id, CLAP_EXT_STATE) && self.implementsState())
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
