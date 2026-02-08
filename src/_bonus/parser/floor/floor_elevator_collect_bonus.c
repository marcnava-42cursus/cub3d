/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   floor_elevator_collect_bonus.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 03:45:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/08 03:30:04 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static int	is_elevator_char_bonus(char c)
{
	const char	*set;
	int			i;

	set = "!\"·$%&/()=?¿";
	i = 0;
	while (set[i])
	{
		if (set[i] == c)
			return (1);
		i++;
	}
	return (0);
}

static int	register_floor_elevator_bonus(t_floor *floor, int seen[256],
		char id, int cell[2])
{
	if (seen[(unsigned char)id])
	{
		printf("Error: Duplicate elevator '%c' in floor %s\n", id, floor->path);
		return (0);
	}
	seen[(unsigned char)id] = 1;
	floor->elevator_ids[floor->elevator_count] = id;
	floor->elevator_x[floor->elevator_count] = cell[0];
	floor->elevator_y[floor->elevator_count] = cell[1];
	floor->elevator_count++;
	return (1);
}

int	collect_floor_elevators_bonus(t_floor *floor)
{
	int		y;
	int		x;
	char	id;
	int		seen[256];
	int		cell[2];

	ft_bzero(seen, sizeof(seen));
	floor->elevator_count = 0;
	y = 0;
	while (y < floor->map.height)
	{
		x = 0;
		while (floor->map.grid[y][x])
		{
			id = floor->map.grid[y][x];
			cell[0] = x;
			cell[1] = y;
			if (is_elevator_char_bonus(id)
				&& !register_floor_elevator_bonus(floor, seen, id, cell))
				return (0);
			x++;
		}
		y++;
	}
	return (1);
}
