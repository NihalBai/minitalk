/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbaidaou <nbaidaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 10:20:00 by root              #+#    #+#             */
/*   Updated: 2025/01/25 16:09:05 by nbaidaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

volatile sig_atomic_t	g_ack_received = 0;
void					error(void);
int						ft_atoi(const char *str);

void	sign_handler(int sig)
{
	if (sig == SIGUSR1)
	{
		g_ack_received = 1;
	}
}

void	send(int bit, int pid)
{
	if (bit == 0)
	{
		if (kill(pid, SIGUSR1) == -1)
			error();
	}
	else
	{
		if (kill(pid, SIGUSR2) == -1)
			error();
	}
}

void	shift_send(char *s, int pid)
{
	size_t			i;
	int				bit;
	unsigned char	byte;
	int				j;

	i = 0;
	while (s[i])
	{
		byte = (unsigned char)s[i];
		j = 7;
		while (j >= 0)
		{
			bit = (byte >> j) & 1;
			send(bit, pid);
			while (!g_ack_received)
				usleep(10);
			g_ack_received = 0;
			j--;
		}
		i++;
	}
}

int	main(int ac, char **av)
{
	int					pid;
	struct sigaction	sa;

	if (ac == 3)
	{
		pid = ft_atoi(av[1]);
		sa.sa_handler = &sign_handler;
		sigemptyset(&sa.sa_mask);
		sa.sa_flags = 0;
		if (sigaction(SIGUSR1, &sa, NULL) == -1)
		{
			perror("sigaction");
			exit(EXIT_FAILURE);
		}
		shift_send(av[2], pid);
		write(1, "finish\n", 7);
	}
}
