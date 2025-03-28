/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssalorin <ssalorin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 12:55:45 by ssalorin          #+#    #+#             */
/*   Updated: 2025/03/27 12:55:47 by ssalorin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @brief Sets an error flag and returns an error status.
 *
 * This function updates the provided error variable to indicate an error 
 * and returns `1` as a general error status.
 *
 * @param err Pointer to an integer that will be set to `1` to signal an error.
 *
 * @return Always returns `1`, indicating an error condition.
 */
static int	error_found(int *err)
{
	*err = 1;
	return (1);
}

/**
 * @brief Checks for overflow during the conversion of a string to a long long 
 *        integer.
 * 
 * This function ensures that adding the next digit to `result` does not exceed 
 * the limits of a `long long` integer. It checks for both positive and negative 
 * overflows.
 * 
 * - If `sign` is positive and `result` would exceed `LLONG_MAX`, `error` is 
 *   set to 1.
 * - If `sign` is negative and `-result` would go below `LLONG_MIN`, `error` 
 *   is set to 1.
 * 
 * @param result The current value of the parsed number before adding the next 
 *               digit.
 * @param digit The next digit to be added to `result`.
 * @param sign The sign of the number (1 for positive, -1 for negative).
 * @param err A pointer to an integer flag that is set to 1 if an overflow 
 *              is detected.
 */
static void	check_overflow(long long result, int digit, int sign, int *err)
{
	if (sign == 1 && (result > (LLONG_MAX - digit) / 10))
		*err = 1;
	if (sign == -1 && (-result < (LLONG_MIN + digit) / 10))
		*err = 1;
}

/**
 * @brief Parses a string to convert it into a long long integer.
 * 
 * This function processes each character of the input string and attempts 
 * to convert it into a `long long` integer. It handles the numeric conversion 
 * and checks for overflow as digits are processed. If an invalid character is 
 * found or an overflow occurs, the function will return an error.
 * 
 * The function uses the provided `sign` to handle negative numbers and sets 
 * the `has_digit` flag to indicate if any valid digits were found. If no valid 
 * digits are found, an error will be reported through the `error` pointer.
 * 
 * @param s The string to parse into a `long long` integer.
 * @param sign The sign of the number (+1 for positive, -1 for negative).
 * @param has_digit A pointer to an integer that will be set to 1 if valid 
 *                  digits are found, or 0 if no digits are found.
 * @param err A pointer to an integer that will be set to 1 if an error 
 *              occurs during the parsing, or 0 if successful.
 * 
 * @return The parsed `long long` integer value if successful, or a predefined 
 *         error value if an error occurs (e.g., invalid character).
 */
static long long	parse_number(char *s, int sign, int *has_digit, int *err)
{
	long long	result;
	int			digit;

	result = 0;
	while (*s)
	{
		if (*s >= '0' && *s <= '9')
		{
			*has_digit = 1;
			digit = *s - '0';
			check_overflow(result, digit, sign, err);
			if (*err == 1)
				return (1);
			result = result * 10 + digit;
		}
		else
			return (error_found(err));
		s++;
	}
	return (sign * result);
}

/**
 * @brief Converts a string to a long long integer.
 * 
 * This function attempts to convert the given string into 
 * a `long long` integer. It handles optional leading whitespace, 
 * optional signs (`+` or `-`), and then parses the numeric characters. 
 * If no valid digits are found or 
 * if the conversion is invalid, an error is flagged.
 * 
 * The function also handles edge cases such as leading spaces, empty strings, 
 * and signs. If no valid digits are found, an error is returned.
 * 
 * @param str The string to convert to a `long long` integer.
 * @param err A pointer to an integer that will be set to `1` if an error 
 *              occurs during the conversion, or `0` if successful.
 * 
 * @return The converted `long long` integer value if successful,
 *          or a predefined error value if an error occurs.
 */
long long	convert_to_ll(char *str, int *err)
{
	long long	nbr;
	int			sign;
	int			has_digit;

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
		return (error_found(err));
	nbr = parse_number(str, sign, &has_digit, err);
	if (!has_digit)
		*err = 1;
	return (nbr);
}
