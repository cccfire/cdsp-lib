#include "clap-adapters.h"
#include "clap-extensions-adapters.h"
#include "clap-gui-adapters.h"

cdsp_clap_extension_t* cdsp_clap_generate_extensions_from_app(cdsp_app_t* app, size_t* out_length)
{
  clap_plugin_gui_t* gui = (clap_plugin_gui_t*)calloc(1, sizeof(clap_plugin_gui_t));
  cdsp_init_clap_gui(gui);

  cdsp_clap_extension_t gui_extension = {
    .name = CLAP_EXT_GUI,
    .extension = gui
  };

  int extensions_count = 1;
  cdsp_clap_extension_t* extensions = (cdsp_clap_extension_t*)calloc(extensions_count, sizeof(cdsp_clap_extension_t));
  extensions[0] = gui_extension;
  *out_length = extensions_count;
  return extensions;
}

void cdsp_clap_free_extensions(cdsp_clap_package_t* package)
{
  free(package->extensions);
  package->extensions = NULL;
  package->extensions_length = 0;
}

