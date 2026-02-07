/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   step_audio_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 00:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/07 04:34:57 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "logic_bonus.h"

bool	audio_step_update_loop(bool player_is_moving)
{
	return (audio_step_set_loop(player_is_moving));
}
