#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/prctl.h>
#include <errno.h>
#include <string.h>

int main() {
    printf("🧟 Starting Zombie Factory (PID 1)...\n");
    int count = 1;

    while (1) {
        pid_t pid = fork();

        if (pid < 0) {
            // Fork fails when --pids-limit is hit
            printf("\n❌SYSTEM CRASH: fork() failed at zombie #%d\n", count);
            printf("❌ ERROR: %s (errno %d)\n", strerror(errno), errno);
            printf(" Pausing forever so you can take your screenshots...\n");
            
            // Sleep indefinitely to keep the container alive for inspection
            while(1) { sleep(60); } 
            
        } else if (pid == 0) {
            // Child process: rename itself, then exit to become a zombie
            char name[16];
            snprintf(name, sizeof(name), "zombie_proc_%d", count);
            prctl(PR_SET_NAME, name);
            
            exit(0);
        } else {
            // Parent process: logs the creation and continues without calling wait()
            printf("Spawned %d (PID %d)\n", count, pid);
            count++;
            sleep(1); // 1-second delay for dramatic effect in the terminal
        }
    }
    return 0;
}