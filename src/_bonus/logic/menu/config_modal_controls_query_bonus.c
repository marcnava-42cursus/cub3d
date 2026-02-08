/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_modal_controls_query_bonus.c                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/06 15:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/08 05:26:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config_bonus.h"
#include "structs.h"
#include "logic_bonus.h"

int	config_controls_selected(t_game *game)
{
	t_menu_state	*menu;

	if (!game)
		return (0);
	menu = &game->menu;
	return (menu->controls_selected);
}

int	config_controls_column(t_game *game)
{
	t_menu_state	*menu;

	if (!game)
		return (CONTROLS_COLUMN_KEYBOARD);
	menu = &game->menu;
	return (menu->controls_column);
}

int	config_controls_rebind_column(t_game *game)
{
	t_menu_state	*menu;

	if (!game)
		return (-1);
	menu = &game->menu;
	return (menu->controls_rebind_column);
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
