#include "microshell.h"

void ft_init_list(t_list *cmds)
{
	cmds->len = 0;
	cmds->argv = NULL;
}

char **ft_add_args(char **argv, int start, int end)
{
	char **tmp;
	int i;

	if (!(tmp = (char **)malloc(sizeof(char *) * (end - start + 1))))
		ft_error();
	i = 0;
	while (start < end)
	{
		tmp[i] = ft_strdup(argv[start]);
		start++;
		i++;
	}
	tmp[i] = NULL;
	return (tmp);
}

void ft_parse(t_list *cmds, char **argv, int begin, int fin)
{
	int start;
	int end;
	int i;

	if (begin == fin)
		return;
	cmds->len++;
	start = begin;
	while (start < fin)
	{
		if (strcmp("|", argv[start]) == 0)
			cmds->len++;
		start++;
	}
	if (!(cmds->argv = (t_args *)malloc(sizeof(t_args) * cmds->len)))
		ft_error();
	start = begin;
	end = begin;
	i = 0;
	while (end < fin)
	{
		if (strcmp("|", argv[end]) == 0)
		{
			cmds->argv[i].args = ft_add_args(argv, start, end);
			end++;
			start = end;
			i++;
		}
		else
		{
			end++;
			if (end == fin)
				cmds->argv[i].args = ft_add_args(argv, start, end);
		}
	}

}

void ft_clear(t_list *cmds)
{
	int i = 0;
	int j;

	while (i < cmds->len)
	{
		//ft_putstr_fd("\nARG\n", 1); //tmp
		j = 0;
		while (cmds->argv[i].args[j])
		{
			//ft_putstr_fd(cmds->argv[i].args[j], 1); //tmp
			//ft_putstr_fd(" ", 1); //tmp
			free(cmds->argv[i].args[j]);
			j++;
		}
		free(cmds->argv[i].args);
		i++;
	}
	if (cmds->len > 0)
		free(cmds->argv);
}

int ft_cd(char **last_arg)
{
	if (strcmp("cd", last_arg[0]) != 0)
		return (-1);
	if (!last_arg[1] || last_arg[2])
		ft_putstr_fd("error: cd: bad arguments\n", 2);
	else
	{
		if (!chdir(last_arg[1])) //cd success
			return 0;
		ft_putstr_fd("error: cd: cannot change directory to ", 2);
		ft_putstr_fd(last_arg[1], 2);
		ft_putstr_fd("\n", 2);
	}
	return (1);
}

void ft_execve(char **args, char **envp)
{
	execve(args[0], args, envp);
	ft_putstr_fd("error: cannot execute ", 2);
	ft_putstr_fd(args[0], 2);
	ft_putstr_fd("\n", 2);
	exit(127);
}

int ft_execute(t_list cmds, char **envp)
{
	int child_status = 0;
	int fdstd[2];
	int fdnew[2];
	int fdpipe[2];
	int i;
	pid_t *pid;

	if (cmds.len < 1)
		return (0);
	if ((child_status = ft_cd(cmds.argv[cmds.len - 1].args)) != -1)
		return child_status;
	if (!(pid = (pid_t *)malloc(sizeof(pid_t) * cmds.len)))
		ft_error();
	fdstd[0] = dup(0);
	fdstd[1] = dup(1);
	fdnew[0] = dup(fdstd[0]);
	i = 0;
	while (i < cmds.len)
	{
		ft_dup2(fdnew[0], 0);
		if (i == cmds.len -1)
			fdnew[1] = dup(fdstd[1]);
		else
		{
			if (pipe(fdpipe) == -1)
				ft_error();
			fdnew[0] = fdpipe[0];
			fdnew[1] = fdpipe[1];
		}
		ft_dup2(fdnew[1], 1);
		pid[i] = fork();
		if (pid[i] == 0)
			ft_execve(cmds.argv[i].args, envp);
		i++;
	}
	i = -1;
	while (++i < cmds.len)
		waitpid(pid[i], &child_status, 0);
	ft_dup2(fdstd[0], 0);
	ft_dup2(fdstd[1], 1);
	free(pid);
	return (WEXITSTATUS(child_status));
}



int main(int argc, char *argv[], char **envp)
{
	int ret = 0;
	int begin;
	int fin;
	t_list cmds;

	if (argc < 2)
		return 0;
	ft_init_list(&cmds);
	begin = 1;
	fin = 1;
	while (argv[fin])
	{
		if (strcmp(";", argv[fin]) == 0)
		{
			//ft_putstr_fd("\nNEW", 1); //tmp
			ft_parse(&cmds, argv, begin, fin);
			ret = ft_execute(cmds, envp);
			ft_clear(&cmds);
			ft_init_list(&cmds);
			while (argv[fin] && strcmp(";", argv[fin]) == 0)
				fin++;
			if (!argv[fin])
				break;
			begin = fin;
		}
		else
		{
			fin++;
			if (!argv[fin])
			{
			//	ft_putstr_fd("\nNEW", 1); //tmp
				ft_parse(&cmds, argv, begin, fin);
				ret = ft_execute(cmds, envp);
				ft_clear(&cmds);
			}
		}
	}
//	system("leaks a.out");
	return ret;
}
