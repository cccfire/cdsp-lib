#include "app.h"

bool cdsp_destroy_app(cdsp_app_t* app)
{
  if (!app) return false;

  free(app->gui);
  free(app->dsp);
  app->gui = NULL;
  app->dsp = NULL;

  return true;
}
