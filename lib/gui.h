#pragma once
#include <cairo.h>
#include <pugl/pugl.h>
#include <pugl/gl.h>

#include "app.h"

typedef struct cdsp_gui cdsp_gui_t;
typedef struct cdsp_gui {
  PuglWorld* world;
  PuglView* view;

  cairo_t* cairo_ctx;

  uint16_t aspect_ratio_width;
  uint16_t aspect_ratio_height;
  uint16_t min_width;
  uint16_t min_height;
  uint16_t default_width;
  uint16_t default_height;
  void *data;

  void (*init) (cdsp_app_t*);
  void (*destroy) (cdsp_app_t*);

  bool should_preserve_aspect_ratio;
} cdsp_gui_t;

void cdsp_gui_init(cdsp_app_t* app, cdsp_gui_t* gui);
void cdsp_gui_destroy(cdsp_app_t* app, cdsp_gui_t* gui);

bool cdsp_gui_set_scale(cdsp_app_t* app, cdsp_gui_t* gui, double scale);
bool cdsp_gui_get_size(cdsp_app_t* app, cdsp_gui_t* gui, uint32_t *width, uint32_t *height);
bool cdsp_gui_can_resize(cdsp_app_t* app, cdsp_gui_t* gui);
bool cdsp_gui_adjust_size(cdsp_app_t* app, cdsp_gui_t* gui, uint32_t *width, uint32_t *height);
bool cdsp_gui_set_size(cdsp_app_t* app, cdsp_gui_t* gui, uint32_t width, uint32_t height);
bool cdsp_gui_set_parent(cdsp_app_t* app, cdsp_gui_t* gui, const void* window);
bool cdsp_gui_set_transient(cdsp_app_t* app, cdsp_gui_t* gui, const void* window);
void cdsp_gui_suggest_title(cdsp_app_t* app, cdsp_gui_t* gui, const char* title);
bool cdsp_gui_show(cdsp_app_t* app, cdsp_gui_t* gui);
bool cdsp_gui_hide(cdsp_app_t* app, cdsp_gui_t* gui);
