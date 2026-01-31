#pragma once

#include <clap/clap.h>
#include <clap/ext/gui.h>

#include "app.h"
#include "clap-adapters.h"

cdsp_clap_feature_t** cdsp_clap_generate_features_from_app(cdsp_app_t* app);
void cdsp_clap_free_features(cdsp_clap_package_t* package);

