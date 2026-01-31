#include "clap-adapters.h"
#include "clap-features-adapters.h"

cdsp_clap_feature_t** cdsp_clap_generate_features_from_app(cdsp_app_t* app)
{
  return NULL;
}

void cdsp_clap_free_features(cdsp_clap_package_t* package)
{
  cdsp_clap_feature_t** features = package->features;
  for (size_t i = 0; i < package->features_length; i++) {
    free(features[i]);
    features[i] = NULL;
  }
  free(features);
  package->features = NULL;
  package->features_length = 0;
}

