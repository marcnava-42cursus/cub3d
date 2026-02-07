/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_modal_cards_bonus.c                         :+:      :+:    :+:   */
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

	highlight = rect_make(card.x + 6, row_y + 2, card.w - 12,
			CONFIG_MODAL_ROW_HEIGHT - 4);
	draw_rect(game->menu.modal, highlight, CONFIG_MODAL_ACCENT_SOFT);
	draw_rect(game->menu.modal, rect_make(highlight.x, highlight.y,
			3, highlight.h), CONFIG_MODAL_ACCENT_COLOR);
}

void	draw_toggle_switch(t_game *game, t_rect rect, bool enabled,
				bool selected)
{
	t_border	border;
	t_rect		knob;

	draw_rect(game->menu.modal, rect, enabled
		? CONFIG_MODAL_TOGGLE_ON_COLOR : CONFIG_MODAL_TOGGLE_OFF_COLOR);
	border.area = rect;
	border.thickness = 1;
	border.color = CONFIG_MODAL_CARD_BORDER_DARK;
	draw_border(game->menu.modal, border);
	knob.w = rect.h - 4;
	knob.h = rect.h - 4;
	knob.y = rect.y + 2;
	if (enabled)
		knob.x = rect.x + rect.w - knob.w - 2;
	else
		knob.x = rect.x + 2;
	draw_rect(game->menu.modal, knob, CONFIG_MODAL_SLIDER_KNOB_COLOR);
	if (selected)
	{
		border.area = knob;
		border.thickness = 1;
		border.color = CONFIG_MODAL_ACCENT_COLOR;
			draw_border(game->menu.modal, border);
	}
}

static int	slider_knob_x(t_rect track, int value)
{
	int	knob_x;

	knob_x = track.x + (track.w - 1) * value / 100
		- CONFIG_MODAL_SLIDER_KNOB / 2;
	if (knob_x < track.x)
		knob_x = track.x;
	if (knob_x + CONFIG_MODAL_SLIDER_KNOB > track.x + track.w)
		knob_x = track.x + track.w - CONFIG_MODAL_SLIDER_KNOB;
	return (knob_x);
}

void	draw_slider(t_game *game, t_rect track, int value, bool selected)
{
	t_rect		fill;
	t_rect		knob;
	t_border	border;

	draw_rect(game->menu.modal, track, CONFIG_MODAL_SLIDER_TRACK_COLOR);
	fill = track;
	fill.w = (track.w * value) / 100;
	draw_rect(game->menu.modal, fill, CONFIG_MODAL_SLIDER_FILL_COLOR);
	knob = rect_make(slider_knob_x(track, value),
			track.y + track.h / 2 - CONFIG_MODAL_SLIDER_KNOB / 2,
			CONFIG_MODAL_SLIDER_KNOB, CONFIG_MODAL_SLIDER_KNOB);
	draw_rect(game->menu.modal, knob, CONFIG_MODAL_SLIDER_KNOB_COLOR);
	border.area = knob;
	border.thickness = 1;
	border.color = CONFIG_MODAL_CARD_BORDER_DARK;
	draw_border(game->menu.modal, border);
	if (selected)
	{
		border.color = CONFIG_MODAL_ACCENT_COLOR;
		draw_border(game->menu.modal, border);
	}
}
