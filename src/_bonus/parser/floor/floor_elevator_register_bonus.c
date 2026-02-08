/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   floor_elevator_register_bonus.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 03:45:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/08 15:23:46 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static int	find_or_create_elevator_slot_bonus(t_cub_data *data, char id)
{
	int	slot;

	slot = 0;
	while (slot < data->elevator_id_count)
	{
		if (data->elevator_ids[slot] == id)
			return (slot);
		slot++;
	}
	if (data->elevator_id_count >= 16)
	{
		printf("Error\nToo many elevator identifiers\n");
		return (-1);
	}
	data->elevator_ids[slot] = id;
	data->elevator_id_count++;
	return (slot);
}

static int	register_floor_in_slot_bonus(t_cub_data *data, t_floor *floor,
		int slot, char id)
{
	if (!data->elevator_floor_a[slot])
		data->elevator_floor_a[slot] = floor;
	else if (data->elevator_floor_a[slot] != floor
		&& !data->elevator_floor_b[slot])
		data->elevator_floor_b[slot] = floor;
	else if (data->elevator_floor_a[slot] != floor
		&& data->elevator_floor_b[slot]
		&& data->elevator_floor_b[slot] != floor)
	{
		printf("Error\nElevator '%c' appears in more than two floors\n", id);
		return (0);
	}
	return (1);
}

int	register_floor_elevators_bonus(t_cub_data *data, t_floor *floor)
{
	int	i;
	int	slot;

	i = 0;
	while (i < floor->elevator_count)
	{
		slot = find_or_create_elevator_slot_bonus(data,
				floor->elevator_ids[i]);
		if (slot < 0)
			return (0);
		if (!register_floor_in_slot_bonus(data, floor, slot,
				floor->elevator_ids[i]))
			return (0);
		i++;
	}
	return (1);
}
