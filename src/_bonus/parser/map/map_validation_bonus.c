/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_validation_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 00:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/11/13 20:03:03 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "structs.h"

static int	is_valid_map_char_bonus(char c)
{
	return (c != ' ' && c != '\n' && c != '\t' && c >= 33 && c <= 126);
}

static int	is_walkable_char_bonus(char c)
{
	if (c == '0')
		return (1);
	if (c == 'N' || c == 'S' || c == 'E' || c == 'W')
		return (1);
	if (c >= 'a' && c <= 'z')
		return (1);
	return (0);
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
				printf("Error: Invalid character '%c' at position (%d, %d)\n",
					map->grid[y][x], x, y);
				return (0);
			}
			x++;
		}
		y++;
	}
	return (1);
}

int	is_map_closed_bonus(t_map *map)
{
	int		x;
	int		y;
	char	c;

	y = 0;
	while (y < map->height)
	{
		x = 0;
		while (x < (int)ft_strlen(map->grid[y]))
		{
			c = map->grid[y][x];
			if (is_walkable_char_bonus(c))
			{
				if (y == 0 || y == map->height - 1)
				{
					printf("Error: Walkable char '%c' at border (%d, %d)\n",
						c, x, y);
					return (0);
				}
				if (x == 0 || x == (int)ft_strlen(map->grid[y]) - 1)
				{
					printf("Error: Walkable char '%c' at border (%d, %d)\n",
						c, x, y);
					return (0);
				}
				if (y > 0 && x < (int)ft_strlen(map->grid[y - 1])
					&& map->grid[y - 1][x] == ' ')
				{
					printf(
						"Error: Map not closed - space adjacent at (%d, %d)\n",
						x, y);
					return (0);
				}
				if (y < map->height - 1 && x < (int)ft_strlen(map->grid[y + 1])
					&& map->grid[y + 1][x] == ' ')
				{
					printf(
						"Error: Map not closed - space adjacent at (%d, %d)\n",
						x, y);
					return (0);
				}
				if (x > 0 && map->grid[y][x - 1] == ' ')
				{
					printf(
						"Error: Map not closed - space adjacent at (%d, %d)\n",
						x, y);
					return (0);
				}
				if (x < (int)ft_strlen(map->grid[y]) - 1
					&& map->grid[y][x + 1] == ' ')
				{
					printf(
						"Error: Map not closed - space adjacent at (%d, %d)\n",
						x, y);
					return (0);
				}
			}
			x++;
		}
		y++;
	}
	return (1);
}

static int	is_player_char(char c)
{
	return (c == 'N' || c == 'S' || c == 'E' || c == 'W');
}

int	find_player_position_bonus(t_map *map, t_player *player)
{
	int		x;
	int		y;
	int		found;

	found = 0;
	y = -1;
	while (++y < map->height)
	{
		x = -1;
		while (++x < (int)ft_strlen(map->grid[y]))
		{
			if (is_player_char(map->grid[y][x]))
			{
				if (found)
				{
					printf("Error: Multiple player positions found\n");
					return (0);
				}
				player->x = (float)x + 0.5f;
				player->y = (float)y + 0.5f;
				player->orientation = map->grid[y][x];
				if (player->orientation == NORTH)
					player->angle = 3.0f * (FT_PI / 2);
				else if (player->orientation == SOUTH)
					player->angle = FT_PI / 2;
				else if (player->orientation == EAST)
					player->angle = 0.0f;
				else if (player->orientation == WEST)
					player->angle = FT_PI;
				map->grid[y][x] = '0';
				found = 1;
			}
		}
	}
	if (!found)
	{
		player->x = -1.0f;
		player->y = -1.0f;
		player->angle = 0.0f;
		player->orientation = 0;
	}
	return (1);
}

int	validate_map_bonus(t_map *map, t_player *player)
{
	if (!validate_map_characters_bonus(map))
		return (0);
	if (!find_player_position_bonus(map, player))
		return (0);
	if (!is_map_closed_bonus(map))
		return (0);
	return (1);
}

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
