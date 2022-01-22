#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct s_zone
{
	int width;
	int height;
	char background;
} t_zone;

typedef struct s_shape
{
	char type;
	float x;
	float y;
	float width;
	float height;
	char fill;
} t_shape;

int ft_strlen(char *str)
{
	int i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

int ft_putstr(char *str)
{
	write(1, str, ft_strlen(str));
	return (1);
}

void ft_exit(FILE *fd, char *drawing)
{
	fclose(fd);
	if (drawing)
		free(drawing);
}

int ft_getzone(FILE *fd, t_zone *zone)
{
	int ret;

	ret = fscanf(fd, "%d %d %c\n", &zone->width, &zone->height, &zone->background);
	if (ret != 3)
		return (0);
	if (zone->width <= 0 || zone->width > 300 || zone->height <= 0 || zone->height > 300)
		return (0);
	// printf("%d %d %c\n", zone->width, zone->height, zone->background);
	return (1);
}

char *ft_setbackground(t_zone zone)
{
	char *background;
	int i;

	background = (char *)malloc(sizeof(char) * (zone.width * zone.height));
	if (!background)
		return (NULL);
	i = 0;
	while (i < (zone.width * zone.height))
		background[i++] = zone.background;
	return (background);
}

int ft_inrectangle(t_shape shape, float x, float y) //int->float
{
	float x_min = shape.x;
	float x_max = shape.x + shape.width;
	float y_min = shape.y;
	float y_max = shape.y + shape.height;
	float float_edge = 1.00000000;

	if ((x < x_min || x > x_max) || (y < y_min || y > y_max))
		return (0);
	if ((x - x_min < float_edge || x_max - x < float_edge) || (y - y_min < float_edge || y_max - y < float_edge))
		return (2);
	return (1);
}

void ft_setshapes2(char *background, t_shape shape, t_zone zone)
{
	int i;
	int j;
	int ret;

	i = 0;
	while (i < zone.height)
	{
		j = 0;
		while (j < zone.width)
		{
			ret = ft_inrectangle(shape, j, i);
			if ((shape.type == 'r' && ret == 2) || (shape.type == 'R' && ret))
				background[(i * zone.width + j)] = shape.fill;
			j++;
		}
		i++;
	}
}

int ft_setshapes(FILE *fd, char *background, t_zone zone)
{
	int ret;
	t_shape shape;
	float float_zero = 0.00000000;

	while ((ret = fscanf(fd, "%c %f %f %f %f %c\n", &shape.type, &shape.x, &shape.y, &shape.width, &shape.height, &shape.fill)) == 6)
	{
		if ((shape.type != 'r' && shape.type != 'R') || shape.width <= float_zero || shape.height <= float_zero)
			return (0);
		ft_setshapes2(background, shape, zone);
	}
	if (ret != -1) //need this!
		return (0);
	return 1;
}

void ft_draw(char *image, t_zone zone)
{
	int i;

	i = 0;
	while (i < zone.height)
	{
		write(1, image + (i * zone.width), zone.width);
		write(1, "\n", 1);
		i++;
	}
}
int main(int argc, char *argv[])
{
	t_zone zone;
	char *background;
	FILE *fd;

	zone.width = 0;
	zone.height = 0;
	zone.background = '\0';

	if (argc != 2)
		return (ft_putstr("Error: argument\n"));
	if (!(fd = fopen(argv[1], "r")))
		return (ft_putstr("Error: Operation file corrupted\n"));
	if (!ft_getzone(fd, &zone))
	{
		ft_exit(fd, NULL);
		return (ft_putstr("Error: Operation file corrupted\n"));
	}
	if (!(background = ft_setbackground(zone)))
	{
		ft_exit(fd, NULL);
		return (ft_putstr("Error: ft_setbackground Malloc failed\n"));
	}
	if (!(ft_setshapes(fd, background, zone)))
	{
		ft_exit(fd, background);
		return (ft_putstr("Error: Operation file corrupted\n"));
	}
	ft_draw(background, zone);
	ft_exit(fd, background);
	return 0;
}
