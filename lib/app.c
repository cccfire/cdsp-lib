#include <assert.h>

#include "app.h"
#include "gui.h"

void cdsp_destroy_app(cdsp_app_t* app)
{
  free(app->gui);
  free(app->dsp);
}

void cdsp_update(cdsp_app_t* app)
{
  assert(app->gui);
  app->gui->update(app);
}
