#pragma once

#include "app.h"

typedef struct cdsp_clap_package {
  cdsp_app_t* app;

  size_t features_length;
  void** features;
} cdsp_clap_package_t;

typedef struct cdsp_clap_feature {
  char* name;
  void* feature;
} cdsp_clap_feature_t;
