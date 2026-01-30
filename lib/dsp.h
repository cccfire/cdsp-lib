#pragma once

typedef struct cdsp_dsp cdsp_dsp_t;
typedef struct cdsp_dsp {
  void *data;

  void (*start_processing) (cdsp_app_t*);
  void (*stop_processing) (cdsp_app_t*);
} cdsp_dsp_t;

