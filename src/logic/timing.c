/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timing.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 00:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/12/28 00:22:51 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "logic.h"

void	init_player_parameters(t_game *game)
{
	if (!game)
		return ;
	game->move_speed = 4.0f;
	game->rot_speed = 2.5f;
	game->player_radius = 0.2f;
}
