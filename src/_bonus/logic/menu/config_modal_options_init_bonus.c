/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_modal_options_init_bonus.c                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/06 15:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/01/21 21:10:57 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config_bonus.h"
#include "structs.h"
#include "logic.h"

void	config_options_init(t_game *game)
{
	static const char	*defaults[CONFIG_MODAL_CONTROL_COUNT] = {
		"W", "S", "D", "A", "RIGHT", "LEFT", "E", "R", "ESC", "M"
	};
	t_menu_state		*menu;
	int					i;

	if (!game)
		return ;
	menu = &game->menu;
	menu->options.show_fps = false;
	menu->options.crosshair = true;
	menu->options.minimap = true;
	menu->options.fullscreen = false;
	menu->options.game_speed = 70;
	menu->options.fps_limit_index = 2;
	menu->options.mouse_sens = 50;
	menu->options.selected = 0;
	menu->pending_slider_index = -1;
	menu->pending_slider_value = -1;
	menu->slider_drag_index = -1;
	menu->controls_selected = 0;
	menu->controls_rebinding = false;
	menu->controls_rebind_target = -1;
	i = 0;
	while (i < CONFIG_MODAL_CONTROL_COUNT)
	{
		ft_strlcpy(menu->controls_key_text[i], defaults[i],
			CONFIG_MODAL_KEY_LABEL_LEN);
		i++;
	}
	ft_bzero(&menu->labels, sizeof(menu->labels));
}

void	config_options_reset(t_game *game)
{
	t_menu_state	*menu;

	if (!game)
		return ;
	menu = &game->menu;
	menu->options.selected = 0;
	menu->controls_selected = 0;
	menu->controls_rebinding = false;
	menu->controls_rebind_target = -1;
	menu->pending_slider_index = -1;
	menu->pending_slider_value = -1;
	menu->slider_drag_index = -1;
}
