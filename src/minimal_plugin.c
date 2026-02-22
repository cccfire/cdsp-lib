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
#include <cdsp/clap-features-adapters.h>

/* Cairo ARGB32 is stored as B,G,R,A in memory on little-endian; GL_BGRA matches. */
#ifndef GL_BGRA
#  define GL_BGRA 0x80E1
#endif
#ifndef GL_CLAMP_TO_EDGE
#  define GL_CLAMP_TO_EDGE 0x812F
#endif

typedef struct {
  GLuint texture;
} gl_state_t;

static void _setup_opengl(cdsp_app_t* app)
{
  printf("setup opengl\n");
  gl_state_t *gl = (gl_state_t*) calloc(1, sizeof(gl_state_t));
  app->gui->data = gl;

  glEnable(GL_TEXTURE_2D);
  glGenTextures(1, &gl->texture);
  glBindTexture(GL_TEXTURE_2D, gl->texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

static void _teardown_opengl(cdsp_app_t* app)
{
  printf("teardown opengl\n");
  gl_state_t *gl = (gl_state_t*) app->gui->data;
  if (gl) {
    glDeleteTextures(1, &gl->texture);
    free(gl);
    app->gui->data = NULL;
  }
}

static void _draw(cdsp_app_t* app)
{
  gl_state_t *gl = (gl_state_t*) app->gui->data;
  if (!gl) return;

  cairo_t *cr = app->gui->cairo_ctx;
  cairo_surface_t *surface = cairo_get_target(cr);
  int w = cairo_image_surface_get_width(surface);
  int h = cairo_image_surface_get_height(surface);
  if (w <= 0 || h <= 0) return;

  /* --- Cairo drawing --- */
  cairo_set_source_rgb(cr, 0.5, 0.05, 0.05);
  cairo_paint(cr);

  /* --- Upload Cairo surface as GL texture --- */
  cairo_surface_flush(surface);
  unsigned char *data = cairo_image_surface_get_data(surface);

  glBindTexture(GL_TEXTURE_2D, gl->texture);
  /* GL_BGRA matches Cairo's native ARGB32 byte order on little-endian. */
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
               GL_BGRA, GL_UNSIGNED_BYTE, data);

  /* --- Blit fullscreen textured quad (fixed-function compat profile) ---
   *
   * UV layout: t=0 maps to the first row of data (Cairo top).
   * NDC y=+1 is the top of the screen, so pair (y=+1, t=0) to avoid flip.
   */
  glClear(GL_COLOR_BUFFER_BIT);
  glMatrixMode(GL_PROJECTION); glLoadIdentity();
  glMatrixMode(GL_MODELVIEW);  glLoadIdentity();

  glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f,  1.0f); /* top-left     */
    glTexCoord2f(1.0f, 0.0f); glVertex2f( 1.0f,  1.0f); /* top-right    */
    glTexCoord2f(1.0f, 1.0f); glVertex2f( 1.0f, -1.0f); /* bottom-right */
    glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f, -1.0f); /* bottom-left  */
  glEnd();
}

static void _empty_fun(cdsp_app_t* app) { (void)app; }

void create_minimal_app(cdsp_app_t* app)
{
  app->name = "minimal app";
  app->type = CDSP_PLUGIN_APP_TYPE;

  cdsp_gui_t* gui = (cdsp_gui_t*) calloc(1, sizeof(cdsp_gui_t));
  gui->default_height = 480;
  gui->default_width = 640;
  gui->should_preserve_aspect_ratio = true;
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
