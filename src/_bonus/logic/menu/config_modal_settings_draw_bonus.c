/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_modal_settings_draw_bonus.c                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/06 15:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/01/21 21:13:30 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config_bonus.h"
#include "structs.h"
#include "logic.h"

static void	place_settings_sections(t_menu_state *menu,
			t_rect left, t_rect right)
{
	menu->labels.settings_sections[0]->instances[0].x = left.x
		+ CONFIG_MODAL_CARD_PADDING;
	menu->labels.settings_sections[0]->instances[0].y = left.y
		+ CONFIG_MODAL_CARD_PADDING;
	menu->labels.settings_sections[1]->instances[0].x = right.x
		+ CONFIG_MODAL_CARD_PADDING;
	menu->labels.settings_sections[1]->instances[0].y = right.y
		+ CONFIG_MODAL_CARD_PADDING;
	set_image_enabled(menu->labels.settings_sections[0], true);
	set_image_enabled(menu->labels.settings_sections[1], true);
}

static void	draw_settings_toggles(t_game *game, t_menu_layout layout)
{
	t_rect	left;
	int		i;
	int		row_y;
	bool	selected;
	t_rect	toggle_rect;

	left = layout.left;
	i = 0;
	while (i < CONFIG_MODAL_TOGGLE_COUNT)
	{
		row_y = menu_layout_row_y(left, i);
		selected = (game->menu.options.selected == i);
		if (selected)
			draw_row_highlight(game, left, row_y);
		game->menu.labels.settings_labels[i]->instances[0].x = left.x
			+ CONFIG_MODAL_CARD_PADDING;
		game->menu.labels.settings_labels[i]->instances[0].y = row_y + 4;
		set_image_enabled(game->menu.labels.settings_labels[i], true);
		toggle_rect = rect_make(left.x + left.w - CONFIG_MODAL_CARD_PADDING
				- CONFIG_MODAL_TOGGLE_W,
				row_y + (CONFIG_MODAL_ROW_HEIGHT - CONFIG_MODAL_TOGGLE_H) / 2,
				CONFIG_MODAL_TOGGLE_W, CONFIG_MODAL_TOGGLE_H);
		draw_toggle_switch(game, toggle_rect,
			config_option_toggle_state(game, i), selected);
		i++;
	}
}

static void	draw_settings_sliders(t_game *game, t_menu_layout layout)
{
	t_rect	right;
	int		i;
	int		row_y;
	bool	selected;
	t_rect	slider_rect;
	t_rect	value_rect;
	char	value_text[CONFIG_MODAL_SLIDER_VALUE_LEN];

	right = layout.right;
	i = 0;
	while (i < CONFIG_MODAL_SLIDER_COUNT)
	{
		row_y = menu_layout_row_y(right, i);
		selected = (game->menu.options.selected
				== CONFIG_MODAL_TOGGLE_COUNT + i);
		if (selected)
			draw_row_highlight(game, right, row_y);
		game->menu.labels.settings_labels[CONFIG_MODAL_TOGGLE_COUNT + i]
			->instances[0].x = right.x + CONFIG_MODAL_CARD_PADDING;
		game->menu.labels.settings_labels[CONFIG_MODAL_TOGGLE_COUNT + i]
			->instances[0].y = row_y + 4;
		set_image_enabled(game->menu.labels.settings_labels
		[CONFIG_MODAL_TOGGLE_COUNT + i], true);
		slider_rect = menu_layout_slider_rect(layout, row_y);
		draw_slider(game, slider_rect,
			config_option_slider_value(game, CONFIG_MODAL_TOGGLE_COUNT + i),
			selected);
		config_option_slider_text(game, CONFIG_MODAL_TOGGLE_COUNT + i,
			value_text, sizeof(value_text));
		if (update_label_text(game,
				&game->menu.labels.slider_value_labels[i],
				game->menu.labels.slider_value_cache[i],
				sizeof(game->menu.labels.slider_value_cache[i]),
				value_text))
		{
			value_rect = rect_make(slider_rect.x + slider_rect.w
					+ CONFIG_MODAL_SLIDER_VALUE_GAP,
					row_y + (CONFIG_MODAL_ROW_HEIGHT
						- (int)game->menu.labels
						.slider_value_labels[i]->height) / 2,
					CONFIG_MODAL_SLIDER_VALUE_W,
					(int)game->menu.labels
					.slider_value_labels[i]->height);
			if (value_rect.x
				+ (int)game->menu.labels.slider_value_labels[i]->width
				> right.x + right.w - CONFIG_MODAL_CARD_PADDING)
				value_rect.x = right.x + right.w
					- CONFIG_MODAL_CARD_PADDING
					- (int)game->menu.labels
					.slider_value_labels[i]->width;
			game->menu.labels.slider_value_labels[i]->instances[0].x
				= value_rect.x;
			game->menu.labels.slider_value_labels[i]->instances[0].y
				= value_rect.y;
			set_image_enabled(game->menu.labels.slider_value_labels[i],
				true);
		}
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
	draw_card(game, layout.left);
	draw_card(game, layout.right);
	place_settings_sections(menu, layout.left, layout.right);
	draw_settings_toggles(game, layout);
	draw_settings_sliders(game, layout);
}
