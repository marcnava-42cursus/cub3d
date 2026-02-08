/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_validation_chars_bonus.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 03:45:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/08 15:23:46 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	is_valid_map_char_bonus(char c)
{
	return (c != ' ' && c != '\n' && c != '\t' && c >= 33 && c <= 126);
}

int	validate_map_characters_bonus(t_map *map)
{
	int	x;
	int	y;

	y = 0;
	while (y < map->height)
	{
		x = 0;
		while (x < (int)ft_strlen(map->grid[y]))
		{
			if (!is_valid_map_char_bonus(map->grid[y][x])
				&& map->grid[y][x] != ' ')
			{
				printf("Error\nInvalid character '%c' at position (%d, %d)\n",
					map->grid[y][x], x, y);
				return (0);
			}
			x++;
		}
		y++;
	}
	return (1);
}
