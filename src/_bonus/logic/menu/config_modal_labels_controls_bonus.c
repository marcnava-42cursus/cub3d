/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_modal_labels_controls_bonus.c               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 05:52:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/08 05:52:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config_bonus.h"
#include "structs.h"
#include "logic_bonus.h"

static void	control_label_texts(const char **text)
{
	text[0] = "Move forward";
	text[1] = "Move backward";
	text[2] = "Strafe right";
	text[3] = "Strafe left";
	text[4] = "Turn right";
	text[5] = "Turn left";
	text[6] = "Look up";
	text[7] = "Look down";
	text[8] = "Break";
	text[9] = "Place";
	text[10] = "Open menu";
	text[11] = "Open map";
	text[12] = "Accept";
	text[13] = "Quit game";
}

static void	hide_controls_labels(t_menu_state *menu)
{
	disable_label_group(menu->labels.controls_labels,
		CONFIG_MODAL_CONTROL_COUNT);
	disable_label_group(menu->labels.controls_key_labels,
		CONFIG_MODAL_CONTROL_COUNT);
	disable_label_group(menu->labels.controls_controller_labels,
		CONFIG_MODAL_CONTROL_COUNT);
	disable_label_group(menu->labels.controls_controller_key_labels,
		CONFIG_MODAL_CONTROL_COUNT);
	set_image_enabled(menu->labels.controls_header, false);
	set_image_enabled(menu->labels.controls_prompt, false);
	set_image_enabled(menu->labels.controls_controller_header, false);
	set_image_enabled(menu->labels.controls_controller_prompt, false);
}

bool	ensure_controls_labels(t_game *game)
{
	const char		*text[CONFIG_MODAL_CONTROL_COUNT];
	t_menu_state	*menu;

	if (!game || !game->mlx)
		return (false);
	menu = &game->menu;
	control_label_texts(text);
	if (!config_ensure_control_headers_bonus(game, menu))
		return (false);
	if (!config_ensure_label_group_text_bonus(game,
			menu->labels.controls_labels, text, CONFIG_MODAL_CONTROL_COUNT))
		return (false);
	if (!config_ensure_label_group_text_bonus(game,
			menu->labels.controls_controller_labels, text,
			CONFIG_MODAL_CONTROL_COUNT))
		return (false);
	if (!config_update_control_key_labels_bonus(game, menu))
		return (false);
	hide_controls_labels(menu);
	return (true);
}
