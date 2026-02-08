/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_modal_controls_rebind_helpers_bonus.c       :+:      :+:    :+:   */
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

const char	*config_key_name_from_mlx_bonus(keys_t key, char *buffer,
			size_t buffer_size)
{
	const char	*name;

	name = config_key_name_alnum_or_fn_bonus(key, buffer, buffer_size);
	if (name)
		return (name);
	name = config_key_name_literal_nav_bonus(key);
	if (name)
		return (name);
	name = config_key_name_literal_modifiers_bonus(key);
	if (name)
		return (name);
	name = config_key_name_literal_punct_bonus(key);
	if (name)
		return (name);
	config_write_prefixed_int_bonus(buffer, buffer_size, "KEY", (int)key);
	return (buffer);
}

void	config_cancel_rebind_and_redraw_bonus(t_game *game)
{
	config_controls_cancel_rebind(game);
	draw_modal_layout(game);
}

bool	config_handle_controller_rebind_key_bonus(t_game *game,
			mlx_key_data_t keydata)
{
	if (game->menu.controls_rebind_column != CONTROLS_COLUMN_CONTROLLER)
		return (false);
	if (keydata.key == MLX_KEY_BACKSPACE)
		config_cancel_rebind_and_redraw_bonus(game);
	return (true);
}

bool	config_is_invalid_rebind_target_bonus(const t_menu_state *menu)
{
	return (menu->controls_rebind_target < 0
		|| menu->controls_rebind_target >= CONFIG_MODAL_CONTROL_COUNT);
}

bool	config_handle_keyboard_rebind_cancel_bonus(t_game *game,
			t_menu_state *menu, mlx_key_data_t keydata)
{
	if (keydata.key == MLX_KEY_BACKSPACE
		|| config_is_invalid_rebind_target_bonus(menu))
	{
		config_cancel_rebind_and_redraw_bonus(game);
		return (true);
	}
	return (false);
}
