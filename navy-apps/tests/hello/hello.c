#include <unistd.h>
#include <stdio.h>

int main() {
  write(1, "Hello World!\n", 13);
  int i = 2;
  volatile int j = 0;
  while (j<10) {
    j ++;
    printf("Hello World for the %dth time\n", i ++);
  }
  return 0;
}
