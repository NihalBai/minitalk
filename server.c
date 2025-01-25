/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbaidaou <nbaidaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 11:14:22 by root              #+#    #+#             */
/*   Updated: 2025/01/25 16:07:22 by nbaidaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void	initialize(char *temp, int *count_bits)
{
	*temp = 0;
	*count_bits = 0;
	write(1, "\n", 1);
}

void	handle_sig(int sig, siginfo_t *info, void *context)
{
	static pid_t	last_pid;
	static char		temp;
	static int		count_bits;

	(void)context;
	if (last_pid != info->si_pid)
		initialize(&temp, &count_bits);
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
	last_pid = info->si_pid;
}

void	ft_putnbr_fd(int n, int fd)
{
	char	c;

	if (fd < 0)
		return ;
	if (n == -2147483648)
		write(fd, "-2147483648", 11);
	else if (n >= 0 && n <= 9)
	{
		c = n + '0';
		write(fd, &c, 1);
	}
	else if (n > 9)
	{
		ft_putnbr_fd(n / 10, fd);
		ft_putnbr_fd(n % 10, fd);
	}
	else if (n < 0)
	{
		write(fd, "-", 1);
		n *= -1;
		ft_putnbr_fd(n, fd);
	}
}

int	main(void)
{
	struct sigaction	sa;
	int					pid;

	sa.sa_sigaction = &handle_sig;
	sa.sa_flags = SA_SIGINFO;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGUSR1, &sa, NULL) == -1
		|| sigaction(SIGUSR2, &sa, NULL) == -1)
	{
		perror("sigaction");
		exit(EXIT_FAILURE);
	}
	pid = getpid();
	write(1, "Server PID: ", 12);
	ft_putnbr_fd(pid, 1);
	while (1)
		pause();
	return (0);
}
