#ifndef MICROSHELL_H
# define MICROSHELL_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

typedef struct s_args
{
	char **args;
} t_args;

typedef struct s_list
{
	int len;
	t_args *argv;
} t_list;

void ft_putstr_fd(char *str, int fd);
char *ft_strdup(char *str);
void ft_dup2(int old_fd, int new_fd);
void ft_error(void);


#endif
