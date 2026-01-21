/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_modal_controls_rebind_bonus.c               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/06 15:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/01/21 21:12:46 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config_bonus.h"
#include "structs.h"
#include "logic.h"
#include <stdio.h>

static const char	*key_name_from_mlx(keys_t key, char *buffer,
			size_t buffer_size)
{
	if (key >= MLX_KEY_A && key <= MLX_KEY_Z)
	{
		buffer[0] = (char)('A' + (key - MLX_KEY_A));
		buffer[1] = '\0';
		return (buffer);
	}
	if (key >= MLX_KEY_0 && key <= MLX_KEY_9)
	{
		buffer[0] = (char)('0' + (key - MLX_KEY_0));
		buffer[1] = '\0';
		return (buffer);
	}
	if (key >= MLX_KEY_F1 && key <= MLX_KEY_F12)
	{
		snprintf(buffer, buffer_size, "F%d", 1 + (key - MLX_KEY_F1));
		return (buffer);
	}
	if (key == MLX_KEY_SPACE)
		return ("SPACE");
	if (key == MLX_KEY_ENTER || key == MLX_KEY_KP_ENTER)
		return ("ENTER");
	if (key == MLX_KEY_LEFT)
		return ("LEFT");
	if (key == MLX_KEY_RIGHT)
		return ("RIGHT");
	if (key == MLX_KEY_UP)
		return ("UP");
	if (key == MLX_KEY_DOWN)
		return ("DOWN");
	if (key == MLX_KEY_TAB)
		return ("TAB");
	if (key == MLX_KEY_BACKSPACE)
		return ("BACK");
	if (key == MLX_KEY_ESCAPE)
		return ("ESC");
	if (key == MLX_KEY_LEFT_SHIFT)
		return ("LSHIFT");
	if (key == MLX_KEY_RIGHT_SHIFT)
		return ("RSHIFT");
	if (key == MLX_KEY_LEFT_CONTROL)
		return ("LCTRL");
	if (key == MLX_KEY_RIGHT_CONTROL)
		return ("RCTRL");
	if (key == MLX_KEY_LEFT_ALT)
		return ("LALT");
	if (key == MLX_KEY_RIGHT_ALT)
		return ("RALT");
	if (key == MLX_KEY_LEFT_SUPER)
		return ("LGUI");
	if (key == MLX_KEY_RIGHT_SUPER)
		return ("RGUI");
	if (key == MLX_KEY_CAPS_LOCK)
		return ("CAPS");
	if (key == MLX_KEY_MINUS)
		return ("-");
	if (key == MLX_KEY_EQUAL)
		return ("=");
	if (key == MLX_KEY_COMMA)
		return (",");
	if (key == MLX_KEY_PERIOD)
		return (".");
	snprintf(buffer, buffer_size, "KEY%d", (int)key);
	return (buffer);
}

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
	if (keydata.key == MLX_KEY_BACKSPACE)
	{
		config_controls_cancel_rebind(game);
		draw_modal_layout(game);
		return (true);
	}
	if (menu->controls_rebind_target < 0
		|| menu->controls_rebind_target >= CONFIG_MODAL_CONTROL_COUNT)
	{
		config_controls_cancel_rebind(game);
		draw_modal_layout(game);
		return (true);
	}
	name = key_name_from_mlx(keydata.key, name_buf, sizeof(name_buf));
	ft_strlcpy(menu->controls_key_text[menu->controls_rebind_target], name,
		CONFIG_MODAL_KEY_LABEL_LEN);
	config_controls_cancel_rebind(game);
	draw_modal_layout(game);
	return (true);
}
