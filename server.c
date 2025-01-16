/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 11:14:22 by root              #+#    #+#             */
/*   Updated: 2025/01/16 17:41:07 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void handle_sig(int sig, siginfo_t *info, void *context) {
    static char temp = 0;
    static int count_bits = 0;
    (void)context; // Unused parameter

    temp <<= 1;
    if (sig == SIGUSR2)
        temp |= 1;

    count_bits++;
    if (count_bits == 8) {
        if (temp == '\0') {
            write(1, "\n", 1); // Print a newline at the end of the message
        } else {
            write(1, &temp, 1); // Print the received character
        }
        temp = 0;
        count_bits = 0;
    }

    // Send acknowledgment to the client
    kill(info->si_pid, SIGUSR1);
}

int main() {
    struct sigaction sa;

    sa.sa_sigaction = &handle_sig;
    sa.sa_flags = SA_SIGINFO;
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGUSR1, &sa, NULL) == -1 || sigaction(SIGUSR2, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    printf("Server PID: %d\n", getpid());

    while (1) {
        pause(); // Wait for incoming signals
    }

    return 0;
}
