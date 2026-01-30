#include <stdio.h>
#include <assert.h>

#include <pugl/pugl.h>
#include <pugl/gl.h>

#include "app.h"

#include "gui.h"


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
  gui->init(app);
}

void cdsp_gui_destroy(cdsp_app_t* app, cdsp_gui_t* gui)
{
  gui->destroy(app);
  puglFreeWorld(gui->world);
}

bool cdsp_gui_set_scale(cdsp_app_t* app, cdsp_gui_t* gui, double scale)
{
  return false;
}

bool cdsp_gui_get_size(cdsp_app_t* app, cdsp_gui_t* gui, uint32_t *width, uint32_t *height)
{
  return false;
}

bool cdsp_gui_can_resize(cdsp_app_t* app, cdsp_gui_t* gui)
{
  return false;
}

bool cdsp_gui_get_resize_hints(cdsp_app_t* app, cdsp_gui_t* gui)
{
  return false;
}

bool cdsp_gui_adjust_size(cdsp_app_t* app, cdsp_gui_t* gui, uint32_t *width, uint32_t *height)
{
  return false;
}

bool cdsp_gui_set_size(cdsp_app_t* app, cdsp_gui_t* gui, uint32_t width, uint32_t height)
{
  return false;
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

