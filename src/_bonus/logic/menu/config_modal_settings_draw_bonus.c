/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_modal_settings_draw_bonus.c                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/06 15:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/08 05:26:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config_bonus.h"
#include "structs.h"
#include "logic_bonus.h"

static void	place_settings_section(t_menu_state *menu, t_rect card,
			int section)
{
	mlx_image_t	*label;

	if (section == CONFIG_MENU_TUNING)
		label = menu->labels.settings_sections[1];
	else
		label = menu->labels.settings_sections[0];
	if (!label)
		return ;
	label->instances[0].x = card.x + CONFIG_MODAL_CARD_PADDING;
	label->instances[0].y = card.y + CONFIG_MODAL_CARD_PADDING;
	set_image_enabled(label, true);
}

static void	draw_settings_toggles(t_game *game, t_rect card)
{
	int		i;
	int		row_y;
	bool	selected;
	t_rect	toggle_rect;

	i = 0;
	while (i < CONFIG_MODAL_TOGGLE_COUNT)
	{
		row_y = menu_layout_row_y(card, i);
		selected = (game->menu.current_column == CONFIG_MENU_COLUMN_RIGHT
				&& game->menu.options.selected == i);
		if (selected)
			draw_row_highlight(game, card, row_y);
		game->menu.labels.settings_labels[i]->instances[0].x = card.x
			+ CONFIG_MODAL_CARD_PADDING;
		game->menu.labels.settings_labels[i]->instances[0].y = row_y + 4;
		set_image_enabled(game->menu.labels.settings_labels[i], true);
		toggle_rect = rect_make(card.x + card.w - CONFIG_MODAL_CARD_PADDING
				- CONFIG_MODAL_TOGGLE_W,
				row_y + (CONFIG_MODAL_ROW_HEIGHT - CONFIG_MODAL_TOGGLE_H) / 2,
				CONFIG_MODAL_TOGGLE_W, CONFIG_MODAL_TOGGLE_H);
		draw_toggle_switch(game, toggle_rect,
			config_option_toggle_state(game, i), selected);
		i++;
	}
}

void	draw_settings_options(t_game *game, t_rect panel)
{
	t_menu_layout	layout;
	t_menu_state	*menu;

	if (!game || !game->menu.modal || !ensure_settings_labels(game))
		return ;
	menu = &game->menu;
	(void)panel;
	layout = menu_layout_build(menu);
	disable_label_group(menu->labels.settings_labels,
		CONFIG_MODAL_OPTION_COUNT);
	disable_label_group(menu->labels.slider_value_labels,
		CONFIG_MODAL_SLIDER_COUNT);
	set_image_enabled(menu->labels.settings_sections[0], false);
	set_image_enabled(menu->labels.settings_sections[1], false);
	draw_card(game, layout.right);
	place_settings_section(menu, layout.right, menu->current_tab);
	if (menu->current_tab == CONFIG_MENU_TUNING)
		draw_settings_sliders_bonus(game, layout);
	else
		draw_settings_toggles(game, layout.right);
}
