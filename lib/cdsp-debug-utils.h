#pragma once
#include <stdio.h>

static void cdsp_log(const char* msg) {
  FILE* f = fopen("C:\\cdsp_debug.log", "a");
  if (!f) return;
  fprintf(f, "%s", msg);
  fflush(f);
  fclose(f);
}
