/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_modal_options_mouse_bonus.c                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/06 15:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/08 00:11:09 by marcnava         ###   ########.fr       */
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

static bool	point_in_row(t_rect area, int row_y, int32_t mx, int32_t my)
{
	return (mx >= area.x && mx <= area.x + area.w
		&& my >= row_y && my <= row_y + CONFIG_MODAL_ROW_HEIGHT);
}

static bool	select_controls_row(t_game *game, t_menu_layout layout,
				int row, int32_t mx)
{
	t_rect	keycap;
	int		row_y;

	game->menu.controls_selected = row;
	row_y = menu_layout_row_y(layout.right, row);
	keycap = rect_make(layout.right.x + layout.right.w
			- CONFIG_MODAL_CARD_PADDING
			- CONFIG_MODAL_KEYCAP_W, row_y + (CONFIG_MODAL_ROW_HEIGHT
				- CONFIG_MODAL_KEYCAP_H) / 2, CONFIG_MODAL_KEYCAP_W,
			CONFIG_MODAL_KEYCAP_H);
	if (mx >= keycap.x && mx <= keycap.x + keycap.w)
		config_controls_begin_rebind(game);
	else
		draw_modal_layout(game);
	return (true);
}

static void	select_controls_from_mouse(t_game *game, t_menu_layout layout,
				int32_t mx, int32_t my)
{
	t_menu_state	*menu;
	int				i;
	int				row_y;

	menu = &game->menu;
	if (menu->current_tab == CONFIG_MENU_CONTROLLER_CONTROLS)
		menu->controls_column = CONTROLS_COLUMN_CONTROLLER;
	else
		menu->controls_column = CONTROLS_COLUMN_KEYBOARD;
	menu->current_column = CONFIG_MENU_COLUMN_RIGHT;
	i = 0;
	while (i < CONFIG_MODAL_CONTROL_COUNT)
	{
		row_y = menu_layout_row_y(layout.right, i);
		if (point_in_row(layout.right, row_y, mx, my))
			if (select_controls_row(game, layout, i, mx))
				return ;
		i++;
	}
}

static int	select_general_option(t_menu_state *menu, t_menu_layout layout,
				int32_t mx, int32_t my)
{
	int	index;
	int	row_y;

	index = 0;
	while (index < CONFIG_MODAL_TOGGLE_COUNT)
	{
		row_y = menu_layout_row_y(layout.right, index);
		if (point_in_row(layout.right, row_y, mx, my))
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

static int	clear_tuning_slider_selection(t_game *game, t_menu_state *menu)
{
	menu->pending_slider_index = -1;
	menu->pending_slider_value = -1;
	menu->slider_drag_index = -1;
	draw_modal_layout(game);
	return (-1);
}

static void	set_tuning_slider_pending(t_menu_state *menu, t_rect slider,
				int option, int32_t mx)
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

static int	select_tuning_row(t_game *game, t_menu_layout layout, int option,
				int32_t mx)
{
	t_rect			slider;
	int				row_y;
	int				slider_idx;
	t_menu_state	*menu;

	menu = &game->menu;
	slider_idx = option - CONFIG_MODAL_TOGGLE_COUNT;
	row_y = menu_layout_row_y(layout.right, slider_idx);
	menu->current_column = CONFIG_MENU_COLUMN_RIGHT;
	menu->options.selected = option;
	slider = menu_layout_slider_rect(layout, row_y);
	if (mx < slider.x || mx > slider.x + slider.w)
		return (clear_tuning_slider_selection(game, menu));
	set_tuning_slider_pending(menu, slider, option, mx);
	return (option);
}

static int	select_tuning_option(t_game *game, t_menu_layout layout,
				int32_t mx, int32_t my)
{
	int	index;
	int	row_y;
	int	option;

	index = 0;
	while (index < CONFIG_MODAL_SLIDER_COUNT)
	{
		row_y = menu_layout_row_y(layout.right, index);
		option = CONFIG_MODAL_TOGGLE_COUNT + index;
		if (point_in_row(layout.right, row_y, mx, my))
			return (select_tuning_row(game, layout, option, mx));
		index++;
	}
	return (-1);
}

static bool	handle_section_click(t_game *game, int section)
{
	t_menu_state	*menu;

	if (section < 0)
		return (false);
	menu = &game->menu;
	menu->current_column = CONFIG_MENU_COLUMN_LEFT;
	if (menu->current_tab == section)
		return (true);
	menu->current_tab = section;
	config_controls_cancel_rebind(game);
	sync_settings_selection(menu);
	draw_modal_layout(game);
	return (true);
}

int	config_option_from_pos(t_game *game, int32_t mx, int32_t my)
{
	int				section;
	t_menu_state	*menu;
	t_menu_layout	layout;

	if (!game)
		return (-1);
	menu = &game->menu;
	layout = menu_layout_build(menu);
	section = menu_section_from_pos(layout, mx, my);
	if (handle_section_click(game, section))
		return (-1);
	if (menu->current_tab == CONFIG_MENU_KEYBOARD_CONTROLS
		|| menu->current_tab == CONFIG_MENU_CONTROLLER_CONTROLS)
	{
		select_controls_from_mouse(game, layout, mx, my);
		return (-1);
	}
	if (menu->current_tab == CONFIG_MENU_GENERAL)
		return (select_general_option(menu, layout, mx, my));
	if (menu->current_tab != CONFIG_MENU_TUNING)
		return (-1);
	return (select_tuning_option(game, layout, mx, my));
}

static void	reset_slider_drag_state(t_menu_state *menu)
{
	menu->slider_drag_index = -1;
	menu->pending_slider_index = -1;
	menu->pending_slider_value = -1;
}

static void	find_slider_drag_target(t_menu_state *menu, t_menu_layout layout,
				int32_t mx, int32_t my)
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

static int	slider_percent_from_mouse(t_rect slider, int32_t mx)
{
	int	track_max;

	track_max = slider.w - 1;
	if (track_max <= 0 || mx <= slider.x)
		return (0);
	if (mx >= slider.x + slider.w)
		return (100);
	return (((mx - slider.x) * 100) / track_max);
}

static bool	prepare_slider_drag(t_game *game, t_menu_layout *layout, int32_t mx,
				int32_t my)
{
	t_menu_state	*menu;

	menu = &game->menu;
	*layout = menu_layout_build(menu);
	if (menu->slider_drag_index < CONFIG_MODAL_TOGGLE_COUNT
		|| menu->slider_drag_index >= CONFIG_MODAL_OPTION_COUNT)
		find_slider_drag_target(menu, *layout, mx, my);
	return (menu->slider_drag_index >= CONFIG_MODAL_TOGGLE_COUNT
		&& menu->slider_drag_index < CONFIG_MODAL_OPTION_COUNT);
}

static bool	apply_slider_drag(t_game *game, t_menu_layout layout,
				t_menu_state *menu, int32_t mx)
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
		slider_percent_from_mouse(slider, mx));
	menu->options.selected = menu->slider_drag_index;
	return (before != config_option_slider_value(game, option));
}

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
		reset_slider_drag_state(menu);
		return (false);
	}
	if (!prepare_slider_drag(game, &layout, mx, my))
		return (false);
	return (apply_slider_drag(game, layout, menu, mx));
}
