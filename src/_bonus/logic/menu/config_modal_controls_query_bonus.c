/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_modal_controls_query_bonus.c                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/06 15:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/01/21 21:35:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config_bonus.h"
#include "structs.h"
#include "logic.h"

int	config_controls_selected(t_game *game)
{
	t_menu_state	*menu;

	if (!game)
		return (0);
	menu = &game->menu;
	return (menu->controls_selected);
}

bool	config_controls_is_rebinding(t_game *game)
{
	t_menu_state	*menu;

	if (!game)
		return (false);
	menu = &game->menu;
	return (menu->controls_rebinding);
}

const char	*config_controls_key_text(t_game *game, int index)
{
	t_menu_state	*menu;

	if (!game || index < 0 || index >= CONFIG_MODAL_CONTROL_COUNT)
		return ("");
	menu = &game->menu;
	return (menu->controls_key_text[index]);
}
