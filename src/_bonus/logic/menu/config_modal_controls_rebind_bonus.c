/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_modal_controls_rebind_bonus.c               :+:      :+:    :+:   */
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

bool	config_controls_handle_key(t_game *game, mlx_key_data_t keydata)
{
	char			name_buf[CONFIG_MODAL_KEY_LABEL_LEN];
	const char		*name;
	t_menu_state	*menu;

	if (!game)
		return (false);
	menu = &game->menu;
	if (!menu->controls_rebinding)
		return (false);
	if (keydata.action != MLX_PRESS)
		return (true);
	if (config_handle_controller_rebind_key_bonus(game, keydata))
		return (true);
	if (config_handle_keyboard_rebind_cancel_bonus(game, menu, keydata))
		return (true);
	menu->controls_key_codes[menu->controls_rebind_target] = keydata.key;
	name = config_key_name_from_mlx_bonus(keydata.key, name_buf,
			sizeof(name_buf));
	ft_strlcpy(menu->controls_key_text[menu->controls_rebind_target], name,
		CONFIG_MODAL_KEY_LABEL_LEN);
	config_cancel_rebind_and_redraw_bonus(game);
	return (true);
}
