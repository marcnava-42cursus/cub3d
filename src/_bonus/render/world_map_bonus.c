/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   world_map_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 09:02:52 by ivmirand          #+#    #+#             */
/*   Updated: 2025/12/02 09:23:55 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

static void	clear_image(mlx_image_t *img)
{
	uint32_t	i;

	i = 0;
	while (i < img->width * img->height)
	{
		((uint32_t *)img->pixels)[i] = 0x00000000;
		i++;
	}
}

static void	set_image_enabled(mlx_image_t *img, bool enabled)
{
	size_t	i;

	if (!img)
		return ;
	i = 0;
	while (i < img->count)
	{
		img->instances[i].enabled = enabled;
		i++;
	}
}

bool	world_map_init(t_game *game)
{
	game->map_layer = mlx_new_image(game->mlx, MAX_WINDOW_WIDTH,
			MAX_WINDOW_HEIGHT);
	game->player_layer = mlx_new_image(game->mlx, MAX_WINDOW_WIDTH,
			MAX_WINDOW_HEIGHT);
	if (!game->map_layer || !game->player_layer)
		return (false);
	clear_image(game->player_layer);
	game->last_player_x = -1.0f;
	game->last_player_y = -1.0f;
	game->last_player_angle = 0.0f;
	game->key_w_pressed = false;
	game->key_s_pressed = false;
	game->key_a_pressed = false;
	game->key_d_pressed = false;
	game->key_left_pressed = false;
	game->key_right_pressed = false;
	game->map_layer_attached = false;
	game->player_layer_attached = false;
	return (true);
}

void	set_map_overlay_visible(t_game *game, bool visible)
{
	if (!game || !game->mlx)
		return ;
	if (visible)
	{
		if (!game->map_layer_attached)
		{
			if (mlx_image_to_window(game->mlx, game->map_layer, 0, 0) >= 0)
				game->map_layer_attached = true;
		}
		if (!game->player_layer_attached)
		{
			if (mlx_image_to_window(game->mlx, game->player_layer, 0, 0) >= 0)
				game->player_layer_attached = true;
		}
		set_image_enabled(game->map_layer, true);
		set_image_enabled(game->player_layer, true);
		render_map_2d_initial(game);
	}
	else
	{
		set_image_enabled(game->map_layer, false);
		set_image_enabled(game->player_layer, false);
	}
	game->map_2d_visible = visible;
}

void	toggle_map_overlay(t_game *game)
{
	if (!game)
		return ;
	set_map_overlay_visible(game, !game->map_2d_visible);
}
