#include <stdio.h>
#include <cdsp/app.h>
#include <cdsp/gui.h>
#include "minimal_plugin.c"

int main(int argc, char* argv[]) {
  printf("1\n");
  cdsp_app_t* app = calloc(1, sizeof(cdsp_app_t));
  printf("2\n");
  create_minimal_app(app);
  cdsp_gui_init(app);
  cdsp_gui_show(app);
  while(app->active) {
    cdsp_gui_update(app);
  }
  cdsp_gui_destroy(app);
  cdsp_destroy_app(app);
  free(app);
  return 1;
}
