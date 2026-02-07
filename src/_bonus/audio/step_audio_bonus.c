/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   step_audio_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 00:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/07 00:00:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "logic_bonus.h"

void	bonus_audio_update_step_loop(bool player_is_moving)
{
	static bool	was_moving;

	if (player_is_moving == was_moving)
		return ;
	was_moving = player_is_moving;
	bonus_audio_set_step_loop(player_is_moving);
}
