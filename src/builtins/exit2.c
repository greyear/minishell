#include "seela.h"

static int	error_found(int *error)
{
	*error = 1;
	return (1);
}

static long long	check_overflow(long long result, int digit, int sign, int *error)
{
	if (sign == 1 && (result > (LLONG_MAX - digit) / 10))
		*error = 1;
	if (sign == -1 && (-result < (LLONG_MIN + digit) / 10))
		*error = 1;
	return (0);
}

static long long	parse_number(char *str, int sign, int *has_digit, int *error)
{
	long long	result;
	int			digit;

	result = 0;
	while (*str)
	{
		if (*str >= '0' && *str <= '9')
		{
			*has_digit = 1;
			digit = *str - '0';
			check_overflow(result, digit, sign, error);
			if (*error == 1)
				return (1);
			result = result * 10 + digit;
		}
		else
			return (error_found(error));
		str++;
	}
	return (sign * result);
}

long long	ft_strtoll(char *str, int *error)
{
	int		sign;
	long long	nbr;
	int		has_digit;

	has_digit = 0;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	sign = 1;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	if (*str == '\0')
		return (error_found(error));
	nbr = parse_number(str, sign, &has_digit, error);
	if (!has_digit)
		*error = 1;
	return (nbr);
}
