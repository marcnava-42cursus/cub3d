/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_orchestrator_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 03:45:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/08 03:28:07 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	init_cub_data(t_cub_data *data);

int	process_file_data_bonus(char **lines, int line_count, t_cub_data *data)
{
	int			map_start;
	const char	*error_prefix;

	error_prefix = "Error: ";
	map_start = find_map_start(lines, line_count);
	if (map_start == -1)
	{
		printf("%sMap section not found\n", error_prefix);
		return (0);
	}
	if (!parse_elements_bonus(lines, map_start, data))
	{
		printf("%sFailed parsing elements before map\n", error_prefix);
		return (0);
	}
	if (!parse_map_with_errors_bonus(lines, map_start, data, error_prefix))
		return (0);
	return (1);
}

int	parse_cub_file_bonus(const char *filename, t_cub_data *data)
{
	char	**lines;
	int		line_count;

	if (!validate_file_extension(filename))
	{
		printf("Error: Invalid file extension. Expected .cub\n");
		return (0);
	}
	init_cub_data(data);
	lines = read_file_lines(filename, &line_count);
	if (!lines)
	{
		printf("Error: Could not read file %s\n", filename);
		return (0);
	}
	if (!process_file_data_bonus(lines, line_count, data))
	{
		free_cub_data(data);
		free_lines(lines, line_count);
		return (0);
	}
	if (!build_floor_graph_bonus(filename, data))
		return (0);
	free_lines(lines, line_count);
	return (1);
}
