/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_modal_visibility_state_bonus.c              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 05:26:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/08 05:26:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config_bonus.h"
#include "structs.h"
#include "logic_bonus.h"

void	toggle_config_modal(t_game *game)
{
	if (!game)
		return ;
	set_config_modal_visible(game, !game->menu.open);
}

bool	is_config_modal_open(const t_game *game)
{
	if (!game)
		return (false);
	return (game->menu.open);
}
