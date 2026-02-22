#pragma once

#include <clap/clap.h>
#include <clap/ext/gui.h>

#include "app.h"
#include "clap-adapters.h"

cdsp_clap_extension_t* cdsp_clap_generate_extensions_from_app(cdsp_app_t* app, size_t* out_length);
void cdsp_clap_free_extensions(cdsp_clap_package_t* package);

