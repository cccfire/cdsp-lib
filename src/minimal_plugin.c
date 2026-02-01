#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <clap/clap.h>

#include <cdsp/app.h>
#include <cdsp/gui.h>
#include <cdsp/dsp.h>
#include <cdsp/clap-adapters.h>
#include <cdsp/clap-features-adapters.h>

void create_minimal_app(cdsp_app_t* app)
{
  app->name = "minimal app";
  app->type = CDSP_PLUGIN_APP_TYPE;
  
  cdsp_gui_t* gui = (cdsp_gui_t*) calloc(1, sizeof(cdsp_gui_t));
  gui->default_height = 480;
  gui->default_width = 640;
  gui->should_preserve_aspect_ratio = true;

  cdsp_dsp_t* dsp = (cdsp_dsp_t*) calloc(1, sizeof(cdsp_dsp_t));

  app->gui = gui;
  app->dsp = dsp;
  
  
  printf("Hello World\n");
}

CDSP_PLUGIN_DESCRIPTOR(
    "com.example.minimal-clap-plugin",
    "minimal plugin",
    "cccfire",
    "https://example.com",
    "0.1.0",
    "minimal plugin example");

CDSP_SETUP_PLUGIN(create_minimal_app);
