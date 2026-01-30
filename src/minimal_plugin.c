#include <clap/clap.h>
#include <clap/ext/gui.h>
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

/*
cdsp_app_t app = {
  .name = "app"
}

// Export the plugin entry point
CLAP_EXPORT const clap_plugin_entry_t clap_entry = cdsp_create_clap_components(&app);
*/
