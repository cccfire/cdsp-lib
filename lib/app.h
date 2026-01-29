#pragma once


typedef struct cdsp_app cdsp_app_t;
typedef struct cdsp_gui cdsp_gui_t;
typedef struct cdsp_dsp cdsp_dsp_t;

typedef enum cdsp_app_type {
  CDSP_STANDALONE_APP_TYPE, CDSP_PLUGIN_APP_TYPE
} CDSP_APP_TYPE_T;

typedef struct cdsp_app {
  char *name;
  void *ctx;
  cdsp_gui_t *gui;
  cdsp_dsp_t *dsp;
  void *_plugin; // 
                 
  void (*init)(cdsp_app_t*);

  CDSP_APP_TYPE_T type;
} cdsp_app_t;

typedef struct cdsp_dsp cdsp_dsp_t;
typedef struct cdsp_dsp {
  void *data;
} cdsp_dsp_t;

