/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_modal_options_mouse_controls_bonus.c        :+:      :+:    :+:   */
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

bool	config_mouse_select_controls_row_bonus(t_game *game,
			t_menu_layout layout, int row, int32_t mx)
{
	t_rect	keycap;
	int		row_y;

	game->menu.controls_selected = row;
	row_y = menu_layout_row_y(layout.right, row);
	keycap = rect_make(layout.right.x + layout.right.w
			- CONFIG_MODAL_CARD_PADDING - CONFIG_MODAL_KEYCAP_W,
			row_y + (CONFIG_MODAL_ROW_HEIGHT - CONFIG_MODAL_KEYCAP_H) / 2,
			CONFIG_MODAL_KEYCAP_W, CONFIG_MODAL_KEYCAP_H);
	if (mx >= keycap.x && mx <= keycap.x + keycap.w)
		config_controls_begin_rebind(game);
	else
		draw_modal_layout(game);
	return (true);
}

void	config_mouse_select_controls_from_mouse_bonus(t_game *game,
			t_menu_layout layout, int32_t mx, int32_t my)
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
		if (config_mouse_point_in_row_bonus(layout.right, row_y, mx, my)
			&& config_mouse_select_controls_row_bonus(game, layout, i, mx))
			return ;
		i++;
	}
}

int	config_mouse_select_general_option_bonus(t_menu_state *menu,
			t_menu_layout layout, int32_t mx, int32_t my)
{
	int	index;
	int	row_y;

	index = 0;
	while (index < CONFIG_MODAL_TOGGLE_COUNT)
	{
		row_y = menu_layout_row_y(layout.right, index);
		if (config_mouse_point_in_row_bonus(layout.right, row_y, mx, my))
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

int	config_mouse_clear_tuning_slider_selection_bonus(t_game *game,
			t_menu_state *menu)
{
	menu->pending_slider_index = -1;
	menu->pending_slider_value = -1;
	menu->slider_drag_index = -1;
	draw_modal_layout(game);
	return (-1);
}

int	config_mouse_select_tuning_row_bonus(t_game *game,
			t_menu_layout layout, int option, int32_t mx)
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
		return (config_mouse_clear_tuning_slider_selection_bonus(game, menu));
	config_mouse_set_tuning_slider_pending_bonus(menu, slider, option, mx);
	return (option);
}
