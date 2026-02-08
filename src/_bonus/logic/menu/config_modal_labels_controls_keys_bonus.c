/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_modal_labels_controls_keys_bonus.c          :+:      :+:    :+:   */
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

bool	config_ensure_control_headers_bonus(t_game *game, t_menu_state *menu)
{
	if (!config_ensure_label_image_bonus(game, &menu->labels.controls_header,
			"KEYBOARD"))
		return (false);
	if (!config_ensure_label_image_bonus(game,
			&menu->labels.controls_controller_header, "CONTROLLER"))
		return (false);
	if (!config_ensure_label_image_bonus(game, &menu->labels.controls_prompt,
			"PRESS A KEY..."))
		return (false);
	if (!config_ensure_label_image_bonus(game,
			&menu->labels.controls_controller_prompt, "PRESS A BUTTON..."))
		return (false);
	return (true);
}

static t_label_cache	label_cache(char *text, size_t size)
{
	t_label_cache	cache;

	cache.text = text;
	cache.size = size;
	return (cache);
}

static bool	update_control_key_labels_at(t_game *game, t_menu_state *menu,
			size_t i)
{
	if (!update_label_text(game, &menu->labels.controls_key_labels[i],
			label_cache(menu->labels.controls_key_cache[i],
				sizeof(menu->labels.controls_key_cache[i])),
			config_controls_key_text(game, (int)i)))
		return (false);
	if (!update_label_text(game,
			&menu->labels.controls_controller_key_labels[i],
			label_cache(menu->labels.controls_controller_key_cache[i],
				sizeof(menu->labels.controls_controller_key_cache[i])),
			config_controls_controller_text(game, (int)i)))
		return (false);
	if (!menu->labels.controls_labels[i] || !menu->labels.controls_key_labels[i]
		|| !menu->labels.controls_controller_labels[i]
		|| !menu->labels.controls_controller_key_labels[i])
		return (false);
	return (true);
}

bool	config_update_control_key_labels_bonus(t_game *game, t_menu_state *menu)
{
	size_t	i;

	i = 0;
	while (i < CONFIG_MODAL_CONTROL_COUNT)
	{
		if (!update_control_key_labels_at(game, menu, i))
			return (false);
		i++;
	}
	return (true);
}
