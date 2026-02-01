#include "app.h"

void cdsp_destroy_app(cdsp_app_t* app)
{
  free(app->gui);
  free(app->dsp);
}
