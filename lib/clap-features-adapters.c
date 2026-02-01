#include "clap-adapters.h"
#include "clap-features-adapters.h"
#include "clap-gui-adapters.h"

cdsp_clap_feature_t* cdsp_clap_generate_features_from_app(cdsp_app_t* app, size_t* out_length)
{
  clap_plugin_gui_t* gui = (clap_plugin_gui_t*)calloc(1, sizeof(clap_plugin_gui_t));
  cdsp_init_clap_gui(gui);

  cdsp_clap_feature_t gui_feature = {
    .name = CLAP_EXT_GUI,
    .feature = gui
  };

  int features_count = 1;
  cdsp_clap_feature_t* features = (cdsp_clap_feature_t*)calloc(features_count, sizeof(cdsp_clap_feature_t));
  features[0] = gui_feature;
  *out_length = features_count;
  return features;
}

void cdsp_clap_free_features(cdsp_clap_package_t* package)
{
  free(package->features);
  package->features = NULL;
  package->features_length = 0;
}

