#include <stdio.h>
#include <assert.h>

#include <pugl/pugl.h>
#include <pugl/gl.h>

#include "app.h"

#include "gui.h"


void cdsp_init_gui(cdsp_app_t* app, cdsp_gui_t* gui)
{
  if (app->type == CDSP_STANDALONE_APP_TYPE) {
    gui->world = puglNewWorld(PUGL_PROGRAM, 0);
  } else if (app->type == CDSP_PLUGIN_APP_TYPE) {
    gui->world = puglNewWorld(PUGL_MODULE, 0);
  } else {
    fprintf(stderr, "error: invalid app type; app type may not be defined. \n");
    fflush(stderr);
    assert ("app type defined" == false);
  }


  puglSetWorldString(gui->world, PUGL_CLASS_NAME, app->name);
  puglSetWorldHandle(gui->world, app);
  gui->init(app);
}
void cdsp_destroy_gui(cdsp_app_t* app, cdsp_gui_t* gui)
{
  gui->destroy(app);
  puglFreeWorld(gui->world);
}

