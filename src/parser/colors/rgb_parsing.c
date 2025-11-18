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

static int	convert_token_to_int(char *token)
{
	char	*endptr;
	int		value;

	while (*token == ' ' || *token == '\t')
		token++;
	if (*token == '\0')
		return (-1);
	value = strtol(token, &endptr, 10);
	if (*endptr != '\0' || endptr == token)
		return (-1);
	return (value);
}

static int	extract_rgb_tokens(char *str_copy, int *values)
{
	char	*token;
	char	*saveptr;
	int		count;
	int		value;

	count = 0;
	token = strtok_r(str_copy, ",", &saveptr);
	while (token && count < 3)
	{
		value = convert_token_to_int(token);
		if (value == -1)
			return (0);
		values[count] = value;
		count++;
		token = strtok_r(NULL, ",", &saveptr);
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
