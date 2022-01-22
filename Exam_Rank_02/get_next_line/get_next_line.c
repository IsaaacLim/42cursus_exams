#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int		get_next_line(char **line)
{
	char	buff[1];
	int 	ret;
	int		a;

	*line = malloc(sizeof(char *) * 1000000);
	a = 0;
	while (1)
	{
		ret = read(0, buff, 1);
		if (ret <= 0 || *buff == '\n')
			break;
		(*line)[a++] = *buff;
	}
	(*line)[a] = '\0';
	return (ret);
}

int		main()
{
	char *line;

	while (get_next_line(&line) == 1)
	{
		printf("%s\n", line);
		free(line);
	}
}

