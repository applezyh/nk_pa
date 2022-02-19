#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"

#define BREAKPOINT 1009
#define WATCHPOINT 1010

typedef struct watchpoint {
  int NO;
  struct watchpoint *next;
  char* expr;
  bool is_use;
  uint32_t data;
  int type;

  /* TODO: Add more members if necessary */


} WP;

WP* new_wp();
void free_wp(int no);
bool check_watchpoint(uint32_t eip);

#endif
