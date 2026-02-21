#include <clap/clap.h>
#include <clap/ext/gui.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "app.h"
#include "gui.h"
#include "clap-adapters.h"

// Plugin descriptor
const clap_plugin_descriptor_t s_plugin_desc = {
  .clap_version = CLAP_VERSION_INIT,
  .id = "com.example.minimal-clap-plugin",
  .name = "Minimal CLAP Plugin",
  .vendor = "Example Vendor",
  .url = "https://example.com",
  .manual_url = "https://example.com/manual",
  .support_url = "https://example.com/support",
  .version = "1.0.0",
  .description = "A minimal CLAP plugin example",
  .features = (const char *[]){CLAP_PLUGIN_FEATURE_AUDIO_EFFECT, CLAP_PLUGIN_FEATURE_STEREO, NULL}
};

// Plugin instance structure
typedef struct {
  const clap_host_t *host;
} minimal_plugin_t;

void cdsp_clap_plugin_destroy(const struct clap_plugin *plugin) 
{
  cdsp_app_t* app = (cdsp_app_t*)((cdsp_clap_package_t*)plugin->plugin_data)->app;
  cdsp_destroy_app(app);
  free(app);
  cdsp_clap_free_features((cdsp_clap_package_t*)plugin->plugin_data);
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
  cdsp_clap_feature_t* features = ((cdsp_clap_package_t*)plugin->plugin_data)->features;
  size_t features_length = ((cdsp_clap_package_t*)plugin->plugin_data)->features_length;
  for (size_t i = 0; i < features_length; i++) {
    if (!strcmp(features[i].name, id)) {
      return features[i].feature;
    }
  }

  return NULL;
}

void cdsp_clap_plugin_on_main_thread(const struct clap_plugin *plugin) 
{
  cdsp_app_t* app = (cdsp_app_t*)((cdsp_clap_package_t*)plugin->plugin_data)->app;
  cdsp_gui_update(app);
}

uint32_t cdsp_clap_plugin_factory_get_plugin_count(const struct clap_plugin_factory *factory) 
{
  return 1;
}

// Entry point
bool cdsp_clap_entry_init(const char *plugin_path) 
{
  return true;
}

void cdsp_clap_entry_deinit(void) 
{
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
