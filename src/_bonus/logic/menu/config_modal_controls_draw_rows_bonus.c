/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_modal_controls_draw_rows_bonus.c            :+:      :+:    :+:   */
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

void	config_controls_select_labels_bonus(t_game *game, int column,
			mlx_image_t ***labels, mlx_image_t ***key_labels)
{
	if (column == CONTROLS_COLUMN_CONTROLLER)
	{
		*labels = game->menu.labels.controls_controller_labels;
		*key_labels = game->menu.labels.controls_controller_key_labels;
	}
	else
	{
		*labels = game->menu.labels.controls_labels;
		*key_labels = game->menu.labels.controls_key_labels;
	}
}

void	config_controls_draw_row_label_bonus(mlx_image_t *label, t_rect card,
			int row_y)
{
	label->instances[0].x = card.x + CONFIG_MODAL_CARD_PADDING;
	label->instances[0].y = row_y + 4;
	set_image_enabled(label, true);
}

t_rect	config_controls_draw_row_keycap_bonus(t_game *game, t_rect card,
			int row_y)
{
	t_rect		keycap;
	t_border	border;

	keycap = rect_make(card.x + card.w - CONFIG_MODAL_CARD_PADDING
			- CONFIG_MODAL_KEYCAP_W,
			row_y + (CONFIG_MODAL_ROW_HEIGHT - CONFIG_MODAL_KEYCAP_H) / 2,
			CONFIG_MODAL_KEYCAP_W, CONFIG_MODAL_KEYCAP_H);
	draw_rect(game->menu.modal, keycap, CONFIG_MODAL_KEYCAP_COLOR);
	border.area = keycap;
	border.thickness = 1;
	border.color = CONFIG_MODAL_KEYCAP_BORDER_COLOR;
	draw_border(game->menu.modal, border);
	return (keycap);
}

void	config_controls_draw_row_key_label_bonus(mlx_image_t *key_label,
			t_rect keycap)
{
	key_label->instances[0].x = keycap.x
		+ (keycap.w - (int)key_label->width) / 2;
	key_label->instances[0].y = keycap.y
		+ (keycap.h - (int)key_label->height) / 2;
	set_image_enabled(key_label, true);
}

void	config_controls_draw_rows_bonus(t_game *game, t_rect card, int column)
{
	mlx_image_t	**labels;
	mlx_image_t	**key_labels;
	int			i;
	int			selected;
	int			row_y;

	config_controls_select_labels_bonus(game, column, &labels, &key_labels);
	selected = config_controls_selected(game);
	if (game->menu.current_column == CONFIG_MENU_COLUMN_LEFT)
		selected = -1;
	i = 0;
	while (i < CONFIG_MODAL_CONTROL_COUNT)
	{
		row_y = menu_layout_row_y(card, i);
		if (selected == i)
			draw_row_highlight(game, card, row_y);
		config_controls_draw_row_label_bonus(labels[i], card, row_y);
		config_controls_draw_row_key_label_bonus(key_labels[i],
			config_controls_draw_row_keycap_bonus(game, card, row_y));
		i++;
	}
}
