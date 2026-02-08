/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_modal_cards_slider_bonus.c                  :+:      :+:    :+:   */
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
