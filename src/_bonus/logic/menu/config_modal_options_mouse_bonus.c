/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_modal_options_mouse_bonus.c                 :+:      :+:    :+:   */
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

int	config_option_from_pos(t_game *game, int32_t mx, int32_t my)
{
	t_rect			left;
	t_rect			right;
	t_rect			slider;
	int				index;
	int				row_y;
	int				option;
	int				track_max;
	t_menu_state	*menu;
	t_menu_layout	layout;

	if (!game)
		return (-1);
	menu = &game->menu;
	if (menu->current_tab != 0)
		return (-1);
	layout = menu_layout_build(menu);
	left = layout.left;
	right = layout.right;
	index = 0;
	while (index < CONFIG_MODAL_TOGGLE_COUNT)
	{
		row_y = menu_layout_row_y(left, index);
		if (mx >= left.x && mx <= left.x + left.w
			&& my >= row_y && my <= row_y + CONFIG_MODAL_ROW_HEIGHT)
		{
			menu->pending_slider_index = -1;
			menu->pending_slider_value = -1;
			return (index);
		}
		index++;
	}
	index = 0;
	while (index < CONFIG_MODAL_SLIDER_COUNT)
	{
		row_y = menu_layout_row_y(right, index);
		option = CONFIG_MODAL_TOGGLE_COUNT + index;
		if (mx >= right.x && mx <= right.x + right.w
			&& my >= row_y && my <= row_y + CONFIG_MODAL_ROW_HEIGHT)
		{
			menu->options.selected = option;
			slider = menu_layout_slider_rect(layout, row_y);
			if (mx < slider.x || mx > slider.x + slider.w)
			{
				menu->pending_slider_index = -1;
				menu->pending_slider_value = -1;
				menu->slider_drag_index = -1;
				draw_modal_layout(game);
				return (-1);
			}
			track_max = slider.w - 1;
			if (track_max <= 0)
				menu->pending_slider_value = 0;
			else
				menu->pending_slider_value = ((mx - slider.x) * 100)
					/ track_max;
			menu->pending_slider_index = option;
			menu->slider_drag_index = option;
			return (option);
		}
		index++;
	}
	return (-1);
}

bool	config_option_drag_update(t_game *game, int32_t mx, int32_t my,
			bool mouse_down)
{
	t_rect			right;
	t_rect			slider;
	int				index;
	int				row_y;
	int				track_max;
	int				option;
	int				slider_idx;
	int				percent;
	int				before;
	t_menu_state	*menu;
	t_menu_layout	layout;

	if (!game)
		return (false);
	menu = &game->menu;
	if (menu->current_tab != 0)
		return (false);
	if (!mouse_down)
	{
		menu->slider_drag_index = -1;
		menu->pending_slider_index = -1;
		menu->pending_slider_value = -1;
		return (false);
	}
	layout = menu_layout_build(menu);
	right = layout.right;
	if (menu->slider_drag_index < CONFIG_MODAL_TOGGLE_COUNT
		|| menu->slider_drag_index >= CONFIG_MODAL_OPTION_COUNT)
	{
		index = 0;
		while (index < CONFIG_MODAL_SLIDER_COUNT)
		{
			row_y = menu_layout_row_y(right, index);
			option = CONFIG_MODAL_TOGGLE_COUNT + index;
			slider = menu_layout_slider_rect(layout, row_y);
			if (mx >= slider.x && mx <= slider.x + slider.w
				&& my >= row_y && my <= row_y + CONFIG_MODAL_ROW_HEIGHT)
			{
				menu->slider_drag_index = option;
				break ;
			}
			index++;
		}
	}
	if (menu->slider_drag_index < CONFIG_MODAL_TOGGLE_COUNT
		|| menu->slider_drag_index >= CONFIG_MODAL_OPTION_COUNT)
		return (false);
	slider_idx = menu->slider_drag_index - CONFIG_MODAL_TOGGLE_COUNT;
	row_y = menu_layout_row_y(right, slider_idx);
	slider = menu_layout_slider_rect(layout, row_y);
	track_max = slider.w - 1;
	if (track_max <= 0)
		percent = 0;
	else if (mx <= slider.x)
		percent = 0;
	else if (mx >= slider.x + slider.w)
		percent = 100;
	else
		percent = ((mx - slider.x) * 100) / track_max;
	option = CONFIG_MODAL_TOGGLE_COUNT + slider_idx;
	before = config_option_slider_value(game, option);
	config_option_set_slider_percent(game, slider_idx, percent);
	menu->options.selected = menu->slider_drag_index;
	return (before != config_option_slider_value(game, option));
}
