/*
 Bruteforce Bootloader Unlocker for HUAWEI Related smartphones
 This is intended to be the fastest ever created bootloader unlocker
 */

//      trigger build
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

#define LEN 64
#define ATTEMPTS_BEFORE_REBOOT 1

unsigned long long base_start = 1000000000000000;

void save_last_code() {
  FILE *fp = fopen("lastcode", "w");
  if (fp) {
    fprintf(fp, "%llu", base_start);
    fclose(fp);
  }
}

void resumer() {
  printf("\n\nLast used code was: %llu", base_start);
  save_last_code();
  exit(1);
}

int kbhit(void) {
  struct termios oldt, newt;
  int ch;
  int oldf;
  
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
  
  ch = getchar();
  
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  fcntl(STDIN_FILENO, F_SETFL, oldf);
  
  if (ch != EOF) {
    ungetc(ch, stdin);
    return 1;
  }
  
  return 0;
}

int main(int argc, char **argv) {
  if (argc > 1) {
    char *base = argv[1];
    base_start = atoll(base);
  } else {
    FILE *fp = fopen("lastcode", "r");
    if (fp) {
      fscanf(fp, "%llu", &base_start);
      fclose(fp);
    }
  }
  
  signal(SIGINT, resumer);
  signal(SIGTERM, resumer);
  
  char fou[] = "fastboot oem unlock ";
  char TOTAL[LEN];
  int attempts = 0;
  
  while (snprintf(TOTAL, LEN, "%s%llu", fou, base_start++) < LEN && system(TOTAL)) {
    attempts++;
    if (attempts >= ATTEMPTS_BEFORE_REBOOT) {
      system("fastboot reboot bootloader");
      printf("\nPress Enter within 3 seconds to exit...\n");
      sleep(3);
      if (kbhit()) {
        getchar();
        printf("\nProcess terminated by user. Saving last used code...\n");
        save_last_code();
        exit(0);
      }
      attempts = 0;
    }
  }
  
  printf("Your unlock code is: %llu", base_start);
  return 0;
}
