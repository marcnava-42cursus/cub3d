/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_modal_controls_draw_bonus.c                 :+:      :+:    :+:   */
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

static int	controls_card_min_height(void)
{
	return (CONFIG_MODAL_CARD_PADDING + CONFIG_MODAL_SECTION_TITLE_GAP
		+ (CONFIG_MODAL_CONTROL_COUNT - 1) * CONFIG_MODAL_ROW_SPACING
		+ CONFIG_MODAL_ROW_HEIGHT + CONFIG_MODAL_CARD_PADDING);
}

static void	draw_controls_prompt(t_game *game, t_rect card, int column)
{
	t_menu_state	*menu;
	mlx_image_t		*prompt;

	menu = &game->menu;
	if (column == CONTROLS_COLUMN_CONTROLLER)
		prompt = menu->labels.controls_controller_prompt;
	else
		prompt = menu->labels.controls_prompt;
	if (config_controls_is_rebinding(game)
		&& config_controls_rebind_column(game) == column)
	{
		prompt->instances[0].x = card.x
			+ CONFIG_MODAL_CARD_PADDING;
		prompt->instances[0].y = card.y + card.h
			- CONFIG_MODAL_CARD_PADDING
			- (int)prompt->height;
		set_image_enabled(prompt, true);
	}
	else
		set_image_enabled(prompt, false);
}

static void	select_controls_labels(t_game *game, int column,
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

static void	draw_controls_row(t_game *game, t_rect card, mlx_image_t *label,
				mlx_image_t *key_label, int row_y)
{
	t_rect		keycap;
	t_border	border;

	label->instances[0].x = card.x + CONFIG_MODAL_CARD_PADDING;
	label->instances[0].y = row_y + 4;
	set_image_enabled(label, true);
	keycap = rect_make(card.x + card.w - CONFIG_MODAL_CARD_PADDING
			- CONFIG_MODAL_KEYCAP_W,
			row_y + (CONFIG_MODAL_ROW_HEIGHT - CONFIG_MODAL_KEYCAP_H) / 2,
			CONFIG_MODAL_KEYCAP_W, CONFIG_MODAL_KEYCAP_H);
	draw_rect(game->menu.modal, keycap, CONFIG_MODAL_KEYCAP_COLOR);
	border.area = keycap;
	border.thickness = 1;
	border.color = CONFIG_MODAL_KEYCAP_BORDER_COLOR;
	draw_border(game->menu.modal, border);
	key_label->instances[0].x = keycap.x + (keycap.w - (int)key_label->width) / 2;
	key_label->instances[0].y = keycap.y + (keycap.h - (int)key_label->height) / 2;
	set_image_enabled(key_label, true);
}

static void	draw_controls_rows(t_game *game, t_rect card, int column)
{
	mlx_image_t	**labels;
	mlx_image_t	**key_labels;
	int			i;
	int			selected;
	int			row_y;

	select_controls_labels(game, column, &labels, &key_labels);
	selected = config_controls_selected(game);
	if (game->menu.current_column == CONFIG_MENU_COLUMN_LEFT)
		selected = -1;
	i = 0;
	while (i < CONFIG_MODAL_CONTROL_COUNT)
	{
		row_y = menu_layout_row_y(card, i);
		if (selected == i)
			draw_row_highlight(game, card, row_y);
		draw_controls_row(game, card, labels[i], key_labels[i], row_y);
		i++;
	}
}

static void	draw_controls_column(t_game *game, t_rect card, int column)
{
	t_menu_state	*menu;
	mlx_image_t		*header;

	menu = &game->menu;
	draw_card(game, card);
	if (column == CONTROLS_COLUMN_CONTROLLER)
		header = menu->labels.controls_controller_header;
	else
		header = menu->labels.controls_header;
	header->instances[0].x = card.x + CONFIG_MODAL_CARD_PADDING;
	header->instances[0].y = card.y + CONFIG_MODAL_CARD_PADDING;
	set_image_enabled(header, true);
	draw_controls_prompt(game, card, column);
	draw_controls_rows(game, card, column);
}

void	draw_controls_options(t_game *game, t_rect panel)
{
	t_menu_layout	layout;
	int				min_height;
	int				column;

	if (!game || !game->menu.modal || !ensure_controls_labels(game))
		return ;
	(void)panel;
	layout = menu_layout_build(&game->menu);
	if (game->menu.current_tab == CONFIG_MENU_CONTROLLER_CONTROLS)
		column = CONTROLS_COLUMN_CONTROLLER;
	else
		column = CONTROLS_COLUMN_KEYBOARD;
	min_height = controls_card_min_height();
	if (min_height > layout.content.h)
		min_height = layout.content.h;
	if (layout.right.h < min_height)
		layout.right.h = min_height;
	draw_controls_column(game, layout.right, column);
}
