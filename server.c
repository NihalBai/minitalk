/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 11:14:22 by root              #+#    #+#             */
/*   Updated: 2025/01/16 11:35:42 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>


void handle_sig(int sig)
{
    static char temp;
    static int count_bits;
    
    count_bits = 0;
    temp = 0;
    temp <<= 1;
    if (sig == SIGUSR2) 
        temp |= 1;
    count_bits++;
    if (count_bits == 8)
    {
       write(1, &temp, 1); 
       temp = 0;
       count_bits =0;
    }
      kill(getppid(), SIGUSR1);
}
int main()
{
    struct sigaction sa;
    sa.sa_handler = &handle_sig;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    
     if (sigaction(SIGUSR1, &sa, NULL) == -1 || sigaction(SIGUSR2, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    printf("Server PID: %d\n", getpid());
    while (1) {
        pause();
    }
}