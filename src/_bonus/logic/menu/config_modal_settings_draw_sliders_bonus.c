/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_modal_settings_draw_sliders_bonus.c         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 05:26:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/08 05:26:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config_bonus.h"
#include "structs.h"
#include "logic_bonus.h"

static void	draw_settings_slider_label(t_game *game, t_menu_layout layout,
			int option, int row_y)
{
	game->menu.labels.settings_labels[option]->instances[0].x
		= layout.right.x + CONFIG_MODAL_CARD_PADDING;
	game->menu.labels.settings_labels[option]->instances[0].y = row_y + 4;
	set_image_enabled(game->menu.labels.settings_labels[option], true);
}

static t_label_cache	slider_value_cache(t_game *game, int i)
{
	t_label_cache	cache;

	cache.text = game->menu.labels.slider_value_cache[i];
	cache.size = sizeof(game->menu.labels.slider_value_cache[i]);
	return (cache);
}

static bool	draw_settings_slider_value(t_game *game, t_menu_layout layout,
			t_rect slider_rect, int i)
{
	t_rect	value_rect;
	char	value_text[CONFIG_MODAL_SLIDER_VALUE_LEN];

	config_option_slider_text(game, CONFIG_MODAL_TOGGLE_COUNT + i, value_text,
		sizeof(value_text));
	if (!update_label_text(game, &game->menu.labels.slider_value_labels[i],
			slider_value_cache(game, i), value_text))
		return (false);
	value_rect = rect_make(slider_rect.x + slider_rect.w
			+ CONFIG_MODAL_SLIDER_VALUE_GAP, slider_rect.y
			+ (CONFIG_MODAL_SLIDER_H
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
	if (!draw_settings_slider_value(game, layout, slider_rect, i))
		return ;
}

void	draw_settings_sliders_bonus(t_game *game, t_menu_layout layout)
{
	int	i;

	i = 0;
	while (i < CONFIG_MODAL_SLIDER_COUNT)
	{
		draw_settings_slider_item(game, layout, i);
		i++;
	}
}
