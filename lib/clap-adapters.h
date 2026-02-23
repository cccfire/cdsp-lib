#pragma once

#include <clap/clap.h>
#include <clap/ext/gui.h>

#include "app.h"

/**
 * What needs to be parameters here?
 * Would like to generate descriptor automatically.
 */

#define CDSP_PLUGIN_DESCRIPTOR(in_id, in_name, in_vendor, in_url, in_version, in_description)\
static const clap_plugin_descriptor_t __cdsp_clap_plugin_desc = {\
    .clap_version = CLAP_VERSION_INIT,\
    .id = in_id,\
    .name = in_name,\
    .vendor = in_vendor,\
    .url = in_url,\
    .manual_url = in_url,\
    .support_url = in_url,\
    .version = in_version,\
    .description = in_description,\
    .features = (const char *[]){CLAP_PLUGIN_FEATURE_AUDIO_EFFECT, CLAP_PLUGIN_FEATURE_STEREO, NULL}\
}

#define CDSP_SETUP_PLUGIN(creator) \
bool cdsp_clap_plugin_init(const struct clap_plugin *plugin)\
{\
  cdsp_app_t* app = (cdsp_app_t*)calloc(1, sizeof(cdsp_app_t));\
  cdsp_create_app_fun_t create_app = creator;\
  create_app(app);\
  cdsp_clap_package_t* plugin_data = ((cdsp_clap_package_t*)plugin->plugin_data);\
  plugin_data->app = app;\
  plugin_data->extensions = cdsp_clap_generate_extensions_from_app(app, &plugin_data->extensions_length);\
  return true;\
}\
const clap_plugin_descriptor_t *cdsp_clap_plugin_factory_get_plugin_descriptor(const struct clap_plugin_factory *factory, \
    uint32_t index) \
{\
  return &__cdsp_clap_plugin_desc;\
}\
\
const clap_plugin_t *cdsp_clap_plugin_factory_create_plugin(const struct clap_plugin_factory *factory,\
    const clap_host_t *host,\
    const char *plugin_id) \
{\
  if (strcmp(plugin_id, __cdsp_clap_plugin_desc.id)) \
  {\
    return NULL;\
  }\
\
  cdsp_clap_package_t *plugin_data = (cdsp_clap_package_t*)calloc(1, sizeof(cdsp_clap_package_t));\
  if (!plugin_data) \
  {\
    return NULL;\
  }\
  \
  plugin_data->host = host;\
\
  clap_plugin_t *plugin = (clap_plugin_t *)calloc(1, sizeof(clap_plugin_t));\
  plugin->desc = &__cdsp_clap_plugin_desc;\
  plugin->plugin_data = plugin_data;\
  plugin->init = cdsp_clap_plugin_init;\
  plugin->destroy = cdsp_clap_plugin_destroy;\
  plugin->activate = cdsp_clap_plugin_activate;\
  plugin->deactivate = cdsp_clap_plugin_deactivate;\
  plugin->start_processing = cdsp_clap_plugin_start_processing;\
  plugin->stop_processing = cdsp_clap_plugin_stop_processing;\
  plugin->reset = cdsp_clap_plugin_reset;\
  plugin->process = cdsp_clap_plugin_process;\
  plugin->get_extension = cdsp_clap_plugin_get_extension;\
  plugin->on_main_thread = cdsp_clap_plugin_on_main_thread;\
\
  fflush(stdout);\
  return plugin;\
}\
const clap_plugin_factory_t __cdsp_clap_plugin_factory = { \
  .get_plugin_count = cdsp_clap_plugin_factory_get_plugin_count, \
  .get_plugin_descriptor = cdsp_clap_plugin_factory_get_plugin_descriptor, \
  .create_plugin = cdsp_clap_plugin_factory_create_plugin\
};\
const void *cdsp_clap_entry_get_factory(const char *factory_id) \
{\
  if (!strcmp(factory_id, CLAP_PLUGIN_FACTORY_ID)) {\
    return &__cdsp_clap_plugin_factory;\
  }\
  return NULL;\
}\
CLAP_EXPORT const clap_plugin_entry_t clap_entry = { \
    .clap_version = CLAP_VERSION_INIT, \
    .init = cdsp_clap_entry_init, \
    .deinit = cdsp_clap_entry_deinit, \
    .get_factory = cdsp_clap_entry_get_factory \
    }

typedef struct cdsp_clap_extension {
  const char* name;
  void* extension;
} cdsp_clap_extension_t;

typedef struct cdsp_clap_package {
  cdsp_app_t* app;
  const clap_host_t* host;
  size_t extensions_length;
  cdsp_clap_extension_t* extensions;
} cdsp_clap_package_t;

const clap_plugin_entry_t cdsp_create_clap_components();

bool cdsp_clap_plugin_init(const struct clap_plugin *plugin);

void cdsp_clap_plugin_destroy(const struct clap_plugin *plugin);

bool cdsp_clap_plugin_activate(const struct clap_plugin *plugin,
    double sample_rate,
    uint32_t min_frames_count,
    uint32_t max_frames_count);

void cdsp_clap_plugin_deactivate(const struct clap_plugin *plugin);

bool cdsp_clap_plugin_start_processing(const struct clap_plugin *plugin);

void cdsp_clap_plugin_stop_processing(const struct clap_plugin *plugin);

void cdsp_clap_plugin_reset(const struct clap_plugin *plugin);

clap_process_status cdsp_clap_plugin_process(const struct clap_plugin *plugin,
    const clap_process_t *process);

const void *cdsp_clap_plugin_get_extension(const struct clap_plugin *plugin, const char *id);

void cdsp_clap_plugin_on_main_thread(const struct clap_plugin *plugin);

// Factory
const clap_plugin_t *cdsp_clap_plugin_factory_create_plugin(
    const struct clap_plugin_factory *factory,
    const clap_host_t *host,
    const char *plugin_id);

uint32_t cdsp_clap_plugin_factory_get_plugin_count(const struct clap_plugin_factory *factory);

const clap_plugin_descriptor_t *cdsp_clap_plugin_factory_get_plugin_descriptor(
    const struct clap_plugin_factory *factory, 
    uint32_t index);

// Entry point
bool cdsp_clap_entry_init(const char *plugin_path);

void cdsp_clap_entry_deinit(void);
