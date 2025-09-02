/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_colors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 03:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/02 03:00:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/parser.h"

int	is_color_identifier(const char *line)
{
	char	*trimmed;
	int		result;

	trimmed = trim_whitespace((char *)line);
	if (!trimmed)
		return (0);
	result = (strncmp(trimmed, "F ", 2) == 0 || strncmp(trimmed, "C ", 2) == 0);
	return (result);
}

static int	validate_identifier(char identifier)
{
	if (identifier != 'F' && identifier != 'C')
	{
		printf("Error: Invalid color identifier: %c\n", identifier);
		return (0);
	}
	return (1);
}

static int	process_values(const char *line, int *rgb)
{
	char	*values_str;

	values_str = extract_color_values(line);
	if (!values_str)
		return (0);
	if (!parse_rgb_values(values_str, rgb))
	{
		free(values_str);
		return (0);
	}
	free(values_str);
	return (1);
}

int	parse_color_line(const char *line, t_color *floor, t_color *ceiling)
{
	char	*trimmed;
	char	identifier;
	int		rgb[3];

	trimmed = trim_whitespace((char *)line);
	if (!trimmed || strlen(trimmed) < 2)
		return (0);
	identifier = trimmed[0];
	if (!validate_identifier(identifier))
		return (0);
	if (!process_values(line, rgb))
		return (0);
	return (assign_color(identifier, floor, ceiling, rgb));
}
