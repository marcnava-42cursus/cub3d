/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   orb_projectile_state_bonus.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 00:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/07 02:23:38 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	set_default_door_texture_state(t_game *game)
{
	t_anim			*door;
	unsigned int	width;

	if (!game || !game->cub_data.effects.door_anims)
		return ;
	door = &game->cub_data.effects.door_anims[DOOR_OPEN];
	if (!door->atlas)
		return ;
	width = door->atlas->max_frame[X];
	if (width == 0)
		return ;
	door->current_frame[X] = 0;
	door->current_frame[Y] = 0;
}

void	init_orb_projectile_advanced(t_game *game)
{
	int	i;

	if (!game)
		return ;
	ft_bzero(&game->orb, sizeof(t_orb_projectile));
	game->orb.speed = ORB_PROJECTILE_DEFAULT_SPEED;
	game->orb.mode = ORB_MODE_NONE;
	game->orb.target_cell_x = -1;
	game->orb.target_cell_y = -1;
	game->orb.elevator_place = false;
	game->orb.elevator_slot = -1;
	game->orb.last_draw_active = false;
	i = 0;
	while (i < game->cub_data.elevator_id_count)
	{
		game->cub_data.elevator_orb[i] = false;
		game->cub_data.elevator_orb_payload[i] = '\0';
		i++;
	}
	set_default_door_texture_state(game);
}

bool	orb_projectile_is_active(const t_game *game)
{
	if (!game)
		return (false);
	return (game->orb.active);
}

bool	orb_projectile_needs_redraw(const t_game *game)
{
	if (!game)
		return (false);
	if (game->orb.needs_redraw)
		return (true);
	if (game->orb.active != game->orb.last_draw_active)
		return (true);
	if (game->orb.active && (game->orb.x != game->orb.last_draw_x
			|| game->orb.y != game->orb.last_draw_y))
		return (true);
	return (false);
}

void	orb_projectile_mark_drawn(t_game *game)
{
	if (!game)
		return ;
	game->orb.needs_redraw = false;
	game->orb.last_draw_active = game->orb.active;
	game->orb.last_draw_x = game->orb.x;
	game->orb.last_draw_y = game->orb.y;
}
