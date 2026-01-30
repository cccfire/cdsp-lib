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

void cdsp_gui_init(cdsp_app_t* app, cdsp_gui_t* gui)
{
  if (app->type == CDSP_STANDALONE_APP_TYPE) {
    gui->world = puglNewWorld(PUGL_PROGRAM, 0);
  } else if (app->type == CDSP_PLUGIN_APP_TYPE) {
    gui->world = puglNewWorld(PUGL_MODULE, 0);
  } else {
    fprintf(stderr, "error: invalid app type %d; app type may not be defined. \n", app->type);
    fflush(stderr);
    assert (app->type);
  }

  puglSetWorldString(gui->world, PUGL_CLASS_NAME, app->name);
  puglSetWorldHandle(gui->world, app);

  gui->view = puglNewView(gui->world);
  puglSetViewString(gui->view, PUGL_WINDOW_TITLE, "pugl window");
  puglSetSizeHint(gui->view, PUGL_DEFAULT_SIZE, gui->default_width, gui->default_height);
  puglSetSizeHint(gui->view, PUGL_MIN_SIZE, gui->min_width, gui->min_height);
  puglSetSizeHint(gui->view, PUGL_MIN_ASPECT, 1, 1);
  puglSetSizeHint(gui->view, PUGL_MAX_ASPECT, 16, 9);

  PuglArea area = puglGetSizeHint(gui->view, PUGL_CURRENT_SIZE);

  cairo_surface_t* cairo_surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, area.width, area.height);
  gui->cairo_ctx = cairo_create(cairo_surface);
  cairo_surface_destroy(cairo_surface);
  
  gui->init(app);
}

void cdsp_gui_destroy(cdsp_app_t* app, cdsp_gui_t* gui)
{
  gui->destroy(app);
  if (gui->cairo_ctx)
    cairo_destroy(gui->cairo_ctx);
  puglFreeWorld(gui->world);
}

bool cdsp_gui_set_scale(cdsp_app_t* app, cdsp_gui_t* gui, double scale)
{
  return false;
}

bool cdsp_gui_get_size(cdsp_app_t* app, cdsp_gui_t* gui, uint32_t *width, uint32_t *height)
{
  PuglArea area = puglGetSizeHint(gui->view, PUGL_CURRENT_SIZE);
  *width = (uint32_t)area.width;
  *height = (uint32_t)area.height;
  return true;
}

bool cdsp_gui_adjust_size(cdsp_app_t* app, cdsp_gui_t* gui, uint32_t *width, uint32_t *height)
{
  if (!gui->should_preserve_aspect_ratio) return true;

  float wratio = (float)*width/(float)gui->aspect_ratio_width;
  
  *height = (uint32_t) wratio * gui->aspect_ratio_height;

  return true;
}

bool cdsp_gui_set_size(cdsp_app_t* app, cdsp_gui_t* gui, uint32_t width, uint32_t height)
{
  PuglStatus status = puglSetSizeHint(gui->view, PUGL_DEFAULT_SIZE, width, height);
  return status == PUGL_SUCCESS;
}

bool cdsp_gui_set_parent(cdsp_app_t* app, cdsp_gui_t* gui, void *window)
{
  return false;
}

bool cdsp_gui_set_transient(cdsp_app_t* app, cdsp_gui_t* gui, void *window)
{
  return false;
}

void cdsp_gui_suggest_title(cdsp_app_t* app, cdsp_gui_t* gui, const char *title)
{
}

bool cdsp_gui_show(cdsp_app_t* app, cdsp_gui_t* gui)
{
  return false;
}

bool cdsp_gui_hide(cdsp_app_t* app, cdsp_gui_t* gui)
{
  return false;
}

