/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_validation_line_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 03:45:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/08 03:33:14 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	is_map_line_bonus(const char *line)
{
	int	i;
	int	has_map_char;

	if (!line || ft_strlen(line) == 0)
		return (0);
	has_map_char = 0;
	i = 0;
	while (line[i])
	{
		if (line[i] != ' ' && line[i] != '\n' && line[i] != '\t')
		{
			if (is_valid_map_char_bonus(line[i]))
				has_map_char = 1;
			else
				return (0);
		}
		i++;
	}
	return (has_map_char);
}
