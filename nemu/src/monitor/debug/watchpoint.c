#include "monitor/watchpoint.h"
#include "utils/proc_expr.h"

#define NR_WP 32

static WP wp_pool[NR_WP];
static WP *head, *free_;

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = &wp_pool[i + 1];
  }
  wp_pool[NR_WP - 1].next = NULL;

  head = NULL;
  free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */

WP* new_wp(){
  for(int i=0;i<NR_WP;i++){
    if(!wp_pool[i].is_use){
      wp_pool[i].is_use=1;
      if(head==NULL){
        head=&(wp_pool[i]);
        head->next=NULL;
      } else{
        wp_pool[i].next=head->next;
        head->next=&(wp_pool[i]);
      }
      return &(wp_pool[i]);
    }
  }
  assert(0);
  return NULL;
}

void free_wp(int no){
  struct watchpoint* wp=&(wp_pool[no]);
  wp->is_use=0;
  struct watchpoint* h=head,* pre_h=NULL;
  while(h!=NULL){
    if(h==wp){
      if(pre_h==NULL){
        head=h->next;
      } else{
        pre_h->next=h->next;
      }
    }
    pre_h=h;
    h=h->next;
  }
}

bool check_watchpoint(){
  struct watchpoint* h=head;
  while(h!=NULL){
    if(h->data!=cal_expr(h->expr)){
      printf("watchpoint NO:%d trigger\n", h->NO);
      return true;
    }
  }
  return false;
}




