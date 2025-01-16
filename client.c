/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 10:20:00 by root              #+#    #+#             */
/*   Updated: 2025/01/16 17:36:51 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

volatile sig_atomic_t ack_received = 0; 

void sign_handler(int sig) {
    if (sig == SIGUSR1) {
        ack_received = 1;  
    }
}

void shift_send(char *s, int pid) {
    int bit;
    size_t i;
    
    i = 0;
    while (i < strlen(s)) 
    {
        int j = 7;
        while (j >= 0) 
        {
            bit = (s[i] >> j) & 1;  
            if (bit == 0)
                kill(pid, SIGUSR1);  
            else
                kill(pid, SIGUSR2);  
            while (!ack_received)
                usleep(100); 
            ack_received = 0; 
            j--;
        }
        i++;
    }
}

int main(int ac ,char ** av)
{
    if(ac == 3)
    {
        int pid;
        pid = atoi(av[1]);
        struct sigaction sa;
        sa.sa_handler = &sign_handler;
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = 0;
        if (sigaction(SIGUSR1, &sa, NULL) == -1) {
            perror("sigaction");
            exit(EXIT_FAILURE);
        }
        shift_send(av[2],pid);
        write(1,"finish\n",7);
    }
}