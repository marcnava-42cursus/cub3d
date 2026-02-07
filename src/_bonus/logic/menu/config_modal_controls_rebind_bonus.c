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
#include "logic_bonus.h"

static void	write_prefixed_int(char *buffer, size_t buffer_size,
			const char *prefix, int value)
{
	char	*num;

	if (!buffer || buffer_size == 0)
		return ;
	buffer[0] = '\0';
	ft_strlcpy(buffer, prefix, buffer_size);
	num = ft_itoa(value);
	if (!num)
		return ;
	ft_strlcat(buffer, num, buffer_size);
	free(num);
}

static const char	*key_name_literal_nav(keys_t key)
{
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
	return (NULL);
}

static const char	*key_name_literal_modifiers(keys_t key)
{
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
	return (NULL);
}

static const char	*key_name_literal_punct(keys_t key)
{
	if (key == MLX_KEY_MINUS)
		return ("-");
	if (key == MLX_KEY_EQUAL)
		return ("=");
	if (key == MLX_KEY_COMMA)
		return (",");
	if (key == MLX_KEY_PERIOD)
		return (".");
	return (NULL);
}

static const char	*key_name_alnum_or_fn(keys_t key, char *buffer,
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
		write_prefixed_int(buffer, buffer_size, "F", 1 + (key - MLX_KEY_F1));
		return (buffer);
	}
	return (NULL);
}

static const char	*key_name_from_mlx(keys_t key, char *buffer,
				size_t buffer_size)
{
	const char	*name;

	name = key_name_alnum_or_fn(key, buffer, buffer_size);
	if (name)
		return (name);
	name = key_name_literal_nav(key);
	if (name)
		return (name);
	name = key_name_literal_modifiers(key);
	if (name)
		return (name);
	name = key_name_literal_punct(key);
	if (name)
		return (name);
	write_prefixed_int(buffer, buffer_size, "KEY", (int)key);
	return (buffer);
}

static void	cancel_rebind_and_redraw(t_game *game)
{
	config_controls_cancel_rebind(game);
	draw_modal_layout(game);
}

static bool	handle_controller_rebind_key(t_game *game, mlx_key_data_t keydata)
{
	if (game->menu.controls_rebind_column != CONTROLS_COLUMN_CONTROLLER)
		return (false);
	if (keydata.key == MLX_KEY_BACKSPACE)
		cancel_rebind_and_redraw(game);
	return (true);
}

static bool	is_invalid_rebind_target(const t_menu_state *menu)
{
	return (menu->controls_rebind_target < 0
		|| menu->controls_rebind_target >= CONFIG_MODAL_CONTROL_COUNT);
}

static bool	handle_keyboard_rebind_cancel(t_game *game, t_menu_state *menu,
				mlx_key_data_t keydata)
{
	if (keydata.key == MLX_KEY_BACKSPACE || is_invalid_rebind_target(menu))
	{
		cancel_rebind_and_redraw(game);
		return (true);
	}
	return (false);
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
	if (handle_controller_rebind_key(game, keydata))
		return (true);
	if (handle_keyboard_rebind_cancel(game, menu, keydata))
		return (true);
	menu->controls_key_codes[menu->controls_rebind_target] = keydata.key;
	name = key_name_from_mlx(keydata.key, name_buf, sizeof(name_buf));
	ft_strlcpy(menu->controls_key_text[menu->controls_rebind_target], name,
		CONFIG_MODAL_KEY_LABEL_LEN);
	cancel_rebind_and_redraw(game);
	return (true);
}
