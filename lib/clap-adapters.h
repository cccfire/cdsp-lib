#pragma once

#include <clap/clap.h>
#include <clap/ext/gui.h>

#include "app.h"

#define CDSP_EXPORT_CLAP_PLUGIN CLAP_EXPORT const clap_plugin_entry_t clap_entry = { \
     .clap_version = CLAP_VERSION_INIT, \
     .init = cdsp_clap_entry_init, \
     .deinit = cdsp_clap_entry_deinit, \
     .get_factory = cdsp_clap_entry_get_factory \
     }

typedef struct cdsp_clap_feature {
  const char* name;
  void* feature;
  void* (*init_feature) (cdsp_app_t*);
} cdsp_clap_feature_t;

typedef struct cdsp_clap_package {
  cdsp_app_t* app;
  const clap_host_t* host;
  size_t features_length;
  cdsp_clap_feature_t** features;
} cdsp_clap_package_t;

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

const void *cdsp_clap_entry_get_factory(const char *factory_id);

  // Export the plugin entry point
  /*
     CLAP_EXPORT const clap_plugin_entry_t clap_entry = {
     .clap_version = CLAP_VERSION_INIT,
     .init = entry_init,
     .deinit = entry_deinit,
     .get_factory = entry_get_factory
     };
     */
