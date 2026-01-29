#include <clap/clap.h>
#include <clap/ext/gui.h>
#include <string.h>

bool cdsp_gui_is_api_supported(const clap_plugin_t *plugin, const char *api, bool is_floating) 
{
  if (is_floating)
    return false;
#if IS_MAC
  if (strcmp(api, CLAP_WINDOW_API_COCOA) == 0)
    return true;
#endif

#if IS_WIN
  if (strcmp(api, CLAP_WINDOW_API_WIN32) == 0)
    return true;
#endif

#if IS_LINUX
  if (strcmp(api, CLAP_WINDOW_API_X11) == 0)
    return true;
#endif

  return false;
}

bool cdsp_gui_get_preferred_api(const clap_plugin_t *plugin, const char **api, bool *is_floating) 
{
  *is_floating = false;
#if IS_MAC
  api = &CLAP_WINDOW_API_COCOA;
  return true;
#endif

#if IS_WIN
  api = &CLAP_WINDOW_API_WIN32;
  return true;
#endif

#if IS_LINUX
  api = &CLAP_WINDOW_API_X11;
  return true;
#endif

  return false;
}
