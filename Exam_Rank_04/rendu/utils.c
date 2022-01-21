#include "microshell.h"

void ft_putstr_fd(char *str, int fd)
{
	int i = 0;

	while (str[i])
		i++;
	write(fd, str, i);
}

char *ft_strdup(char *str)
{
	char *tmp;
	int i = 0;

	while (str[i])
		i++;
	if (!(tmp = (char *)malloc(sizeof(char) * (i + 1))))
		ft_error();
	i = -1;
	while (str[++i])
		tmp[i] = str[i];
	tmp[i] = '\0';
	return (tmp);
}

void ft_dup2(int old_fd, int new_fd)
{
	if (dup2(old_fd, new_fd) == -1)
		ft_error();
	close(old_fd);
}

void ft_error(void)
{
	ft_putstr_fd("error: fatal\n", 2);
	exit(1);
}
