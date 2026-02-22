#include <stdio.h>
#include <assert.h>

#include <cairo.h>
#include <pugl/pugl.h>
#include <pugl/gl.h>

#include "app.h"

#include "gui.h"

/* Cairo ARGB32 is stored as B,G,R,A in memory on little-endian; GL_BGRA matches. */
#ifndef GL_BGRA
#  define GL_BGRA 0x80E1
#endif
#ifndef GL_CLAMP_TO_EDGE
#  define GL_CLAMP_TO_EDGE 0x812F
#endif

// pugl event handler
PuglStatus cdsp_pugl_on_event(PuglView *view, const PuglEvent *event)
{
  cdsp_app_t* app = (cdsp_app_t*)puglGetHandle(view);
  switch (event->type) {
    case PUGL_REALIZE:
      glEnable(GL_TEXTURE_2D);
      glGenTextures(1, &app->gui->texture);
      glBindTexture(GL_TEXTURE_2D, app->gui->texture);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      app->gui->setup_opengl(app);
      break;
    case PUGL_UNREALIZE:
      glDeleteTextures(1, &app->gui->texture);
      app->gui->teardown_opengl(app);
      break;
    case PUGL_CONFIGURE:
      break;
    case PUGL_EXPOSE:
      app->gui->draw(app);

      cairo_t *cr = app->gui->cairo_ctx;
      cairo_surface_t *surface = cairo_get_target(cr);
      int w = cairo_image_surface_get_width(surface);
      int h = cairo_image_surface_get_height(surface);
      if (w <= 0 || h <= 0) return PUGL_SUCCESS;

      /* --- Upload Cairo surface as GL texture --- */
      cairo_surface_flush(surface);
      unsigned char *data = cairo_image_surface_get_data(surface);

      glBindTexture(GL_TEXTURE_2D, app->gui->texture);
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
      break;
    case PUGL_CLOSE:
      break;
    case PUGL_BUTTON_PRESS:
      break;
    default:
      break;
  }
  return PUGL_SUCCESS;
}

void cdsp_gui_init(cdsp_app_t* app)
{
  if (app->type == CDSP_STANDALONE_APP_TYPE) {
    app->gui->world = puglNewWorld(PUGL_PROGRAM, 0);
  } else if (app->type == CDSP_PLUGIN_APP_TYPE) {
    app->gui->world = puglNewWorld(PUGL_MODULE, 0);
  } else {
    fprintf(stderr, "error: invalid app type %d; app type may not be defined. \n", app->type);
    fflush(stderr);
    assert (app->type);
  }

  puglSetWorldString(app->gui->world, PUGL_CLASS_NAME, app->name);
  puglSetWorldHandle(app->gui->world, app);

  app->gui->view = puglNewView(app->gui->world);
  puglSetViewString(app->gui->view, PUGL_WINDOW_TITLE, "pugl window");
  puglSetSizeHint(app->gui->view, PUGL_DEFAULT_SIZE, app->gui->default_width, app->gui->default_height);
  puglSetSizeHint(app->gui->view, PUGL_MIN_SIZE, app->gui->min_width, app->gui->min_height);
  puglSetSizeHint(app->gui->view, PUGL_MIN_ASPECT, 1, 1);
  puglSetSizeHint(app->gui->view, PUGL_MAX_ASPECT, 16, 9);

  puglSetEventFunc(app->gui->view, &cdsp_pugl_on_event);

  puglSetViewHint(app->gui->view, PUGL_CONTEXT_VERSION_MAJOR, 3);
  puglSetViewHint(app->gui->view, PUGL_CONTEXT_VERSION_MINOR, 3);
  puglSetViewHint(app->gui->view, PUGL_CONTEXT_PROFILE, PUGL_OPENGL_COMPATIBILITY_PROFILE);
  puglSetBackend(app->gui->view, puglGlBackend());

  puglSetHandle(app->gui->view, app);

  cairo_surface_t* cairo_surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, app->gui->default_width, app->gui->default_height);
  app->gui->cairo_ctx = cairo_create(cairo_surface);
  cairo_surface_destroy(cairo_surface);
  
  app->gui->init(app);
}

void cdsp_gui_destroy(cdsp_app_t* app)
{
  app->gui->destroy(app);
  if (app->gui->cairo_ctx)
    cairo_destroy(app->gui->cairo_ctx);
  puglUnrealize(app->gui->view);
  puglFreeWorld(app->gui->world);
}

bool cdsp_gui_set_scale(cdsp_app_t* app, double scale)
{
  return false;
}

bool cdsp_gui_get_size(cdsp_app_t* app, uint32_t *width, uint32_t *height)
{
  PuglArea area = puglGetSizeHint(app->gui->view, PUGL_CURRENT_SIZE);
  *width = (uint32_t)area.width;
  *height = (uint32_t)area.height;
  return true;
}

bool cdsp_gui_adjust_size(cdsp_app_t* app, uint32_t *width, uint32_t *height)
{
  if (!app->gui->should_preserve_aspect_ratio) return true;

  float wratio = (float)*width/(float)app->gui->aspect_ratio_width;
  
  *height = (uint32_t) wratio * app->gui->aspect_ratio_height;

  return true;
}

bool cdsp_gui_set_size(cdsp_app_t* app, uint32_t width, uint32_t height)
{
  PuglStatus status = puglSetSizeHint(app->gui->view, PUGL_CURRENT_SIZE, width, height);
  if (status) {
      fprintf(stderr, "Error setting size (%s)\n", puglStrerror(status));
      fflush(stderr);
  } else {
    if (app->gui->cairo_ctx)
      cairo_destroy(app->gui->cairo_ctx);
    cairo_surface_t* cairo_surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
    app->gui->cairo_ctx = cairo_create(cairo_surface);
    cairo_surface_destroy(cairo_surface);
  }
  return status == PUGL_SUCCESS;
}

bool cdsp_gui_set_parent(cdsp_app_t* app, void *window)
{
  PuglStatus status = puglSetParent(app->gui->view, (PuglNativeView)window);
  if (status) {
      fprintf(stderr, "Error setting parent (%s)\n", puglStrerror(status));
      fflush(stderr);
  }
  return status == PUGL_SUCCESS;
}

bool cdsp_gui_set_transient(cdsp_app_t* app, void *window)
{
  PuglStatus status = puglSetTransientParent(app->gui->view, (PuglNativeView)window);
  if (status) {
      fprintf(stderr, "Error setting transient parent (%s)\n", puglStrerror(status));
      fflush(stderr);
  }
  return status == PUGL_SUCCESS;
}

void cdsp_gui_set_title(cdsp_app_t* app, const char *title)
{
  puglSetViewString(app->gui->view, PUGL_WINDOW_TITLE, "pugl window");
}

void cdsp_gui_update(cdsp_app_t* app)
{
  puglUpdate(app->gui->world, 0.0);
}

bool cdsp_gui_show(cdsp_app_t* app)
{
  PuglStatus status = puglShow(app->gui->view, PUGL_SHOW_RAISE);
  printf("show %d\n", (int) (status));
  return status == PUGL_SUCCESS;
}

bool cdsp_gui_hide(cdsp_app_t* app)
{
  PuglStatus status = puglHide(app->gui->view);
  return status == PUGL_SUCCESS;
}
