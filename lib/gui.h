#pragma once
#include <pugl/pugl.h>
#include <pugl/gl.h>

#include "app.h"

typedef struct cdsp_gui cdsp_gui_t;
typedef struct cdsp_gui {
  PuglWorld* world;
  void *data;
  void (*init) (cdsp_app_t*);
  void (*destroy) (cdsp_app_t*);
} cdsp_gui_t;

void cdsp_init_gui(cdsp_app_t* app, cdsp_gui_t* gui);
void cdsp_destroy_gui(cdsp_app_t* app, cdsp_gui_t* gui);
