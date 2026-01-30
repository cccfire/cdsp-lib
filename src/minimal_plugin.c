#include <clap/clap.h>
#include <cdsp/app.h>
#include <cdsp/clap-adapters.h>

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

CDSP_EXPORT_CLAP_PLUGIN;
