/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_modal_settings_draw_bonus.c                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/06 15:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/07 18:53:29 by marcnava         ###   ########.fr       */
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
	label->instances[0].x = card.x
		+ CONFIG_MODAL_CARD_PADDING;
	label->instances[0].y = card.y
		+ CONFIG_MODAL_CARD_PADDING;
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

static void	draw_settings_slider_label(t_game *game, t_menu_layout layout,
				int option, int row_y)
{
	game->menu.labels.settings_labels[option]->instances[0].x
		= layout.right.x + CONFIG_MODAL_CARD_PADDING;
	game->menu.labels.settings_labels[option]->instances[0].y = row_y + 4;
	set_image_enabled(game->menu.labels.settings_labels[option], true);
}

static bool	draw_settings_slider_value(t_game *game, t_menu_layout layout,
				t_rect slider_rect, int i, int row_y)
{
	t_rect	value_rect;
	char	value_text[CONFIG_MODAL_SLIDER_VALUE_LEN];

	config_option_slider_text(game, CONFIG_MODAL_TOGGLE_COUNT + i, value_text,
		sizeof(value_text));
	if (!update_label_text(game, &game->menu.labels.slider_value_labels[i],
			game->menu.labels.slider_value_cache[i],
			sizeof(game->menu.labels.slider_value_cache[i]), value_text))
		return (false);
	value_rect = rect_make(slider_rect.x + slider_rect.w
			+ CONFIG_MODAL_SLIDER_VALUE_GAP, row_y
			+ (CONFIG_MODAL_ROW_HEIGHT
				- (int)game->menu.labels.slider_value_labels[i]->height) / 2,
			CONFIG_MODAL_SLIDER_VALUE_W,
			(int)game->menu.labels.slider_value_labels[i]->height);
	if (value_rect.x + (int)game->menu.labels.slider_value_labels[i]->width
		> layout.right.x + layout.right.w - CONFIG_MODAL_CARD_PADDING)
		value_rect.x = layout.right.x + layout.right.w
			- CONFIG_MODAL_CARD_PADDING
			- (int)game->menu.labels.slider_value_labels[i]->width;
	game->menu.labels.slider_value_labels[i]->instances[0].x = value_rect.x;
	game->menu.labels.slider_value_labels[i]->instances[0].y = value_rect.y;
	set_image_enabled(game->menu.labels.slider_value_labels[i], true);
	return (true);
}

static void	draw_settings_slider_item(t_game *game, t_menu_layout layout, int i)
{
	int		option;
	int		row_y;
	bool	selected;
	t_rect	slider_rect;

	option = CONFIG_MODAL_TOGGLE_COUNT + i;
	row_y = menu_layout_row_y(layout.right, i);
	selected = (game->menu.current_column == CONFIG_MENU_COLUMN_RIGHT
			&& game->menu.options.selected == option);
	if (selected)
		draw_row_highlight(game, layout.right, row_y);
	draw_settings_slider_label(game, layout, option, row_y);
	slider_rect = menu_layout_slider_rect(layout, row_y);
	draw_slider(game, slider_rect, config_option_slider_value(game, option),
		selected);
	if (!draw_settings_slider_value(game, layout, slider_rect, i, row_y))
		return ;
}

static void	draw_settings_sliders(t_game *game, t_menu_layout layout)
{
	int	i;

	i = 0;
	while (i < CONFIG_MODAL_SLIDER_COUNT)
	{
		draw_settings_slider_item(game, layout, i);
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
		draw_settings_sliders(game, layout);
	else
		draw_settings_toggles(game, layout.right);
}
