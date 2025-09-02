/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 01:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/02 01:48:31 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parser.h"

int	is_map_line(const char *line)
{
	int	i;

	if (!line || strlen(line) == 0)
		return (0);

	i = 0;
	while (line[i])
	{
		if (line[i] != '0' && line[i] != '1' && line[i] != 'N'
			&& line[i] != 'S' && line[i] != 'E' && line[i] != 'W'
			&& line[i] != ' ' && line[i] != '\n')
		{
			return (0);
		}
		i++;
	}

	i = 0;
	while (line[i])
	{
		if (line[i] == '0' || line[i] == '1' || line[i] == 'N'
			|| line[i] == 'S' || line[i] == 'E' || line[i] == 'W')
		{
			return (1);
		}
		i++;
	}

	return (0);
}

int	find_player_position(t_map *map, t_player *player)
{
	int	x;
	int	y;
	int	player_count;
	char	c;

	player_count = 0;
	y = 0;
	while (y < map->height)
	{
		x = 0;
		while (x < (int)strlen(map->grid[y]))
		{
			c = map->grid[y][x];
			if (c == 'N' || c == 'S' || c == 'E' || c == 'W')
			{
				player_count++;
				player->x = x;
				player->y = y;
				player->orientation = (t_orientation)c;
				map->grid[y][x] = '0';
			}
			x++;
		}
		y++;
	}

	if (player_count == 0)
	{
		printf("Error: No player position found in map\n");
		return (0);
	}

	if (player_count > 1)
	{
		printf("Error: Multiple player positions found in map\n");
		return (0);
	}

	return (1);
}

static int	is_valid_map_char(char c)
{
	return (c == '0' || c == '1' || c == 'N' || c == 'S' || c == 'E' || c == 'W' || c == ' ');
}

static int	validate_map_characters(t_map *map)
{
	int	x;
	int	y;

	y = 0;
	while (y < map->height)
	{
		x = 0;
		while (x < (int)strlen(map->grid[y]))
		{
			if (!is_valid_map_char(map->grid[y][x]))
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

static int	check_position(t_map *map, int x, int y)
{
	// Verificar límites
	if (y < 0 || y >= map->height)
		return (0);  // Fuera del mapa = pared

	if (x < 0 || x >= (int)strlen(map->grid[y]))
		return (0);  // Fuera de la línea = pared

	char c = map->grid[y][x];
	return (c == '1');  // Solo las paredes son válidas como límites
}

int	is_map_closed(t_map *map)
{
	int	x, y;

	// Verificar cada espacio vacío ('0') o posición de jugador
	for (y = 0; y < map->height; y++)
	{
		for (x = 0; x < (int)strlen(map->grid[y]); x++)
		{
			char c = map->grid[y][x];

			// Si es un espacio transitable, verificar que esté rodeado
			if (c == '0' || c == 'N' || c == 'S' || c == 'E' || c == 'W')
			{
				// Verificar los 4 lados
				int up = check_position(map, x, y - 1);
				int down = check_position(map, x, y + 1);
				int left = check_position(map, x - 1, y);
				int right = check_position(map, x + 1, y);

				// Si algún lado está fuera o es espacio, el mapa no está cerrado
				if (y == 0 || y == map->height - 1)  // Primera o última fila
				{
					if (c != '1')
					{
						printf("Error: Map not closed at border (%d, %d)\n", x, y);
						return (0);
					}
				}

				if (x == 0 || x == (int)strlen(map->grid[y]) - 1)  // Primer o último carácter
				{
					if (c != '1')
					{
						printf("Error: Map not closed at border (%d, %d)\n", x, y);
						return (0);
					}
				}

				// Verificar espacios adyacentes
				if (y > 0 && x < (int)strlen(map->grid[y - 1]) && map->grid[y - 1][x] == ' ')
				{
					printf("Error: Map not closed - space adjacent to traversable area at (%d, %d)\n", x, y);
					return (0);
				}
				if (y < map->height - 1 && x < (int)strlen(map->grid[y + 1]) && map->grid[y + 1][x] == ' ')
				{
					printf("Error: Map not closed - space adjacent to traversable area at (%d, %d)\n", x, y);
					return (0);
				}
				if (x > 0 && map->grid[y][x - 1] == ' ')
				{
					printf("Error: Map not closed - space adjacent to traversable area at (%d, %d)\n", x, y);
					return (0);
				}
				if (x < (int)strlen(map->grid[y]) - 1 && map->grid[y][x + 1] == ' ')
				{
					printf("Error: Map not closed - space adjacent to traversable area at (%d, %d)\n", x, y);
					return (0);
				}
			}
		}
	}

	return (1);
}

int	validate_map(t_map *map, t_player *player)
{
	// Validar caracteres del mapa
	if (!validate_map_characters(map))
		return (0);

	// Encontrar posición del jugador
	if (!find_player_position(map, player))
		return (0);

	// Verificar que el mapa esté cerrado
	if (!is_map_closed(map))
		return (0);

	return (1);
}

static int	calculate_map_width(char **lines, int start, int end)
{
	int	max_width = 0;
	int	i;

	for (i = start; i <= end; i++)
	{
		int len = strlen(lines[i]);
		// Quitar el \n del final si existe
		if (len > 0 && lines[i][len - 1] == '\n')
			len--;
		if (len > max_width)
			max_width = len;
	}

	return (max_width);
}

int	parse_map_section(char **lines, int start_line, t_cub_data *data)
{
	int	i, j;
	int	map_end = start_line;
	int	line_len;

	// Encontrar el final del mapa (última línea no vacía)
	for (i = start_line; lines[i]; i++)
	{
		if (!is_empty_line(lines[i]) && is_map_line(lines[i]))
			map_end = i;
	}

	data->map.height = map_end - start_line + 1;
	data->map.width = calculate_map_width(lines, start_line, map_end);

	if (data->map.height <= 0 || data->map.width <= 0)
	{
		printf("Error: Invalid map dimensions\n");
		return (0);
	}

	// Alocar memoria para el mapa
	data->map.grid = malloc(sizeof(char *) * (data->map.height + 1));
	if (!data->map.grid)
		return (0);

	// Copiar líneas del mapa
	for (i = 0; i < data->map.height; i++)
	{
		char *line = lines[start_line + i];
		line_len = strlen(line);

		// Quitar \n del final si existe
		if (line_len > 0 && line[line_len - 1] == '\n')
			line_len--;

		data->map.grid[i] = malloc(line_len + 1);
		if (!data->map.grid[i])
		{
			// Liberar memoria ya alocada
			for (j = 0; j < i; j++)
				free(data->map.grid[j]);
			free(data->map.grid);
			return (0);
		}

		// Copiar caracteres
		for (j = 0; j < line_len; j++)
			data->map.grid[i][j] = line[j];
		data->map.grid[i][line_len] = '\0';
	}

	data->map.grid[data->map.height] = NULL;

	// Validar el mapa
	return (validate_map(&data->map, &data->player));
}
