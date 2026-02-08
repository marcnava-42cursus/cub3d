/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_modal_options_init_bonus.c                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/06 15:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/07 23:35:46 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config_bonus.h"
#include "structs.h"
#include "logic_bonus.h"

static void	init_menu_defaults(t_menu_state *menu)
{
	menu->options.show_fps = false;
	menu->options.crosshair = true;
	menu->options.minimap = true;
	menu->options.fullscreen = false;
	menu->options.debug_mode = false;
	menu->options.fps_limit_index = 2;
	menu->options.mouse_sens = 50;
	menu->options.quality_index = CONFIG_QUALITY_MEDIUM;
	menu->options.selected = 0;
	menu->current_column = CONFIG_MENU_COLUMN_LEFT;
	menu->pending_slider_index = -1;
	menu->pending_slider_value = -1;
	menu->slider_drag_index = -1;
	menu->controls_selected = 0;
	menu->controls_column = CONTROLS_COLUMN_KEYBOARD;
	menu->controls_rebind_column = -1;
	menu->controls_rebinding = false;
	menu->controls_rebind_target = -1;
}

static void	init_default_controls(t_menu_state *menu,
				const keys_t *default_keys, const char **defaults)
{
	int	i;

	i = 0;
	while (i < CONFIG_MODAL_CONTROL_COUNT)
	{
		menu->controls_key_codes[i] = default_keys[i];
		ft_strlcpy(menu->controls_key_text[i], defaults[i],
			CONFIG_MODAL_KEY_LABEL_LEN);
		i++;
	}
}

void	config_options_init(t_game *game)
{
	static const keys_t	default_keys[CONFIG_MODAL_CONTROL_COUNT] = {
		MLX_KEY_W, MLX_KEY_S, MLX_KEY_D, MLX_KEY_A,
		MLX_KEY_RIGHT, MLX_KEY_LEFT, MLX_KEY_UP, MLX_KEY_DOWN,
		MLX_KEY_E, MLX_KEY_R, MLX_KEY_ESCAPE, MLX_KEY_M,
		MLX_KEY_ENTER, MLX_KEY_Q
	};
	static const char	*defaults[CONFIG_MODAL_CONTROL_COUNT] = {
		"W", "S", "D", "A", "RIGHT", "LEFT", "UP", "DOWN",
		"E", "R", "ESC", "M", "ENTER", "Q"
	};
	t_menu_state		*menu;

	if (!game)
		return ;
	menu = &game->menu;
	init_menu_defaults(menu);
	init_default_controls(menu, default_keys, defaults);
	config_option_set_slider_raw(game, CONFIG_SLIDER_MOUSE_SENS,
		menu->options.mouse_sens);
	config_option_set_slider_raw(game, CONFIG_SLIDER_QUALITY,
		menu->options.quality_index);
	ft_bzero(&menu->labels, sizeof(menu->labels));
	controller_init_bonus(game);
}

void	config_options_reset(t_game *game)
{
	t_menu_state	*menu;

	if (!game)
		return ;
	menu = &game->menu;
	menu->options.selected = 0;
	menu->current_column = CONFIG_MENU_COLUMN_LEFT;
	menu->controls_selected = 0;
	menu->controls_column = CONTROLS_COLUMN_KEYBOARD;
	menu->controls_rebind_column = -1;
	menu->controls_rebinding = false;
	menu->controls_rebind_target = -1;
	menu->pending_slider_index = -1;
	menu->pending_slider_value = -1;
	menu->slider_drag_index = -1;
}
