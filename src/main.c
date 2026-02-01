#include <stdio.h>
#include <cdsp/app.h>
#include "minimal_plugin.c"

int main(int argc, char* argv[]) {
  printf("1\n");
  fflush(stdout);
  cdsp_app_t* app = calloc(1, sizeof(cdsp_app_t));
  printf("2\n");
  fflush(stdout);
  create_minimal_app(app);
  printf("%s\n", app->name);
  fflush(stdout);
  free(app);
  return 1;
}
