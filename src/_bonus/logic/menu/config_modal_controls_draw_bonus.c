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
#include "logic.h"

static void	draw_controls_prompt(t_game *game, t_rect card)
{
	t_menu_state	*menu;

	menu = &game->menu;
	if (config_controls_is_rebinding(game))
	{
		menu->labels.controls_prompt->instances[0].x = card.x
			+ CONFIG_MODAL_CARD_PADDING;
		menu->labels.controls_prompt->instances[0].y = card.y + card.h
			- CONFIG_MODAL_CARD_PADDING
			- (int)menu->labels.controls_prompt->height;
		set_image_enabled(menu->labels.controls_prompt, true);
	}
	else
		set_image_enabled(menu->labels.controls_prompt, false);
}

static void	draw_controls_rows(t_game *game, t_rect card)
{
	t_rect		keycap;
	t_border	border;
	int			i;
	int			row_y;
	int			selected;

	selected = config_controls_selected(game);
	i = 0;
	while (i < CONFIG_MODAL_CONTROL_COUNT)
	{
		row_y = menu_layout_row_y(card, i);
		if (selected == i)
			draw_row_highlight(game, card, row_y);
		game->menu.labels.controls_labels[i]->instances[0].x = card.x
			+ CONFIG_MODAL_CARD_PADDING;
		game->menu.labels.controls_labels[i]->instances[0].y = row_y + 4;
		set_image_enabled(game->menu.labels.controls_labels[i], true);
		keycap = rect_make(card.x + card.w - CONFIG_MODAL_CARD_PADDING
				- CONFIG_MODAL_KEYCAP_W,
				row_y + (CONFIG_MODAL_ROW_HEIGHT - CONFIG_MODAL_KEYCAP_H) / 2,
				CONFIG_MODAL_KEYCAP_W, CONFIG_MODAL_KEYCAP_H);
		draw_rect(game->menu.modal, keycap, CONFIG_MODAL_KEYCAP_COLOR);
		border.area = keycap;
		border.thickness = 1;
		border.color = CONFIG_MODAL_KEYCAP_BORDER_COLOR;
		draw_border(game->menu.modal, border);
		game->menu.labels.controls_key_labels[i]->instances[0].x = keycap.x
			+ (keycap.w
				- (int)game->menu.labels.controls_key_labels[i]->width) / 2;
		game->menu.labels.controls_key_labels[i]->instances[0].y = keycap.y
			+ (keycap.h
				- (int)game->menu.labels.controls_key_labels[i]->height) / 2;
		set_image_enabled(game->menu.labels.controls_key_labels[i], true);
		i++;
	}
}

void	draw_controls_options(t_game *game, t_rect panel)
{
	t_menu_layout	layout;
	t_rect			card;
	t_menu_state	*menu;

	if (!game || !game->menu.modal || !ensure_controls_labels(game))
		return ;
	menu = &game->menu;
	(void)panel;
	layout = menu_layout_build(menu);
	card = rect_make(layout.content.x, layout.content.y,
			layout.content.w, layout.content.h);
	draw_card(game, card);
	menu->labels.controls_header->instances[0].x = card.x
		+ CONFIG_MODAL_CARD_PADDING;
	menu->labels.controls_header->instances[0].y = card.y
		+ CONFIG_MODAL_CARD_PADDING;
	set_image_enabled(menu->labels.controls_header, true);
	draw_controls_prompt(game, card);
	draw_controls_rows(game, card);
}
