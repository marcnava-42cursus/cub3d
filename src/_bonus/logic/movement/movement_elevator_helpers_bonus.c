/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement_elevator_helpers_bonus.c                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 04:49:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/08 04:49:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "logic_bonus.h"
#include "structs.h"

int	is_elevator_char_logic(char c)
{
	return (get_elevator_index(c) >= 0);
}

int	find_elevator_slot(t_cub_data *data, char id)
{
	int	i;

	i = 0;
	while (i < data->elevator_id_count)
	{
		if (data->elevator_ids[i] == id)
			return (i);
		i++;
	}
	return (-1);
}

bool	is_elevator_open(t_game *game, char id)
{
	int	index;

	if (!game)
		return (false);
	index = get_elevator_index(id);
	if (index < 0 || index >= ELEVATOR_STATE_SLOTS)
		return (false);
	return (game->cub_data.map.elevator_states[index] == ELEVATOR_OPENED);
}

int	get_elevator_coords(const t_floor *floor, char id, int *x, int *y)
{
	int	i;

	if (!floor)
		return (0);
	i = 0;
	while (i < floor->elevator_count)
	{
		if (floor->elevator_ids[i] == id)
		{
			*x = floor->elevator_x[i];
			*y = floor->elevator_y[i];
			return (1);
		}
		i++;
	}
	return (0);
}

t_floor	*get_target_floor(t_game *game, int slot)
{
	t_floor	*from;
	t_floor	*to;

	from = game->cub_data.current_floor;
	to = game->cub_data.elevator_floor_a[slot];
	if (to == from)
		to = game->cub_data.elevator_floor_b[slot];
	if (!to || to == from)
		return (NULL);
	return (to);
}
