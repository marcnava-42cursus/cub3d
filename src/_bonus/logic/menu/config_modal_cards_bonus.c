/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_modal_cards_bonus.c                         :+:      :+:    :+:   */
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

void	draw_card(t_game *game, t_rect card)
{
	t_border	border;

	draw_vertical_gradient(game->menu.modal, card,
		CONFIG_MODAL_CARD_TOP_COLOR, CONFIG_MODAL_CARD_BOTTOM_COLOR);
	border.area = card;
	border.thickness = CONFIG_MODAL_BORDER_THICKNESS;
	draw_bevel_border(game->menu.modal, border,
		CONFIG_MODAL_CARD_BORDER_LIGHT, CONFIG_MODAL_CARD_BORDER_DARK);
}

void	draw_row_highlight(t_game *game, t_rect card, int row_y)
{
	t_rect	highlight;
	t_rect	accent;

	highlight = rect_make(card.x + 6, row_y + 2, card.w - 12,
			CONFIG_MODAL_ROW_HEIGHT - 4);
	accent = rect_make(highlight.x, highlight.y, 3, highlight.h);
	draw_rect(game->menu.modal, highlight, CONFIG_MODAL_ACCENT_SOFT);
	draw_rect(game->menu.modal, accent, CONFIG_MODAL_ACCENT_COLOR);
}

static t_rect	toggle_knob_rect(t_rect rect, bool enabled)
{
	t_rect	knob;

	knob.w = rect.h - 4;
	knob.h = rect.h - 4;
	knob.y = rect.y + 2;
	if (enabled)
		knob.x = rect.x + rect.w - knob.w - 2;
	else
		knob.x = rect.x + 2;
	return (knob);
}

static void	draw_selected_knob_border(t_game *game, t_rect knob)
{
	t_border	border;

	border.area = knob;
	border.thickness = 1;
	border.color = CONFIG_MODAL_ACCENT_COLOR;
	draw_border(game->menu.modal, border);
}

void	draw_toggle_switch(t_game *game, t_rect rect, bool enabled,
			bool selected)
{
	t_border	border;
	t_rect		knob;
	uint32_t	color;

	if (enabled)
		color = CONFIG_MODAL_TOGGLE_ON_COLOR;
	else
		color = CONFIG_MODAL_TOGGLE_OFF_COLOR;
	draw_rect(game->menu.modal, rect, color);
	border.area = rect;
	border.thickness = 1;
	border.color = CONFIG_MODAL_CARD_BORDER_DARK;
	draw_border(game->menu.modal, border);
	knob = toggle_knob_rect(rect, enabled);
	draw_rect(game->menu.modal, knob, CONFIG_MODAL_SLIDER_KNOB_COLOR);
	if (selected)
		draw_selected_knob_border(game, knob);
}
