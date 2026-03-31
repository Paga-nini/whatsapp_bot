#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define KEY "kallstrom"
#define STATE_FILE ".dx_state"

void print_banner() {
    printf("\033[1;32m"); // Green
    printf("  _              _   _     __  __  __   __\n");
    printf(" | |            | | | |    \\ \\/ /  \\ \\ / /\n");
    printf(" | |__ ___  __ _| |_| |__   \\  /    \\ V / \n");
    printf(" | '_ ` _ \\/ _` | __| '_ \\  /  \\     > <  \n");
    printf(" | | |  __/ (_| | |_| | | |/_/\\_\\   /_/ \\_\\\n");
    printf(" |_| |_\\___|\\__,_|\\__|_| |_|               V2.0\n");
    printf("\033[0m\n");
}

void progress_bar() {
    printf("[");
    for (int i = 0; i <= 40; i++) {
        printf("=");
        fflush(stdout);
        usleep(75000); // Total ~3 seconds
    }
    printf("] 100%%\n");
}

void generate_otp() {
    char charset[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    printf("\033[1;31mOTP CODE FOUND: ");
    for (int i = 0; i < 6; i++) {
        putchar(charset[rand() % 36]);
    }
    printf("\033[0m\n");
}

int main() {
    if (geteuid() != 0) {
        printf("Error: This tool must be run with root privileges (sudo).\n");
        return 1;
    }

    srand(time(NULL));
    print_banner();

    char input_key[50];
    printf("Enter Activation Key: ");
    scanf("%49s", input_key);

    if (strcmp(input_key, KEY) != 0) {
        printf("Invalid Activation Key. Access Denied.\n");
        return 1;
    }

    printf("Initializing kernel modules...\n");
    progress_bar();

    FILE *f = fopen(STATE_FILE, "r");
    if (f == NULL) {
        // --- STAGE 1: CONFIGURATION ---
        char url[100];
        int threads;
        printf("\nTarget URL: ");
        scanf("%99s", url);
        printf("[*] Resolving %s... Done. Source IP: 192.168.%d.%d\n", url, rand()%255, rand()%255);
        
        printf("Thread Count: ");
        scanf("%d", &threads);

        printf("\nStarting configuration injection...\n");
        for(int i=0; i<50; i++) {
            printf("%02x ", rand()%255);
            if(i%10==0 && i!=0) printf("\n");
            usleep(20000);
        }
        
        printf("\n\nConfiguration complete. Restart deauthX to perform OTP intercept.\n");
        f = fopen(STATE_FILE, "w");
        fprintf(f, "configured");
        fclose(f);
    } else {
        // --- STAGE 2: INTERCEPT ---
        fclose(f);
        printf("\n[*] Resuming session... Target Locked.\n");
        printf("[*] Monitoring packet stream for OTP strings...\n");
        
        for(int i=0; i<100; i++) {
            printf("%02x", rand()%255);
            if(i%20==0) printf("\n");
            usleep(15000);
        }
        
        printf("\n\n");
        generate_otp();
        printf("\033[1;32m[========================================] 100%% Request successfully executed\033[0m\n");
        
        // Self-destruction
        remove(STATE_FILE);
        remove("deauthX"); 
        printf("\n[!] Session closed. Binaries purged.\n");
    }

    return 0;
}
