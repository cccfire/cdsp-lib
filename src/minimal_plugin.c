#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>

#include <clap/clap.h>
#include <pugl/pugl.h>
#include <pugl/gl.h>
#include <cairo.h>

#include <cdsp/app.h>
#include <cdsp/gui.h>
#include <cdsp/dsp.h>
#include <cdsp/clap-adapters.h>
#include <cdsp/clap-extensions-adapters.h>

static void _setup_opengl(cdsp_app_t* app)
{
}

static void _teardown_opengl(cdsp_app_t* app)
{
}

static void _draw(cdsp_app_t* app)
{
  cairo_t *cr = app->gui->cairo_ctx;

  cairo_set_source_rgb(cr, 0.5, 0.05, 0.05);
  cairo_paint(cr);
}

static void _empty_fun(cdsp_app_t* app) { (void)app; }

void create_minimal_app(cdsp_app_t* app)
{
  app->name = "minimal plugin";
  app->type = CDSP_PLUGIN_APP_TYPE;
  app->active = true;

  cdsp_gui_t* gui = (cdsp_gui_t*) calloc(1, sizeof(cdsp_gui_t));
  gui->default_height = 480;
  gui->default_width = 640;
  gui->aspect_ratio_height = 480;
  gui->aspect_ratio_width = 640;
  gui->should_preserve_aspect_ratio = true;
  gui->realized = false;
  gui->setup_opengl = _setup_opengl;
  gui->teardown_opengl = _teardown_opengl;
  gui->draw = _draw;
  gui->init = _empty_fun;
  gui->destroy = _empty_fun;

  cdsp_dsp_t* dsp = (cdsp_dsp_t*) calloc(1, sizeof(cdsp_dsp_t));
  dsp->init = _empty_fun;
  dsp->destroy = _empty_fun;

  app->gui = gui;
  app->dsp = dsp;

  app->init = _empty_fun;
  app->destroy = _empty_fun;
}

CDSP_PLUGIN_DESCRIPTOR(
    "com.example.minimal-clap-plugin",
    "minimal plugin",
    "cccfire",
    "https://example.com",
    "0.1.0",
    "minimal plugin example");

CDSP_SETUP_PLUGIN(create_minimal_app);
