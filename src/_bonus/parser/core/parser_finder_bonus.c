/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_finder_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 00:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/08 03:30:09 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static int	is_standard_texture(const char *line)
{
	char	*trimmed;
	int		result;

	trimmed = trim_whitespace((char *)line);
	if (!trimmed || ft_strlen(trimmed) < 3)
		return (0);
	result = (ft_strncmp(trimmed, "NO ", 3) == 0
			|| ft_strncmp(trimmed, "SO ", 3) == 0
			|| ft_strncmp(trimmed, "WE ", 3) == 0
			|| ft_strncmp(trimmed, "EA ", 3) == 0);
	return (result);
}

static int	check_element_type_bonus(char *line, int *texture_count,
			int *color_count)
{
	if (is_link_identifier_bonus(line))
		return (0);
	if (is_texture_identifier(line))
	{
		if (is_standard_texture(line))
			(*texture_count)++;
		return (0);
	}
	if (is_color_identifier(line))
	{
		(*color_count)++;
		return (0);
	}
	if (is_map_line(line) && *texture_count == 4)
		return (1);
	if (!is_texture_identifier(line) && !is_color_identifier(line)
		&& !is_empty_line(line))
	{
		if (*texture_count < 4)
			return (-1);
		return (1);
	}
	return (0);
}

static int	handle_count_result_bonus(int result)
{
	if (result == 1)
		return (1);
	if (result == -1)
	{
		printf("Error: Map found before all textures are defined\n");
		return (-1);
	}
	return (0);
}

static int	count_elements_bonus(char **lines, int line_count,
		int *texture_count, int *color_count)
{
	int	i;
	int	result;
	int	status;

	*texture_count = 0;
	*color_count = 0;
	i = 0;
	while (i < line_count)
	{
		if (is_empty_line(lines[i]))
		{
			i++;
			continue ;
		}
		result = check_element_type_bonus(lines[i], texture_count,
				color_count);
		status = handle_count_result_bonus(result);
		if (status == 1)
			return (i);
		if (status == -1)
			return (-1);
		i++;
	}
	printf("Error: Map section not found\n");
	return (-1);
}

int	find_map_start_bonus(char **lines, int line_count)
{
	int	texture_count;
	int	color_count;

	return (count_elements_bonus(lines, line_count, &texture_count,
			&color_count));
}
