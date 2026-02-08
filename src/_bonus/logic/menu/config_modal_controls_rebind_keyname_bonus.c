/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_modal_controls_rebind_keyname_bonus.c       :+:      :+:    :+:   */
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

void	config_write_prefixed_int_bonus(char *buffer, size_t buffer_size,
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

const char	*config_key_name_literal_nav_bonus(keys_t key)
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

const char	*config_key_name_literal_modifiers_bonus(keys_t key)
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

const char	*config_key_name_literal_punct_bonus(keys_t key)
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

const char	*config_key_name_alnum_or_fn_bonus(keys_t key, char *buffer,
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
		config_write_prefixed_int_bonus(buffer, buffer_size,
			"F", 1 + (key - MLX_KEY_F1));
		return (buffer);
	}
	return (NULL);
}
