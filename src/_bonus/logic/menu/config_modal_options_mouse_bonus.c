/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_modal_options_mouse_bonus.c                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/06 15:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/08 06:04:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config_bonus.h"
#include "structs.h"
#include "logic_bonus.h"

void	config_mouse_sync_settings_selection_bonus(t_menu_state *menu)
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

int	config_mouse_menu_section_from_pos_bonus(t_menu_layout layout,
			int32_t mx, int32_t my)
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

bool	config_mouse_point_in_row_bonus(t_rect area, int row_y,
			int32_t mx, int32_t my)
{
	return (mx >= area.x && mx <= area.x + area.w
		&& my >= row_y && my <= row_y + CONFIG_MODAL_ROW_HEIGHT);
}

bool	config_mouse_handle_section_click_bonus(t_game *game, int section)
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
	config_mouse_sync_settings_selection_bonus(menu);
	draw_modal_layout(game);
	return (true);
}

int	config_option_from_pos(t_game *game, int32_t mx, int32_t my)
{
	t_menu_layout	layout;

	if (!game)
		return (-1);
	layout = menu_layout_build(&game->menu);
	if (config_mouse_handle_section_click_bonus(game,
			config_mouse_menu_section_from_pos_bonus(layout, mx, my)))
		return (-1);
	if (game->menu.current_tab == CONFIG_MENU_KEYBOARD_CONTROLS
		|| game->menu.current_tab == CONFIG_MENU_CONTROLLER_CONTROLS)
	{
		config_mouse_select_controls_from_mouse_bonus(game, layout, mx, my);
		return (-1);
	}
	if (game->menu.current_tab == CONFIG_MENU_GENERAL)
		return (config_mouse_select_general_option_bonus(&game->menu, layout,
				mx, my));
	if (game->menu.current_tab != CONFIG_MENU_TUNING)
		return (-1);
	return (config_mouse_select_tuning_option_bonus(game, layout, mx, my));
}
