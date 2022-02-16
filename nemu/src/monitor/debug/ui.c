#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"
#include "utils/proc_expr.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void cpu_exec(uint64_t);

/* We use the `readline' library to provide more flexibility to read from stdin. */
char* rl_gets() {
  static char *line_read = NULL;

  if (line_read) {
    free(line_read);
    line_read = NULL;
  }

  line_read = readline("(nemu) ");

  if (line_read && *line_read) {
    add_history(line_read);
  }

  return line_read;
}

static int cmd_c(char *args) {
  cpu_exec(-1);
  return 0;
}

static int cmd_q(char *args) {
  return -1;
}

int check_si_args(char* args){
    
    int n=(*args++)-48;
    int result=n;
    if(n<1||n>9) {
      Log("ERROR! bad number!\n");
      return 0;
    } else {
      while(*args!='\0'){
        n=(*args++)-48;
        if(n<0||n>9) return -1;
        result = result*10+n;
      }
    }
    return result;
}

static int cmd_si(char* args){
  if(args==NULL){
      Log("si instruction need a parameter\n");
      return 0;
  }
  int n = check_si_args(args);
  if(n>0) cpu_exec(n);
  else{
    Log("si instruction need a parameter\n");
    return 0;
  }
  return 0;
}

static int cmd_info(char *args) {
  if(args==NULL){
      Log("info instruction need a parameter\n");
      return 0;
    }
  if (strcmp(args, "r")){
      printf("---------REG INFO---------\n");
      printf("(reg: eip value: %x)\n",cpu.eip);
      for(int i=R_EAX;i<R_EDI;i++){
          printf("(reg: %s value: %x)\n",reg_name(i, 4), reg_l(i));
      }
      printf("--------------------------\n");
      for(int i=R_EAX;i<R_EDI;i++){
          printf("(reg: %s value: %x)\n",reg_name(i, 2), reg_w(i));
      }
      printf("--------------------------\n");
      for(int i=R_EAX;i<R_EDI;i++){
          printf("(reg: %s value: %x)\n",reg_name(i, 1), reg_b(i));
      }
      printf("-----------END-----------\n"); 
  } else if(strcmp(args, "w")){
    printf("function not implemented we will release new version to suppot this function\n");
  } else {
    Log("ERROR! bad args!\n");
    return 0;
  }
  return 0;
}

static int cmd_p(char* expr){
  if(expr==NULL){
    printf(" ");
    return 0;
  }
  printf("expr= %d\n", cal_expr(expr));
  return 0;
}

char** split(char* str,int *argv, const char flag){
  char** argc=malloc(10*sizeof(char*));
  int i=0;
  argc[i]=malloc(100*sizeof(char));
  memset(argc[i],0,100);
  while(*str!='\0'&&i<10){
    int k=0;
    while(*str!=flag&&*str!='\0'){
      argc[i][k++]=*str++;
    }
    i++;
    if(*str==flag) {str++;argc[i]=malloc(100*sizeof(char));memset(argc[i],0,100);}
  }
  *argv=i;
  return argc;
}

static int cmd_x(char* args){
  int argv=0;
  char** argc = split(args, &argv, ' ');
  if(2!=argv){
    Log("ERROR! bad args!\n");
    return 0;
  } else {
    int n = check_si_args(argc[0]);
    if(n<0){
      Log("ERROR! bad number!\n");
      return 0;
    } else{
      int mem_loc=cal_expr(argc[1]);
      printf("--------MEM--------\n");
      for(int i=mem_loc;i<mem_loc+4*n;i+=4){
        printf("%x   :   %x\n",i,*((int*)(pmem+i)));
      }
    }
  }
  return 0;
}

static int cmd_help(char *args);

static struct {
  char *name;
  char *description;
  int (*handler) (char *);
} cmd_table [] = {
  { "help", "Display informations about all supported commands", cmd_help },
  { "c", "Continue the execution of the program", cmd_c },
  { "q", "Exit NEMU", cmd_q },
  { "info", "`r` print register infomation on screen\n`w` function under development", cmd_info},
  { "si", "single step debugging use si N to run N step instruction", cmd_si},
  { "p", "calculate expression", cmd_p},
  { "x", "scan mem", cmd_x}
  /* TODO: Add more commands */

};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args) {
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  int i;

  if (arg == NULL) {
    /* no argument given */
    for (i = 0; i < NR_CMD; i ++) {
      printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
    }
  }
  else {
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(arg, cmd_table[i].name) == 0) {
        printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        return 0;
      }
    }
    printf("Unknown command '%s'\n", arg);
  }
  return 0;
}

void ui_mainloop(int is_batch_mode) {
  if (is_batch_mode) {
    cmd_c(NULL);
    return;
  }

  while (1) {
    char *str = rl_gets();
    char *str_end = str + strlen(str);

    /* extract the first token as the command */
    char *cmd = strtok(str, " ");
    if (cmd == NULL) { continue; }

    /* treat the remaining string as the arguments,
     * which may need further parsing
     */
    char *args = cmd + strlen(cmd) + 1;
    if (args >= str_end) {
      args = NULL;
    }

#ifdef HAS_IOE
    extern void sdl_clear_event_queue(void);
    sdl_clear_event_queue();
#endif

    int i;
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(cmd, cmd_table[i].name) == 0) {
        if (cmd_table[i].handler(args) < 0) { return; }
        break;
      }
    }

    if (i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
  }
}
