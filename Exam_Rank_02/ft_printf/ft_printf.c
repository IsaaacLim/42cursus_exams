#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <limits.h>
#include <stdio.h>

typedef struct s_print
{
	va_list	args;
	const char	*format;
	int		argument_length;
	int		total_length;
	int		precision;
	int		width;
	bool	has_precision;
} t_print;

// Libft -----------------------------------------------------------------------
int		ft_putchar(char c)
{
	return (write(1, &c, 1));
}

int		ft_putstr(char *str)
{
	int	total;

	total = 0;
	while (*str)
	{
		total += ft_putchar(*str);
		str++;
	}
	
	return (total);
}

int		ft_strlen(char *str)
{
	int i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

bool	ft_isdigit(char c)
{
	if (c >= '0' && c <= '9')
		return (true);
	return (false);
}

int		ft_atoi(const char *str)
{
	int i;
	long	tot;
	int	flag;

	i = 0;
	while((str[i] >= 9 && str[i] <= 13) || str[i] == ' ')
		i++;
	flag = 1;
	if (str[i] == '-')
	{
		flag *= -1;
		i++;
	}
	else if (str[i] == '+')
		i++;
	tot = 0;
	while (str[i] >= '0' && str[i] <= '9')
	{
		tot = tot * 10 + (str[i] - '0');
		i++;
	}
	return (tot * (long)flag);
}

char	*ft_itoa(int n)
{
	int		len;
	int		temp;
	char	*num;

	len = 0;
	temp = n;
	if (temp < 0)
	{
		len++;
		temp *= -1;
	}
	if (temp == 0)
		len++;
	while (temp)
	{
		temp /= 10;
		len++;
	}
	num = malloc(sizeof(char) * (len + 1));
	num[len--] = '\0';
	if (n == INT_MIN)
	{
		num[len--] = '8';
		n /= 10;
	}
	if (n < 0)
	{
		num[0] = '-';
		n *= -1;
	}
	if (n == 0)
		num[0] = '0';
	while (n)
	{
		num[len--] = n % 10 + '0';
		n /= 10;
	}
	return (num);
}

char	*ft_utoa_hex(unsigned int n)
{
	int		len;
	unsigned int		temp;
	char	*hex_num;

	len = 0;
	temp = n;
	if (temp == 0)
		len++;
	while (temp)
	{
		temp /= 16;
		len++;
	}
	hex_num = malloc(sizeof(char) * (len + 1));
	hex_num[len] = '\0'; 
	while(--len >= 0)
	{
		hex_num[len] = "0123456789abcdef"[n % 16];
		n /= 16;
	}
	return (hex_num);
}

// Functions -------------------------------------------------------------------
void	ft_output_string(t_print *info)
{
	char	*string;
	char	*temp;
	int		len;
	int		i;

	string = va_arg(info->args, char *);	
	if (!string)
		string = "(null)";
	len = ft_strlen(string);
	temp = (char *)malloc(sizeof(char) * (len + 1));
	i = 0;
	while (string[i])
	{
		temp[i] = string[i];
		i++;
	}
	temp[i] = '\0';
	if (info->has_precision)
	{
		if (info->precision == 0)
		{
			while (--len >= 0)
				temp[len] = '\0';
		}
		else if (info->precision < len)
		{
			while (--len >= info->precision)
				temp[len] = '\0';
		}
	}
	info->argument_length = ft_strlen(temp);
	while (info->width > 0 && info->width-- > info->argument_length)
		info->total_length += ft_putchar(' ');
	info->total_length += ft_putstr(temp);
	free(temp);
}

void	ft_output_blank(t_print *info)
{
	while (info->width-- > 0)
		info->total_length += ft_putchar(' ');
}

void	ft_mod_left_digit(t_print *info, char *itoa, int neg)
{
	int	num_len;

	num_len = ft_strlen(itoa);
	while (info->width > num_len && info->width-- > (info->precision + neg))
		info->argument_length += ft_putchar(' ');
	if (neg)
		info->argument_length += ft_putchar('-');
	if (info->has_precision)
	{
		while (info->precision-- > (int)ft_strlen(&itoa[neg]))
			info->argument_length += ft_putchar('0');
	}
}

void	ft_output_int(t_print *info)
{
	int		neg;
	char	*itoa;
	int		number;

	number = va_arg(info->args, int);
	if (info->has_precision && info->precision == 0 && number == 0)
		ft_output_blank(info);
	else
	{
		itoa = ft_itoa(number);
		neg = 0;
		if (number < 0)
			neg = 1;
		ft_mod_left_digit(info, itoa, neg);
		info->argument_length += ft_putstr(&itoa[neg]);
		info->total_length += info->argument_length;
		free(itoa);
	}
}

void	ft_output_hex(t_print *info)
{
	char	*utoa_hex;
	unsigned int		number;

	number = va_arg(info->args, unsigned int);
	if (info->has_precision && info->precision == 0 && number == 0)
		ft_output_blank(info);
	else
	{
		utoa_hex = ft_utoa_hex(number);
		ft_mod_left_digit(info, utoa_hex, 0);
		info->argument_length += ft_putstr(utoa_hex);
		info->total_length += info->argument_length;
		free(utoa_hex);
	}
}

void	ft_eval_digit(t_print *info) //gets number and format++
{
	char *itoa;

	if(!info->has_precision)
	{
		info->width = ft_atoi(info->format);
		itoa = ft_itoa(info->width);
	}
	else
	{
		info->precision = ft_atoi(info->format);
		itoa = ft_itoa(info->precision);
	}
	info->format += ft_strlen(itoa);
	free(itoa);
}

void	ft_format_precision(t_print *info)
{
	info->has_precision = true;
	info->format++;
	while(*info->format == '0' && ft_isdigit(*(info->format + 1)))
		info->format++;
	if (ft_isdigit(*info->format))
		ft_eval_digit(info);
}

void	ft_format_flag(t_print *info)
{
	info->format++;
	if (ft_isdigit(*info->format))
		ft_eval_digit(info);
	if (*info->format == '.')
		ft_format_precision(info);
	if (*info->format == 's')
		ft_output_string(info);
	else if (*info->format == 'd')
		ft_output_int(info);
	else if (*info->format == 'x')
		ft_output_hex(info);
	else
		printf("none\n");
	info->format++;
}

void	ft_reset_arg_info(t_print *info)
{
	info->argument_length = 0;
	info->precision = 0;
	info->width = 0;
	info->has_precision = false;
}

//didn't use ft_init_arg_info
int	ft_printf(const char *format, ...)
{
	t_print *info;
	int		len;

	info = (t_print *)malloc(sizeof(t_print));
	if (!info)
		return (-1);
	va_start(info->args, format);
	info->format = format;
	info->total_length = 0;
	while (*info->format)
	{
		while (*info->format != '%' && *info->format != '\0')
		{
			info->total_length += ft_putchar(*info->format);
			info->format++;
		}
		if (*info->format == '%')
			ft_format_flag(info);
		ft_reset_arg_info(info);
	}
	va_end(info->args);
	len = info->total_length;
	free(info);
	return (len);
}
