#include <clap/clap.h>
#include <clap/ext/gui.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "app.h"
#include "gui.h"
#include "clap-adapters.h"
#include "cdsp-debug-utils.h"

// Plugin instance structure
typedef struct {
  const clap_host_t *host;
} minimal_plugin_t;

void cdsp_clap_plugin_destroy(const struct clap_plugin *plugin) 
{
  cdsp_app_t* app = (cdsp_app_t*)((cdsp_clap_package_t*)plugin->plugin_data)->app;
  cdsp_destroy_app(app);
  free(app);
  cdsp_clap_free_extensions((cdsp_clap_package_t*)plugin->plugin_data);
  free((void*)plugin->plugin_data);
}

bool cdsp_clap_plugin_activate(const struct clap_plugin *plugin,
    double sample_rate,
    uint32_t min_frames_count,
    uint32_t max_frames_count) 
{
  cdsp_app_t* app = (cdsp_app_t*)((cdsp_clap_package_t*)plugin->plugin_data)->app;
  app->sample_rate = sample_rate;
  app->min_frames_count = min_frames_count;
  app->max_frames_count = max_frames_count;
  app->active = true;
  return true;
}

void cdsp_clap_plugin_deactivate(const struct clap_plugin *plugin) 
{
  cdsp_app_t* app = (cdsp_app_t*)((cdsp_clap_package_t*)plugin->plugin_data)->app;
  app->active = false;
}

bool cdsp_clap_plugin_start_processing(const struct clap_plugin *plugin) 
{
  return true;
}

void cdsp_clap_plugin_stop_processing(const struct clap_plugin *plugin) 
{
}

void cdsp_clap_plugin_reset(const struct clap_plugin *plugin) 
{
}

clap_process_status cdsp_clap_plugin_process(const struct clap_plugin *plugin,
    const clap_process_t *process) 
{
  const uint32_t nframes = process->frames_count;
  const uint32_t nev = process->in_events->size(process->in_events);


  if (process->audio_outputs_count == 0) {
    // cdsp_log("zero audio_outputs\n");
    return CLAP_PROCESS_CONTINUE;
  }
  if (process->audio_inputs_count == 0) {
    // cdsp_log("zero audio_inputs\n");
    return CLAP_PROCESS_CONTINUE;
  }
  if (!process->audio_outputs) {
    // cdsp_log("null audio output\n");
    return CLAP_PROCESS_CONTINUE;
  }
  if (!process->audio_inputs) {
    // cdsp_log("null audio input\n");
    return CLAP_PROCESS_CONTINUE;
  }

  // Simple passthrough - copy input to output
  for (uint32_t ch = 0; ch < process->audio_outputs[0].channel_count; ++ch) {
    if (ch < process->audio_inputs[0].channel_count) {
      memcpy(process->audio_outputs[0].data32[ch],
          process->audio_inputs[0].data32[ch],
          nframes * sizeof(float));
    } else {
      memset(process->audio_outputs[0].data32[ch], 0, nframes * sizeof(float));
    }
  }


  return CLAP_PROCESS_CONTINUE;
}

const void *cdsp_clap_plugin_get_extension(const struct clap_plugin *plugin, const char *id) 
{
  cdsp_log("queried extension: ");
  cdsp_log(id);
  cdsp_log("\n");
  cdsp_clap_extension_t* extensions = ((cdsp_clap_package_t*)plugin->plugin_data)->extensions;
  size_t extensions_length = ((cdsp_clap_package_t*)plugin->plugin_data)->extensions_length;
  for (size_t i = 0; i < extensions_length; i++) {
    if (!strcmp(extensions[i].name, id)) {
      cdsp_log("we found it\n");
      return extensions[i].extension;
    }
  }

  return NULL;
}

void cdsp_clap_plugin_on_main_thread(const struct clap_plugin *plugin) 
{
  cdsp_app_t* app = (cdsp_app_t*)((cdsp_clap_package_t*)plugin->plugin_data)->app;
  cdsp_gui_update(app);
  clap_host_t* host = (clap_host_t*)((cdsp_clap_package_t*)plugin->plugin_data)->host;
  host->request_callback(host);
}

uint32_t cdsp_clap_plugin_factory_get_plugin_count(const struct clap_plugin_factory *factory) 
{
  return 1;
}

// Entry point
bool cdsp_clap_entry_init(const char *plugin_path) 
{
  cdsp_log("entry init\n");
  return true;
}

void cdsp_clap_entry_deinit(void) 
{
  cdsp_log("entry deinit\n");
}

// Export the plugin entry point


/*
   const void *Plugin<h, l>::clapExtension(const clap_plugin *plugin, const char *id) noexcept 
   {
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
