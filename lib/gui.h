#pragma once
#include <cairo.h>
#include <pugl/pugl.h>
#include <pugl/gl.h>

#include "app.h"

typedef struct cdsp_gui cdsp_gui_t;
typedef struct cdsp_gui {
  void *data;
  PuglWorld* world;
  PuglView* view;

  GLuint texture;
  cairo_t* cairo_ctx;


  uint16_t aspect_ratio_width;
  uint16_t aspect_ratio_height;
  uint16_t min_width;
  uint16_t min_height;
  uint16_t width;
  uint16_t height;

  void (*setup_opengl) (cdsp_app_t*);
  void (*teardown_opengl) (cdsp_app_t*);

  void (*draw) (cdsp_app_t*);

  void (*init) (cdsp_app_t*);
  void (*destroy) (cdsp_app_t*);

  bool should_preserve_aspect_ratio : 1;
  bool realized : 1;
} cdsp_gui_t;

PuglStatus cdsp_pugl_on_event(PuglView *view, const PuglEvent *event);

void cdsp_gui_init(cdsp_app_t* app);
void cdsp_gui_destroy(cdsp_app_t* app);

bool cdsp_gui_set_scale(cdsp_app_t* app, double scale);
bool cdsp_gui_get_size(cdsp_app_t* app, uint32_t *width, uint32_t *height);
bool cdsp_gui_can_resize(cdsp_app_t* app);
bool cdsp_gui_adjust_size(cdsp_app_t* app, uint32_t *width, uint32_t *height);
bool cdsp_gui_set_size(cdsp_app_t* app, uint32_t width, uint32_t height);
bool cdsp_gui_set_parent(cdsp_app_t* app, const void* window);
bool cdsp_gui_set_transient(cdsp_app_t* app, const void* window);
void cdsp_gui_suggest_title(cdsp_app_t* app, const char* title);
void cdsp_gui_update(cdsp_app_t* app);
bool cdsp_gui_show(cdsp_app_t* app);
bool cdsp_gui_hide(cdsp_app_t* app);
