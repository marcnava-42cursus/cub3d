/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_modal_options_mouse_tuning_bonus.c          :+:      :+:    :+:   */
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

void	config_mouse_set_tuning_slider_pending_bonus(t_menu_state *menu,
			t_rect slider, int option, int32_t mx)
{
	int	track_max;

	track_max = slider.w - 1;
	if (track_max <= 0)
		menu->pending_slider_value = 0;
	else
		menu->pending_slider_value = ((mx - slider.x) * 100) / track_max;
	menu->pending_slider_index = option;
	menu->slider_drag_index = option;
}

int	config_mouse_select_tuning_option_bonus(t_game *game,
			t_menu_layout layout, int32_t mx, int32_t my)
{
	int	index;
	int	row_y;
	int	option;

	index = 0;
	while (index < CONFIG_MODAL_SLIDER_COUNT)
	{
		row_y = menu_layout_row_y(layout.right, index);
		option = CONFIG_MODAL_TOGGLE_COUNT + index;
		if (config_mouse_point_in_row_bonus(layout.right, row_y, mx, my))
			return (config_mouse_select_tuning_row_bonus(game, layout, option,
					mx));
		index++;
	}
	return (-1);
}
