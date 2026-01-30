/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collision_alias_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+         +:+     */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 00:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/01/29 00:00:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "logic.h"

bool	is_cell_blocking_bonus(t_game *game, int cell_x, int cell_y);
bool	collides_with_wall_bonus(t_game *game, float x, float y);
void	attempt_move_bonus(t_game *game, float step_x, float step_y);

bool	is_cell_blocking(t_game *game, int cell_x, int cell_y)
{
	return (is_cell_blocking_bonus(game, cell_x, cell_y));
}

bool	collides_with_wall(t_game *game, float x, float y)
{
	return (collides_with_wall_bonus(game, x, y));
}

void	attempt_move(t_game *game, float step_x, float step_y)
{
	attempt_move_bonus(game, step_x, step_y);
}
