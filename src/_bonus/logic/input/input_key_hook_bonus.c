/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_key_hook_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 07:05:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/08 07:05:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config_bonus.h"
#include "structs.h"
#include "logic_bonus.h"

static bool	handle_modal_key_input(t_game *game, mlx_key_data_t keydata)
{
	keys_t	menu_key;

	if (!game)
		return (true);
	if (is_config_modal_open(game) && game->menu.controls_rebinding)
	{
		config_modal_handle_key(game, keydata);
		return (true);
	}
	menu_key = game->menu.controls_key_codes[ACTION_MENU];
	if (keydata.key == menu_key && keydata.action == MLX_PRESS)
	{
		toggle_config_modal(game);
		return (true);
	}
	if (is_config_modal_open(game))
	{
		config_modal_handle_key(game, keydata);
		return (true);
	}
	return (false);
}

static void	handle_place_break_input(t_game *game, mlx_key_data_t keydata)
{
	keys_t	break_key;
	keys_t	place_key;

	place_key = game->menu.controls_key_codes[ACTION_PLACE];
	if (keydata.key == place_key && keydata.action == MLX_PRESS)
	{
		place_breakable_block(game);
		return ;
	}
	break_key = game->menu.controls_key_codes[ACTION_BREAK];
	if (keydata.key == break_key && keydata.action == MLX_PRESS)
		test_break_wall_in_front(game);
}

void	key_hook_bonus(mlx_key_data_t keydata, void *param)
{
	t_game	*game;

	game = (t_game *)param;
	if (!game)
		return ;
	if (handle_modal_key_input(game, keydata))
		return ;
	if (keydata.key == MLX_KEY_SPACE && keydata.action == MLX_PRESS)
	{
		trigger_jump_bonus(game);
		return ;
	}
	handle_place_break_input(game, keydata);
}
