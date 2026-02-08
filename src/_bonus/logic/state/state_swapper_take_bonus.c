/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state_swapper_take_bonus.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 05:30:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/08 05:30:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "structs.h"
#include "logic_bonus.h"

static void	finalize_elevator_orb_take_bonus(t_game *game, int slot, char cell)
{
	int	elevator_index;

	game->orb.elevator_shot = true;
	game->orb.elevator_slot = slot;
	game->cub_data.elevator_orb[slot] = false;
	game->cub_data.elevator_orb_payload[slot] = '\0';
	elevator_index = get_elevator_index(cell);
	if (elevator_index >= 0 && elevator_index < ELEVATOR_STATE_SLOTS)
		game->cub_data.map.elevator_states[elevator_index] = ELEVATOR_CLOSED;
	game->cub_data.player.state = STATE_TAKE;
	if (game->menu.options.debug_mode)
		printf("Removed orb from elevator '%c'\n", cell);
}

bool	state_handle_elevator_orb_take_bonus(t_game *game,
			t_rayhit *hit, char cell)
{
	int		slot;
	char	payload;

	if (!state_is_elevator_bonus(cell))
		return (false);
	slot = get_elevator_slot_bonus(&game->cub_data, cell);
	if (slot < 0 || !game->cub_data.elevator_orb[slot])
		return (true);
	if (player_has_block(&game->cub_data.player))
		return (true);
	payload = game->cub_data.elevator_orb_payload[slot];
	if (payload == '\0')
		payload = '2';
	if (!orb_projectile_start_take(game, hit->cell[X], hit->cell[Y], payload))
		return (true);
	finalize_elevator_orb_take_bonus(game, slot, cell);
	return (true);
}

void	state_handle_breakable_cell_bonus(t_game *game,
			t_rayhit *hit, char cell)
{
	if (cell != '2')
		return ;
	if (orb_projectile_is_active(game))
		return ;
	if (player_has_block(&game->cub_data.player))
		return ;
	game->cub_data.map.grid[hit->cell[1]][hit->cell[0]] = '0';
	if (!orb_projectile_start_take(game, hit->cell[0], hit->cell[1], cell))
	{
		game->cub_data.map.grid[hit->cell[1]][hit->cell[0]] = cell;
		return ;
	}
	if (game->menu.options.debug_mode)
		printf("Broke block at (%d, %d)\n", hit->cell[0], hit->cell[1]);
	game->cub_data.player.state = STATE_TAKE;
}
