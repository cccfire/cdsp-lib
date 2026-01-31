#include <clap/clap.h>
#include <cdsp/app.h>
#include <cdsp/clap-adapters.h>

#include <string.h>
#include <stdlib.h>

void create_minimal_app(cdsp_app_t* app)
{
  app->name = "minimal app";
}

CDSP_PLUGIN_DESCRIPTOR(
    "com.example.minimal-clap-plugin",
    "minimal plugin",
    "cccfire",
    "https://example.com",
    "0.1.0",
    "minimal plugin example");

CDSP_SETUP_PLUGIN(create_minimal_app);
