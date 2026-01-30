#include <clap/clap.h>
#include <clap/ext/gui.h>
#include <string.h>
#include <stdlib.h>

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

// Plugin callbacks
bool cdsp_clap_plugin_init(const struct clap_plugin *plugin) 
{
  return true;
}

void cdsp_clap_plugin_destroy(const struct clap_plugin *plugin) 
{
  free((cdsp_clap_package_t*)plugin->plugin_data);
  free((void *)plugin);
}

bool cdsp_clap_plugin_activate(const struct clap_plugin *plugin,
    double sample_rate,
    uint32_t min_frames_count,
    uint32_t max_frames_count) 
{
  return true;
}

void cdsp_clap_plugin_deactivate(const struct clap_plugin *plugin) 
{
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

  if (strcmp(id, CLAP_EXT_GUI)) {
    return NULL;
  }
  return NULL;
}

void cdsp_clap_plugin_on_main_thread(const struct clap_plugin *plugin) 
{
}

// Factory
const clap_plugin_t *cdsp_clap_plugin_factory_create_plugin(const struct clap_plugin_factory *factory,
    const clap_host_t *host,
    const char *plugin_id) 
{
  if (strcmp(plugin_id, s_plugin_desc.id)) 
  {
    return NULL;
  }

  cdsp_clap_package_t *plugin_data = (cdsp_clap_package_t*)calloc(1, sizeof(cdsp_clap_package_t) + sizeof(clap_plugin_t));
  if (!plugin_data) 
  {
    return NULL;
  }

  plugin_data->host = host;

  clap_plugin_t *plugin = (clap_plugin_t *)(plugin_data + 1);
  plugin->desc = &s_plugin_desc;
  plugin->plugin_data = plugin_data;
  plugin->init = cdsp_clap_plugin_init;
  plugin->destroy = cdsp_clap_plugin_destroy;
  plugin->activate = cdsp_clap_plugin_activate;
  plugin->deactivate = cdsp_clap_plugin_deactivate;
  plugin->start_processing = cdsp_clap_plugin_start_processing;
  plugin->stop_processing = cdsp_clap_plugin_stop_processing;
  plugin->reset = cdsp_clap_plugin_reset;
  plugin->process = cdsp_clap_plugin_process;
  plugin->get_extension = cdsp_clap_plugin_get_extension;
  plugin->on_main_thread = cdsp_clap_plugin_on_main_thread;

  return plugin;
}

uint32_t cdsp_clap_plugin_factory_get_plugin_count(const struct clap_plugin_factory *factory) 
{
  return 1;
}

const clap_plugin_descriptor_t *cdsp_clap_plugin_factory_get_plugin_descriptor(const struct clap_plugin_factory *factory,
    uint32_t index) 
{
  return index == 0 ? &s_plugin_desc : NULL;
}

const clap_plugin_factory_t s_plugin_factory = {
  .get_plugin_count = cdsp_clap_plugin_factory_get_plugin_count,
  .get_plugin_descriptor = cdsp_clap_plugin_factory_get_plugin_descriptor,
  .create_plugin = cdsp_clap_plugin_factory_create_plugin,
};

// Entry point
bool cdsp_clap_entry_init(const char *plugin_path) 
{
  return true;
}

void cdsp_clap_entry_deinit(void) 
{
}

const void *cdsp_clap_entry_get_factory(const char *factory_id) 
{
  if (!strcmp(factory_id, CLAP_PLUGIN_FACTORY_ID)) {
    return &s_plugin_factory;
  }
  return NULL;
}

const clap_plugin_entry_t cdsp_create_clap_components(cdsp_app_t* app) 
{
  clap_plugin_entry_t entry = {
    .clap_version = CLAP_VERSION_INIT,
    .init = cdsp_clap_entry_init,
    .deinit = cdsp_clap_entry_deinit,
    .get_factory = cdsp_clap_entry_get_factory
  };
  return entry;
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
