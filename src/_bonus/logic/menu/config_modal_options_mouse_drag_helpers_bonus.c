/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_modal_options_mouse_drag_helpers_bonus.c    :+:      :+:    :+:   */
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

void	config_mouse_reset_slider_drag_state_bonus(t_menu_state *menu)
{
	menu->slider_drag_index = -1;
	menu->pending_slider_index = -1;
	menu->pending_slider_value = -1;
}

void	config_mouse_find_slider_drag_target_bonus(t_menu_state *menu,
			t_menu_layout layout, int32_t mx, int32_t my)
{
	t_rect	slider;
	int		index;
	int		row_y;
	int		option;

	index = 0;
	while (index < CONFIG_MODAL_SLIDER_COUNT)
	{
		row_y = menu_layout_row_y(layout.right, index);
		option = CONFIG_MODAL_TOGGLE_COUNT + index;
		slider = menu_layout_slider_rect(layout, row_y);
		if (mx >= slider.x && mx <= slider.x + slider.w
			&& my >= row_y && my <= row_y + CONFIG_MODAL_ROW_HEIGHT)
		{
			menu->slider_drag_index = option;
			return ;
		}
		index++;
	}
}

int	config_mouse_slider_percent_from_mouse_bonus(t_rect slider, int32_t mx)
{
	int	track_max;

	track_max = slider.w - 1;
	if (track_max <= 0 || mx <= slider.x)
		return (0);
	if (mx >= slider.x + slider.w)
		return (100);
	return (((mx - slider.x) * 100) / track_max);
}

bool	config_mouse_prepare_slider_drag_bonus(t_game *game,
			t_menu_layout *layout, int32_t mx, int32_t my)
{
	t_menu_state	*menu;

	menu = &game->menu;
	*layout = menu_layout_build(menu);
	if (menu->slider_drag_index < CONFIG_MODAL_TOGGLE_COUNT
		|| menu->slider_drag_index >= CONFIG_MODAL_OPTION_COUNT)
		config_mouse_find_slider_drag_target_bonus(menu, *layout, mx, my);
	return (menu->slider_drag_index >= CONFIG_MODAL_TOGGLE_COUNT
		&& menu->slider_drag_index < CONFIG_MODAL_OPTION_COUNT);
}

bool	config_mouse_apply_slider_drag_bonus(t_game *game,
			t_menu_layout layout, t_menu_state *menu, int32_t mx)
{
	t_rect	slider;
	int		row_y;
	int		slider_idx;
	int		option;
	int		before;

	slider_idx = menu->slider_drag_index - CONFIG_MODAL_TOGGLE_COUNT;
	row_y = menu_layout_row_y(layout.right, slider_idx);
	slider = menu_layout_slider_rect(layout, row_y);
	option = CONFIG_MODAL_TOGGLE_COUNT + slider_idx;
	before = config_option_slider_value(game, option);
	config_option_set_slider_percent(game, slider_idx,
		config_mouse_slider_percent_from_mouse_bonus(slider, mx));
	menu->options.selected = menu->slider_drag_index;
	return (before != config_option_slider_value(game, option));
}
