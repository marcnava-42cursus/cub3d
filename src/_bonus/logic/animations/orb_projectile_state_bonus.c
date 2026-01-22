/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   orb_projectile_state_bonus.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 00:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/01/22 00:00:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	init_orb_projectile_bonus(t_game *game)
{
	if (!game)
		return ;
	ft_bzero(&game->orb, sizeof(t_orb_projectile));
	game->orb.speed = ORB_PROJECTILE_DEFAULT_SPEED;
	game->orb.mode = ORB_MODE_NONE;
	game->orb.target_cell_x = -1;
	game->orb.target_cell_y = -1;
	game->orb.last_draw_active = false;
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
