/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 16:27:46 by ivmirand          #+#    #+#             */
/*   Updated: 2025/10/02 13:44:05 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "render.h"

bool	window_init(t_game *game)
{
	int			window_width;
	int			window_height;

	window_width = game->cub_data.map.width * TILE_SIZE;
	window_height = game->cub_data.map.height * TILE_SIZE;
	game->mlx = mlx_init(window_width, window_height, "cub3D", false);
	if (!game->mlx)
		return (false);
	game->rc_buf_zero = mlx_new_image(game->mlx, window_width,
			window_height);
	game->rc_buf_one = mlx_new_image(game->mlx, window_width,
			window_height);
	game->bg_buf_zero = mlx_new_image(game->mlx, window_width,
			window_height);
	// Crear capas separadas para renderizado 2D
	game->map_layer = mlx_new_image(game->mlx, window_width, window_height);
	game->player_layer = mlx_new_image(game->mlx, window_width, window_height);
	if (!game->map_layer || !game->player_layer)
		return (false);
	// Hacer la capa del jugador transparente inicialmente
	for (unsigned int i = 0; i < window_width * window_height; i++)
		((uint32_t*)game->player_layer->pixels)[i] = 0x00000000;
	// Inicializar variables de seguimiento
	game->last_player_x = -1.0f;
	game->last_player_y = -1.0f;
	game->last_player_angle = 0.0f;
	// Inicializar estado de teclas
	game->key_w_pressed = false;
	game->key_s_pressed = false;
	game->key_a_pressed = false;
	game->key_d_pressed = false;
	game->key_left_pressed = false;
	game->key_right_pressed = false;
	if (mlx_image_to_window(game->mlx, game->bg_buf_zero, 0, 0) < 0)
		return (false);
	game->bg_layer_attached = true;
	if (mlx_image_to_window(game->mlx, game->map_layer, 0, 0) < 0)
		return (false);
	game->map_layer_attached = true;
	if (mlx_image_to_window(game->mlx, game->player_layer, 0, 0) < 0)
		return (false);
	game->player_layer_attached = true;
	return (true);
}

void	render_bg(t_game *game, int sky, int ground)
{
	unsigned int	x;
	unsigned int	y;

	y = 0;
	while (y < game->bg_buf_zero->height / 2)
	{
		x = 0;
		while (x < game->bg_buf_zero->width)
		{
			mlx_put_pixel(game->bg_buf_zero, x, y, sky);
			x++;
		}
		y++;
	}
	while (y < game->bg_buf_zero->height)
	{
		x = 0;
		while (x < game->bg_buf_zero->width)
		{
			mlx_put_pixel(game->bg_buf_zero, x, y, ground);
			x++;
		}
		y++;
	}
}

void	window_free(t_game *game)
{
	free_map_textures(game);
	mlx_delete_image(game->mlx, game->rc_buf_zero);
	mlx_delete_image(game->mlx, game->rc_buf_one);
	mlx_delete_image(game->mlx, game->bg_buf_zero);
	if (game->map_layer)
		mlx_delete_image(game->mlx, game->map_layer);
	if (game->player_layer)
		mlx_delete_image(game->mlx, game->player_layer);
	mlx_terminate(game->mlx);
}

