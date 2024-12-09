#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/select.h>
#include <signal.h>
#include <string.h>

volatile sig_atomic_t interrupted = 0;

void sigint_handler(int signum) {
    interrupted = 1;
}

int main() {
    // Set up the SIGINT handler
    // struct sigaction sa;
    // sa.sa_handler = sigint_handler;
    // sa.sa_flags = 0; // Do not set SA_RESTART to see EINTR in action
    // sigemptyset(&sa.sa_mask);
    // if (sigaction(SIGINT, &sa, NULL) == -1) {
    //     perror("sigaction");
    //     exit(EXIT_FAILURE);
    // }

    fd_set read_fds;
    int max_fd = STDIN_FILENO; // Monitor standard input
    struct timeval timeout;
    int ret;

    printf("Press Ctrl+C to interrupt.\n");
    printf("Waiting for input (timeout every 10 seconds)...\n");

    while (!interrupted) {
        FD_ZERO(&read_fds);
        FD_SET(STDIN_FILENO, &read_fds);

        // Set timeout to 10 seconds
        timeout.tv_sec = 10;
        timeout.tv_usec = 0;

        ret = select(max_fd + 1, &read_fds, NULL, NULL, &timeout);

        if (ret == -1) {
            if (errno == EINTR) {
                printf("select() was interrupted by a signal.\n");
                // Decide whether to continue or break
                // For this example, we'll continue
                continue;
            } else {
                perror("select");
                exit(EXIT_FAILURE);
            }
        } else if (ret == 0) {
            printf("No input received in the last 10 seconds.\n");
            continue;
        }

        if (FD_ISSET(STDIN_FILENO, &read_fds)) {
            char buffer[1024];
            ssize_t bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer) - 1);
            if (bytes_read < 0) {
                perror("read");
                exit(EXIT_FAILURE);
            }

            buffer[bytes_read] = '\0'; // Null-terminate the string
            printf("You entered: %s", buffer);
        }
    }

    printf("\nProgram interrupted by user. Exiting gracefully.\n");
    return 0;
}
