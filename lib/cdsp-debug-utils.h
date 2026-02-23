#pragma once
#include <stdio.h>
#define CDSP_DEBUG_FILE_PATH "C:\\cdsp_debug.log"

static void cdsp_log(const char* msg) {
  FILE* f = fopen(CDSP_DEBUG_FILE_PATH, "a");
  if (!f) return;
  fprintf(f, "%s", msg);
  fflush(f);
  fclose(f);
}
