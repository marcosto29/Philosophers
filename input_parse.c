/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_parse.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matoledo <matoledo@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 23:30:55 by marcos            #+#    #+#             */
/*   Updated: 2025/08/15 18:37:12 by matoledo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int	ft_atoi(char *argument)
{
	int	number;
	int	base;

	number = 0;
	base = 10;
	if (*argument == '+')
		argument++;
	while (*argument)
	{
		number = number * base + *argument - '0';
		argument++;
	}
	return (number);
}

static int	is_number(char c)
{
	if (c >= 48 && c <= 57)
		return (0);
	return (1);
}

static int	is_positive_digit(char *argument)
{
	if (*argument == '+')
	{
		if (!*(argument + 1) || is_number(*(argument + 1)) == 1)
			return (1);
		argument++;
	}
	while (*argument)
	{
		if (is_number(*argument) == 1)
			return (1);
		argument++;
	}
	return (0);
}

int	parse_to_number(char *argument)
{
	int	number;

	if (is_positive_digit(argument) == 1)
	{
		printf("argument is not valid\n");
		return (-1);
	}
	number = ft_atoi(argument);
	return (number);
}

int	parse_input(int *arguments, int argc, char **argv)
{
	int	counter;

	if (argc < 5 || argc > 6)
	{
		printf("incorrect number of arguments\n");
		exit(1);
	}
	counter = 0;
	while (counter < argc - 1)
	{
		arguments[counter] = parse_to_number(argv[counter + 1]);
		if (arguments[counter] <= 0)
		{
			free(arguments);
			return (1);
		}
		counter++;
	}
	return (0);
}
