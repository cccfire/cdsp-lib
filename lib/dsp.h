#pragma once

#include "app.h"

typedef enum cdsp_num_format {
  CDSP_FLOAT, CDSP_DOUBLE
} CDSP_NUM_FORMAT_T;

typedef union cdsp_buffer_data {
  float **data32;
  double **data64;
} cdsp_buffer_data_t;

typedef enum cdsp_side {
  CDSP_LEFT, CDSP_RIGHT  
} CDSP_SIDE_T;

typedef struct cdsp_audio_buffer {
  cdsp_buffer_data_t data;
  CDSP_NUM_FORMAT_T format;
  uint32_t channel_count;
  uint32_t latency; // latency from/to the audio interface
  uint64_t constant_mask; // would like to not keep this but it makes things easier to process for sure
  void* (*access) (
      cdsp_buffer_data_t data, 
      CDSP_NUM_FORMAT_T,
      uint32_t channel, 
      uint32_t frame_index,
      uint64_t constant_mask);
} cdsp_audio_buffer_t;

typedef struct cdsp_process_data {
  int64_t timestamp;
  uint32_t frames_count;
  cdsp_audio_buffer_t input_buffer;
  cdsp_audio_buffer_t output_buffer;
} cdsp_process_data_t; 

typedef struct cdsp_dsp cdsp_dsp_t;
typedef struct cdsp_dsp {
  void *data;

  void (*start_processing) (cdsp_app_t*);
  void (*process) (cdsp_app_t*, cdsp_process_data_t*);
  void (*stop_processing) (cdsp_app_t*);
} cdsp_dsp_t;

// bool returns true on success, false otherwise.
bool cdsp_dsp_process(cdsp_app_t* app, cdsp_process_data_t* data);

/**
 * need to figure out how to deal with clap events
 */
