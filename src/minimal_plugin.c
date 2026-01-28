#define NK_IMPLEMENTATION
#include <nuklear.h>
#include <clap/clap.h>
#include <string.h>
#include <stdlib.h>

// Plugin descriptor
static const clap_plugin_descriptor_t s_plugin_desc = {
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
static bool plugin_init(const struct clap_plugin *plugin) {
    return true;
}

static void plugin_destroy(const struct clap_plugin *plugin) {
    free((void *)plugin);
}

static bool plugin_activate(const struct clap_plugin *plugin,
                           double sample_rate,
                           uint32_t min_frames_count,
                           uint32_t max_frames_count) {
    return true;
}

static void plugin_deactivate(const struct clap_plugin *plugin) {
}

static bool plugin_start_processing(const struct clap_plugin *plugin) {
    return true;
}

static void plugin_stop_processing(const struct clap_plugin *plugin) {
}

static void plugin_reset(const struct clap_plugin *plugin) {
}

static clap_process_status plugin_process(const struct clap_plugin *plugin,
                                         const clap_process_t *process) {
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

static const void *plugin_get_extension(const struct clap_plugin *plugin, const char *id) {
    return NULL;
}

static void plugin_on_main_thread(const struct clap_plugin *plugin) {
}

// Factory
static const clap_plugin_t *plugin_factory_create_plugin(const struct clap_plugin_factory *factory,
                                                         const clap_host_t *host,
                                                         const char *plugin_id) {
    if (strcmp(plugin_id, s_plugin_desc.id)) {
        return NULL;
    }

    minimal_plugin_t *plugin_data = (minimal_plugin_t *)calloc(1, sizeof(minimal_plugin_t) + sizeof(clap_plugin_t));
    if (!plugin_data) {
        return NULL;
    }

    plugin_data->host = host;

    clap_plugin_t *plugin = (clap_plugin_t *)(plugin_data + 1);
    plugin->desc = &s_plugin_desc;
    plugin->plugin_data = plugin_data;
    plugin->init = plugin_init;
    plugin->destroy = plugin_destroy;
    plugin->activate = plugin_activate;
    plugin->deactivate = plugin_deactivate;
    plugin->start_processing = plugin_start_processing;
    plugin->stop_processing = plugin_stop_processing;
    plugin->reset = plugin_reset;
    plugin->process = plugin_process;
    plugin->get_extension = plugin_get_extension;
    plugin->on_main_thread = plugin_on_main_thread;

    return plugin;
}

static uint32_t plugin_factory_get_plugin_count(const struct clap_plugin_factory *factory) {
    return 1;
}

static const clap_plugin_descriptor_t *plugin_factory_get_plugin_descriptor(const struct clap_plugin_factory *factory,
                                                                            uint32_t index) {
    return index == 0 ? &s_plugin_desc : NULL;
}

static const clap_plugin_factory_t s_plugin_factory = {
    .get_plugin_count = plugin_factory_get_plugin_count,
    .get_plugin_descriptor = plugin_factory_get_plugin_descriptor,
    .create_plugin = plugin_factory_create_plugin,
};

// Entry point
static bool entry_init(const char *plugin_path) {
    return true;
}

static void entry_deinit(void) {
}

static const void *entry_get_factory(const char *factory_id) {
    if (!strcmp(factory_id, CLAP_PLUGIN_FACTORY_ID)) {
        return &s_plugin_factory;
    }
    return NULL;
}

// Export the plugin entry point
CLAP_EXPORT const clap_plugin_entry_t clap_entry = {
    .clap_version = CLAP_VERSION_INIT,
    .init = entry_init,
    .deinit = entry_deinit,
    .get_factory = entry_get_factory
};
