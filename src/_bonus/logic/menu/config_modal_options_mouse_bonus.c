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
#include "logic_bonus.h"

static void	sync_settings_selection(t_menu_state *menu)
{
	if (!menu)
		return ;
	if (menu->current_tab == CONFIG_MENU_GENERAL
		&& menu->options.selected >= CONFIG_MODAL_TOGGLE_COUNT)
		menu->options.selected = CONFIG_OPTION_SHOW_FPS;
	else if (menu->current_tab == CONFIG_MENU_TUNING
		&& menu->options.selected < CONFIG_MODAL_TOGGLE_COUNT)
		menu->options.selected = CONFIG_OPTION_FPS_LIMIT;
}

static int	menu_section_from_pos(t_menu_layout layout, int32_t mx, int32_t my)
{
	t_rect	item;
	int		i;
	int		row_y;

	i = 0;
	while (i < CONFIG_MENU_SECTION_COUNT)
	{
		row_y = layout.left.y + CONFIG_MODAL_CARD_PADDING
			+ i * (CONFIG_MODAL_MENU_ITEM_H + CONFIG_MODAL_MENU_ITEM_GAP);
		item = rect_make(layout.left.x + CONFIG_MODAL_CARD_PADDING, row_y,
				layout.left.w - CONFIG_MODAL_CARD_PADDING * 2,
				CONFIG_MODAL_MENU_ITEM_H);
		if (mx >= item.x && mx <= item.x + item.w
			&& my >= item.y && my <= item.y + item.h)
			return (i);
		i++;
	}
	return (-1);
}

static void	select_controls_from_mouse(t_game *game, t_menu_layout layout,
			int32_t mx, int32_t my)
{
	t_rect	keycap;
	int		i;
	int		row_y;

	if (game->menu.current_tab == CONFIG_MENU_CONTROLLER_CONTROLS)
		game->menu.controls_column = CONTROLS_COLUMN_CONTROLLER;
	else
		game->menu.controls_column = CONTROLS_COLUMN_KEYBOARD;
	game->menu.current_column = CONFIG_MENU_COLUMN_RIGHT;
	i = 0;
	while (i < CONFIG_MODAL_CONTROL_COUNT)
	{
		row_y = menu_layout_row_y(layout.right, i);
		if (mx >= layout.right.x && mx <= layout.right.x + layout.right.w
			&& my >= row_y && my <= row_y + CONFIG_MODAL_ROW_HEIGHT)
		{
			game->menu.controls_selected = i;
			keycap = rect_make(layout.right.x + layout.right.w
					- CONFIG_MODAL_CARD_PADDING - CONFIG_MODAL_KEYCAP_W,
					row_y + (CONFIG_MODAL_ROW_HEIGHT - CONFIG_MODAL_KEYCAP_H) / 2,
					CONFIG_MODAL_KEYCAP_W, CONFIG_MODAL_KEYCAP_H);
			if (mx >= keycap.x && mx <= keycap.x + keycap.w)
				config_controls_begin_rebind(game);
			else
				draw_modal_layout(game);
			return ;
		}
		i++;
	}
}

int	config_option_from_pos(t_game *game, int32_t mx, int32_t my)
{
	t_rect			slider;
	int				section;
	int				index;
	int				row_y;
	int				option;
	int				track_max;
	t_menu_state	*menu;
	t_menu_layout	layout;

	if (!game)
		return (-1);
	menu = &game->menu;
	layout = menu_layout_build(menu);
	section = menu_section_from_pos(layout, mx, my);
	if (section >= 0)
	{
		menu->current_column = CONFIG_MENU_COLUMN_LEFT;
		if (menu->current_tab != section)
		{
			menu->current_tab = section;
			config_controls_cancel_rebind(game);
			sync_settings_selection(menu);
			draw_modal_layout(game);
		}
		return (-1);
	}
	if (menu->current_tab == CONFIG_MENU_KEYBOARD_CONTROLS
		|| menu->current_tab == CONFIG_MENU_CONTROLLER_CONTROLS)
	{
		select_controls_from_mouse(game, layout, mx, my);
		return (-1);
	}
	if (menu->current_tab == CONFIG_MENU_GENERAL)
	{
		index = 0;
		while (index < CONFIG_MODAL_TOGGLE_COUNT)
		{
			row_y = menu_layout_row_y(layout.right, index);
			if (mx >= layout.right.x && mx <= layout.right.x + layout.right.w
				&& my >= row_y && my <= row_y + CONFIG_MODAL_ROW_HEIGHT)
			{
				menu->current_column = CONFIG_MENU_COLUMN_RIGHT;
				menu->pending_slider_index = -1;
				menu->pending_slider_value = -1;
				return (index);
			}
			index++;
		}
		return (-1);
	}
	if (menu->current_tab != CONFIG_MENU_TUNING)
		return (-1);
	index = 0;
	while (index < CONFIG_MODAL_SLIDER_COUNT)
	{
		row_y = menu_layout_row_y(layout.right, index);
		option = CONFIG_MODAL_TOGGLE_COUNT + index;
		if (mx >= layout.right.x && mx <= layout.right.x + layout.right.w
			&& my >= row_y && my <= row_y + CONFIG_MODAL_ROW_HEIGHT)
		{
			menu->current_column = CONFIG_MENU_COLUMN_RIGHT;
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
	if (menu->current_tab != CONFIG_MENU_TUNING)
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
