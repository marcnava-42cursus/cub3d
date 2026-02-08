/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_modal_events_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/06 15:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/08 06:04:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config_bonus.h"
#include "structs.h"
#include "logic_bonus.h"

static void	handle_settings_tab_keys(t_game *game, bool confirm_pressed,
			mlx_key_data_t keydata)
{
	if (keydata.key == MLX_KEY_UP)
		config_option_select(game, -1);
	else if (keydata.key == MLX_KEY_DOWN)
		config_option_select(game, 1);
	else if (confirm_pressed)
		config_option_toggle(game, game->menu.options.selected);
	else if (keydata.key == MLX_KEY_A
		&& game->menu.current_tab == CONFIG_MENU_TUNING)
		config_option_adjust(game, -1);
	else if (keydata.key == MLX_KEY_D
		&& game->menu.current_tab == CONFIG_MENU_TUNING)
		config_option_adjust(game, 1);
}

void	config_modal_handle_key(t_game *game, mlx_key_data_t keydata)
{
	keys_t	accept_key;
	bool	confirm_pressed;

	if (!game || !game->menu.open)
		return ;
	if (keydata.action != MLX_PRESS)
		return ;
	if (config_controls_handle_key(game, keydata))
		return ;
	accept_key = game->menu.controls_key_codes[ACTION_ACCEPT];
	confirm_pressed = (keydata.key == accept_key
			|| keydata.key == MLX_KEY_SPACE);
	if (config_modal_handle_column_navigation_bonus(game, keydata))
		return ;
	if (config_modal_handle_left_column_keys_bonus(game,
			confirm_pressed, keydata))
		return ;
	if (config_modal_handle_controls_tab_keys_bonus(game,
			confirm_pressed, keydata))
		return ;
	handle_settings_tab_keys(game, confirm_pressed, keydata);
}
