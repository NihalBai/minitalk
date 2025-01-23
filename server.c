/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbaidaou <nbaidaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 11:14:22 by root              #+#    #+#             */
/*   Updated: 2025/01/23 19:51:52 by nbaidaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void	handle_sig(int sig, siginfo_t *info, void *context)
{
	static pid_t	last_pid = 0;
	static char		temp = 0;
	static int		count_bits = 0;

	(void)context;
	if (last_pid != info->si_pid)
	{
		last_pid = info->si_pid;
		temp = 0;
		count_bits = 0;
	}
	temp <<= 1;
	if (sig == SIGUSR2)
		temp |= 1;
	count_bits++;
	if (count_bits == 8)
	{
		if (temp == '\0')
			write(1, "\n", 1);
		else
			write(1, &temp, 1);
		temp = 0;
		count_bits = 0;
	}
	kill(info->si_pid, SIGUSR1);
}

int	main(void)
{
	struct sigaction	sa;

	sa.sa_sigaction = &handle_sig;
	sa.sa_flags = SA_SIGINFO;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGUSR1, &sa, NULL) == -1 || sigaction(SIGUSR2, &sa, NULL) ==
		-1)
	{
		perror("sigaction");
		exit(EXIT_FAILURE);
	}
	printf("Server PID: %d\n", getpid());
	while (1)
	{
		pause();
	}
	return (0);
}
