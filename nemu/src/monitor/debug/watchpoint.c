#include "monitor/watchpoint.h"
#include "monitor/expr.h"

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
      } else{
        wp_pool[i].next=head->next;
        head->next=&(wp_pool[i]);
      }
      return &(wp_pool[i]);
    }
  }
  return NULL;
}

void free_wp(WP* wp){
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




