/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_modal_controls_state_bonus.c                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/06 15:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/01/21 21:14:28 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config_bonus.h"
#include "structs.h"
#include "logic.h"

void	config_controls_select(t_game *game, int delta)
{
	int				selected;
	t_menu_state	*menu;

	if (!game)
		return ;
	menu = &game->menu;
	if (menu->controls_rebinding)
		return ;
	selected = menu->controls_selected + delta;
	if (selected < 0)
		selected = CONFIG_MODAL_CONTROL_COUNT - 1;
	else if (selected >= CONFIG_MODAL_CONTROL_COUNT)
		selected = 0;
	menu->controls_selected = selected;
	draw_modal_layout(game);
}

void	config_controls_begin_rebind(t_game *game)
{
	t_menu_state	*menu;

	if (!game)
		return ;
	menu = &game->menu;
	menu->controls_rebinding = true;
	menu->controls_rebind_target = menu->controls_selected;
	draw_modal_layout(game);
}

void	config_controls_cancel_rebind(t_game *game)
{
	t_menu_state	*menu;

	if (!game)
		return ;
	menu = &game->menu;
	menu->controls_rebinding = false;
	menu->controls_rebind_target = -1;
}
