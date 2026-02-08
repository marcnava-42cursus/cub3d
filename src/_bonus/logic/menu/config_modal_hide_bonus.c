/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_modal_hide_bonus.c                          :+:      :+:    :+:   */
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

void	hide_settings_options(t_game *game)
{
	t_menu_state	*menu;

	if (!game)
		return ;
	menu = &game->menu;
	disable_label_group(menu->labels.settings_labels,
		CONFIG_MODAL_OPTION_COUNT);
	disable_label_group(menu->labels.slider_value_labels,
		CONFIG_MODAL_SLIDER_COUNT);
	set_image_enabled(menu->labels.settings_sections[0], false);
	set_image_enabled(menu->labels.settings_sections[1], false);
}

void	hide_controls_options(t_game *game)
{
	t_menu_state	*menu;

	if (!game)
		return ;
	menu = &game->menu;
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
