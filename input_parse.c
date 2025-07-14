/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_parse.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcos <marcos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 23:30:55 by marcos            #+#    #+#             */
/*   Updated: 2025/07/08 23:31:50 by marcos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int	ft_atoi(char *argument)
{
	int	number;
	int	negative;
	int	base;

	number = 0;
	base = 10;
	negative = 1;
	if (*argument == '+' || *argument == '-')
	{
		if (*argument == '-')
			negative = -1;
		argument++;
	}
	while (*argument)
	{
		number = number * base + *argument - '0';
		argument++;
	}
	return (number * negative);
}

static int	is_number(char c)
{
	if (c >= 48 && c <= 57)
		return (0);
	return (1);
}

static int	is_digit(char *argument)
{
	if (*argument == '+' || *argument == '-')
	{
		if (!*(argument + 1) || is_number(*(argument + 1)) == 1)
			return (1);
		argument++;
	}
	while (*argument)
	{
		if (is_number(*argument) == 1)
			return(1);
		argument++;
	}
	return (0);
}

int	parse_to_number(char *argument)
{
	int	number;

	if (is_digit(argument) == 1)
	{
		printf("argument is not numeric\n");
		exit(1);
	}
	number = ft_atoi(argument);
	return (number);
}