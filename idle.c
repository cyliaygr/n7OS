#include <stdio.h>
#include <n7OS/cpu.h>

void idle() {
  printf("Hello, world from IDLE\n");
  scheduler();
  while (1) {
    hlt();
  }
}