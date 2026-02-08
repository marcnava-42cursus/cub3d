/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_modal_options_mouse_drag_bonus.c            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 06:04:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/08 06:04:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config_bonus.h"
#include "structs.h"
#include "logic_bonus.h"

bool	config_option_drag_update(t_game *game, int32_t mx, int32_t my,
			bool mouse_down)
{
	t_menu_state	*menu;
	t_menu_layout	layout;

	if (!game)
		return (false);
	menu = &game->menu;
	if (menu->current_tab != CONFIG_MENU_TUNING)
		return (false);
	if (!mouse_down)
	{
		config_mouse_reset_slider_drag_state_bonus(menu);
		return (false);
	}
	if (!config_mouse_prepare_slider_drag_bonus(game, &layout, mx, my))
		return (false);
	return (config_mouse_apply_slider_drag_bonus(game, layout, menu, mx));
}
