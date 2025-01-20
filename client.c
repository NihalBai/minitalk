/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbaidaou <nbaidaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 10:20:00 by root              #+#    #+#             */
/*   Updated: 2025/01/19 01:57:24 by nbaidaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

volatile sig_atomic_t	ack_received = 0;

void	sign_handler(int sig)
{
	if (sig == SIGUSR1)
	{
		ack_received = 1;
	}
}

void	error(void)
{
	perror("kill");
	exit(EXIT_FAILURE);
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
			while (!ack_received)
				usleep(100);
			ack_received = 0;
			j--;
		}
		i++;
	}
}

int	ft_atoi(const char *str)
{
	int		i;
	int		sign;
	long	r;

	i = 0;
	sign = 1;
	r = 0;
	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\r' || str[i] == '\v'
		|| str[i] == '\n' || str[i] == '\f')
		i++;
	if (str[i] == '+' || str[i] == '-')
		if (str[i++] == '-')
			sign = -1;
	while (str[i] && str[i] >= '0' && str[i] <= '9')
	{
		r = r * 10 + (str[i++] - '0');
	}
	return (r * sign);
}
int	main(int ac, char **av)
{
	if (ac == 3)
	{
		int pid;
		pid = ft_atoi(av[1]);
		struct sigaction sa;
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