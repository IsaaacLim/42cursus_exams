#include <stdio.h>
#include <limits.h>

int	ft_printf(const char *format, ...);

/*
STRING 
width: less than, more than
presc: less than, more than, zero
w + p: w more than, p less than (p more than does nothing)
*/
void	ft_string(void)
{
	int i;
	int j;

	ft_printf("----String----\n");
	ft_printf("This is a %s\n", "string");

	ft_printf("mine - width of 3: %3s\n", "abcdef");
	ft_printf("mine - width of 10: %10s\n", "abcdef");
	printf("moul -  width of 3: %3s\n", "abcdef");
	printf("moul -  width of 10: %10s\n", "abcdef");

	ft_printf("mine - presc of 0: %.0s\n", "abcdef");
	ft_printf("mine - presc of 3: %.3s\n", "abcdef");
	ft_printf("mine - presc of 10: %.10s\n", "abcdef");
	printf("moul - presc of 0: %.0s\n", "abcdef");
	printf("moul - presc of 3: %.3s\n", "abcdef");
	printf("moul - presc of 10: %.10s\n", "abcdef");

	i = ft_printf("mine - w of 10 p of 3: %10.3s\n", "abcdef");
	j = printf("moul - w of 10 p of 3: %10.3s\n", "abcdef");

	printf("Total len: %d vs %d\n", i, j);
}

/*
INT
Nothing: 42, 0, INT MIN, INT MAX (+/- 1)
width: less than, more than (include '-')
presc: zero(does nothing), less than, more than (doesn't include '-')
*/
void	ft_int(void)
{
	int i;
	int j;

	ft_printf("mine: %d\n", 0);
	ft_printf("mine: %d\n", 42);
	ft_printf("mine: %d\n", INT_MIN);
	ft_printf("mine: %d\n", INT_MAX);

	ft_printf("mine - width of 11: %11d\n", 0);
	ft_printf("mine - width of 11: %11d\n", 42);
	ft_printf("mine - width of 11: %11d\n", INT_MIN);
	ft_printf("mine - width of 11: %11d\n", INT_MAX);
	printf("moul - width of 11: %11d\n", 0);
	printf("moul - width of 11: %11d\n", 42);
	printf("moul - width of 11: %11d\n", INT_MIN);
	printf("moul - width of 11: %11d\n", INT_MAX);

	ft_printf("mine - presc of 11: %.11d\n", 0);
	ft_printf("mine - presc of 11: %.11d\n", INT_MIN);
	ft_printf("mine - presc of 11: %.11d\n", INT_MAX);
	printf("moul - presc of 11: %.11d\n", 0);
	printf("moul - presc of 11: %.11d\n", INT_MIN);
	printf("moul - presc of 11: %.11d\n", INT_MAX);

	ft_printf("mine - w of 15 p of 11: %15.11d\n", 0);
	ft_printf("mine - w of 15 p of 0: %15.0d\n", 0);
	ft_printf("mine - w of 15 p of 11: %15.11d\n", INT_MIN);
	i = ft_printf("mine - w of 15 p of 11: %15.11d\n", INT_MAX);
	printf("moul - w of 15 p of 11: %15.11d\n", 0);
	printf("moul - w of 15 p of 0: %15.0d\n", 0);
	printf("moul - w of 15 p of 11: %15.11d\n", INT_MIN);
	j = printf("moul - w of 15 p of 11: %15.11d\n", INT_MAX);

	printf("Total len: %d vs %d\n", i, j);
}

void	ft_hex(void)
{
	ft_printf("mine: %x\n", -1);
	ft_printf("mine: %x\n", 0);
	ft_printf("mine: %x\n", UINT_MAX);
	printf("moul: %x\n", -1);
	printf("moul: %x\n", 0);
	printf("moul: %x\n", UINT_MAX);
}

int	main(void)
{
	// ft_string();
	ft_int();
	// ft_hex();
	// printf("Hexadecimal for %d si %x\n", 42, 42);
	// printf("Magic %s is %5d\n", "number", 42);
	// ft_printf("Hexadecimal for %d si %x\n", 42, 42);
	// ft_printf("Magic %s is %5d\n", "number", 42);
}

/*
Easy:
ft_putchar
ft_putstr
ft_strlen
ft_isdigit

Remember:
ft_atoi - take in const char *str, long total, return as int
ft_itoa
ft_utoa_hex - declare var as unsigned int

ft_output:
string - dup to temp to mod precision; ft_output_blank if precision
Int & hex - ft_output_blank if precision == 0 and num == 0
ft_spaces - left of string; right of int & hex


TEST CASES
-- All -- 
no need right spaces
-- %s --
if !string, string = "(null)"
presc > str_len does nothing
-- %d --
ft_output_blank if have presc, presc == 0 & num = 0.
width > num_len (included neg) && width > presc + neg ***
presc == 0 && < num_len does nothing
presc > (abs)num_len adds zero to left
test all with 0
-- %x -- 
*same with %d*
handle input: -1, UINT_MAX - 1
*/
