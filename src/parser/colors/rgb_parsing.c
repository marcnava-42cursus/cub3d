/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rgb_parsing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 01:55:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/11/03 19:07:06 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "structs.h"

static int	convert_token_to_int(char *token)
{
	long	value;
	int		sign;

	while (*token == ' ' || *token == '\t')
		token++;
	if (*token == '\0')
		return (-1);
	sign = 1;
	if (*token == '+' || *token == '-')
	{
		if (*token == '-')
			sign = -1;
		token++;
	}
	if (!ft_isdigit(*token))
		return (-1);
	value = 0;
	while (ft_isdigit(*token))
	{
		value = (value * 10) + (*token - '0');
		if (value > 2147483647L)
			return (-1);
		token++;
	}
	while (*token == ' ' || *token == '\t')
		token++;
	if (*token != '\0')
		return (-1);
	return ((int)(value * sign));
}

static int	extract_rgb_tokens(char *str_copy, int *values)
{
	char	*token;
	char	*comma;
	int		count;
	int		value;

	count = 0;
	token = str_copy;
	while (token)
	{
		if (count >= 3)
			return (0);
		comma = ft_strchr(token, ',');
		if (comma)
			*comma = '\0';
		value = convert_token_to_int(token);
		if (value == -1)
			return (0);
		values[count] = value;
		count++;
		if (!comma)
			token = NULL;
		else
			token = comma + 1;
	}
	return (count);
}

int	parse_rgb_values(const char *rgb_str, int *rgb)
{
	char	*str_copy;
	int		values[3];
	int		count;

	str_copy = malloc(ft_strlen(rgb_str) + 1);
	if (!str_copy)
		return (0);
	ft_strlcpy(str_copy, rgb_str, ft_strlen(rgb_str) + 1);
	count = extract_rgb_tokens(str_copy, values);
	free(str_copy);
	if (count != 3)
		return (0);
	if (!validate_rgb_values(values[0], values[1], values[2]))
		return (0);
	rgb[0] = values[0];
	rgb[1] = values[1];
	rgb[2] = values[2];
	return (1);
}
