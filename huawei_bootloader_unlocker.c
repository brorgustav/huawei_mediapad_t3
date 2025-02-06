/*
    Bruteforce Bootloader Unlocker for HUAWEI Related smartphones
    This is intended to be the fastest ever created bootloader unlocker
    It will reboot after 4 tries because of the code limit
*/
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

#define LEN 64
#define ATTEMPTS_BEFORE_REBOOT 4

unsigned long long base_start = 1000000000000000;

void resumer() {
    printf("\n\nLast used code was: %llu", base_start);
    FILE *fp = fopen("lastcode", "w");
    if (fp) {
        fprintf(fp, "%llu", base_start);
        fclose(fp);
    }
    exit(1);
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
            attempts = 0;
        }
    }

    printf("Your unlock code is: %llu", base_start);
    return 0;
}
