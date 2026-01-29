#pragma once
#include <pugl/pugl.h>
#include <pugl/gl.h>

typedef struct cdsp_app cdsp_app_t;
typedef struct cdsp_app {
  void *ctx,
  void *_plugin, // 
  void (*init)(cdsp_app_t*),
} cdsp_app_t;

typedef struct cdsp_gui cdsp_gui_t;
typedef struct cdsp_gui {
  PuglWorld* world,
  void *data
} cdsp_gui_t;

typedef struct cdsp_dsp cdsp_dsp_t;
typedef struct cdsp_dsp {
  void *data
} cdsp_dsp_t;

