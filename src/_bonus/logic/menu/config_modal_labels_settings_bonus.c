/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_modal_labels_settings_bonus.c               :+:      :+:    :+:   */
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

static void	settings_label_texts(const char **text)
{
	text[CONFIG_OPTION_SHOW_FPS] = "Show FPS";
	text[CONFIG_OPTION_CROSSHAIR] = "Show crosshair";
	text[CONFIG_OPTION_MINIMAP] = "Show minimap";
	text[CONFIG_OPTION_FULLSCREEN] = "Fullscreen";
	text[CONFIG_OPTION_DEBUG_MODE] = "DEBUG MODE";
	text[CONFIG_OPTION_FPS_LIMIT] = "FPS limit";
	text[CONFIG_OPTION_MOUSE_SENS] = "Mouse sensitivity";
	text[CONFIG_OPTION_QUALITY] = "Quality";
}

static void	hide_settings_labels(t_menu_state *menu)
{
	disable_label_group(menu->labels.settings_labels,
		CONFIG_MODAL_OPTION_COUNT);
	set_image_enabled(menu->labels.settings_sections[0], false);
	set_image_enabled(menu->labels.settings_sections[1], false);
}

bool	ensure_settings_labels(t_game *game)
{
	const char		*text[CONFIG_MODAL_OPTION_COUNT];
	t_menu_state	*menu;

	if (!game || !game->mlx)
		return (false);
	menu = &game->menu;
	settings_label_texts(text);
	if (!config_ensure_label_image_bonus(game,
			&menu->labels.settings_sections[0], "GENERAL"))
		return (false);
	if (!config_ensure_label_image_bonus(game,
			&menu->labels.settings_sections[1], "TUNING"))
		return (false);
	if (!config_ensure_label_group_text_bonus(game,
			menu->labels.settings_labels, text, CONFIG_MODAL_OPTION_COUNT))
		return (false);
	hide_settings_labels(menu);
	return (true);
}
