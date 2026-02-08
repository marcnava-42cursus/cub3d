/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_modal_options_actions_bonus.c               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/06 15:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/08 05:40:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config_bonus.h"
#include "structs.h"
#include "logic_bonus.h"

#include <stdio.h>

static const char	*slider_name(int index)
{
	if (index == CONFIG_OPTION_FPS_LIMIT)
		return ("FPS Limit");
	if (index == CONFIG_OPTION_MOUSE_SENS)
		return ("Mouse Sensitivity");
	if (index == CONFIG_OPTION_QUALITY)
		return ("Quality");
	return ("Slider");
}

static void	adjust_slider_option(t_game *game, int index, t_menu_state *menu)
{
	int		slider;
	char	value_text[CONFIG_MODAL_SLIDER_VALUE_LEN];

	slider = index - CONFIG_MODAL_TOGGLE_COUNT;
	if (menu->pending_slider_index == index && menu->pending_slider_value >= 0)
		config_option_set_slider_percent(game, slider,
			menu->pending_slider_value);
	else
		config_option_step_slider(game, slider, 1);
	menu->pending_slider_index = -1;
	menu->pending_slider_value = -1;
	config_option_slider_text(game, index, value_text, sizeof(value_text));
	printf("Config: %s %s\n", slider_name(index), value_text);
}

void	config_option_toggle(t_game *game, int index)
{
	t_menu_state	*menu;
	bool			*toggle;

	if (!game || index < 0 || index >= CONFIG_MODAL_OPTION_COUNT)
		return ;
	menu = &game->menu;
	menu->options.selected = index;
	if (index < CONFIG_MODAL_TOGGLE_COUNT)
	{
		toggle = config_toggle_value_ptr_bonus(game, index);
		if (!toggle)
			return ;
		config_toggle_option_value_bonus(game, index, toggle);
	}
	else
		adjust_slider_option(game, index, menu);
	draw_modal_layout(game);
}

void	config_option_adjust(t_game *game, int delta)
{
	int	index;
	int	slider;

	if (!game)
		return ;
	index = game->menu.options.selected;
	if (index < CONFIG_MODAL_TOGGLE_COUNT || index >= CONFIG_MODAL_OPTION_COUNT)
		return ;
	slider = index - CONFIG_MODAL_TOGGLE_COUNT;
	config_option_step_slider(game, slider, delta);
	game->menu.pending_slider_index = -1;
	game->menu.pending_slider_value = -1;
	draw_modal_layout(game);
}
