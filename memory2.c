#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>

void pause_for_input() {
    printf("Press any key to continue...\n");
    getchar();
}

int main() {
    // Memory map an empty page
    void* mapped_page = mmap(NULL, getpagesize(), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (mapped_page == MAP_FAILED) {
        printf("Failed to mmap the page.\n");
        return 1;
    }

    printf("Memory mapped page address: %p\n", mapped_page);

    // Run the program for a long duration
    while (1) {
        // Pause for user input or sleep for a while
        pause_for_input();
        // sleep(10);

        // Measure memory usage using `ps` command
        system("ps -o pid,vsz,rss,cmd -p $PPID");

        // Access proc file system to see memory image
        FILE* fp = fopen("/proc/self/maps", "r");
        if (fp != NULL) {
            char buffer[1024];
            while (fgets(buffer, sizeof(buffer), fp) != NULL) {
                printf("%s", buffer);
            }
            fclose(fp);
        } else {
            printf("Failed to access /proc/self/maps\n");
        }
    }

    // Unmap the memory page
    munmap(mapped_page, getpagesize());

    return 0;
}

