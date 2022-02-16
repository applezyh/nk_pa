#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"

typedef struct watchpoint {
  int NO;
  struct watchpoint *next;
  uint32_t point_loc;
  bool is_use;
  uint32_t data;

  /* TODO: Add more members if necessary */


} WP;

WP* new_wp();
void free_wp(int no);
bool check_watchpoint();

#endif
