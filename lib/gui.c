#include <stdio.h>
#include <assert.h>

#include <cairo.h>
#include <pugl/pugl.h>
#include <pugl/gl.h>

#include "app.h"

#include "gui.h"

// pugl event handler
PuglStatus cdsp_gui_event_handler(PuglView *view, const PuglEvent *event)
{
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

  // Set up opengl backend
  puglSetBackend(app->gui->view, puglGlBackend());
  puglSetViewHint(app->gui->view, PUGL_CONTEXT_VERSION_MAJOR, 3);
  puglSetViewHint(app->gui->view, PUGL_CONTEXT_VERSION_MINOR, 3);
  puglSetViewHint(app->gui->view,
      PUGL_CONTEXT_PROFILE,
      PUGL_OPENGL_COMPATIBILITY_PROFILE);

  PuglArea area = puglGetSizeHint(app->gui->view, PUGL_CURRENT_SIZE);

  cairo_surface_t* cairo_surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, area.width, area.height);
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

void cdsp_gui_update(cdsp_app_t* app)
{
  puglUpdate(app->gui->world, (double)0.0);
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
  PuglStatus status = puglSetSizeHint(app->gui->view, PUGL_DEFAULT_SIZE, width, height);
  return status == PUGL_SUCCESS;
}

bool cdsp_gui_set_parent(cdsp_app_t* app, void *window)
{
  PuglStatus status = puglSetParent(app->gui->view, (PuglNativeView)window);
  return status == PUGL_SUCCESS;
}

bool cdsp_gui_set_transient(cdsp_app_t* app, void *window)
{
  PuglStatus status = puglSetTransientParent(app->gui->view, (PuglNativeView)window);
  return status == PUGL_SUCCESS;
}

void cdsp_gui_set_title(cdsp_app_t* app, const char *title)
{
  puglSetViewString(app->gui->view, PUGL_WINDOW_TITLE, "pugl window");
}

bool cdsp_gui_show(cdsp_app_t* app)
{
  PuglStatus status = puglShow(app->gui->view, PUGL_SHOW_RAISE);
  return status == PUGL_SUCCESS;
}

bool cdsp_gui_hide(cdsp_app_t* app)
{
  PuglStatus status = puglHide(app->gui->view);
  return status == PUGL_SUCCESS;
}

