/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   double_buffer.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 17:32:32 by ivmirand          #+#    #+#             */
/*   Updated: 2025/10/29 21:00:54 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

void	render_double_buffer(t_game *game)
{
	uint8_t	*temp_pixels;

	mlx_resize_image(game->double_buffer[NEXT],
			game->double_buffer[CURRENT]->width * game->resolution_scale,
			game->double_buffer[CURRENT]->height * game->resolution_scale);
	render_gameplay_window(game);
	mlx_resize_image(game->double_buffer[NEXT],
			game->double_buffer[CURRENT]->width,
			game->double_buffer[CURRENT]->height);
	temp_pixels = game->double_buffer[NEXT]->pixels;
	game->double_buffer[NEXT]->pixels = game->double_buffer[CURRENT]->pixels;
	game->double_buffer[CURRENT]->pixels = temp_pixels;
	render_minimap_bg(&game->minimap);
	render_minimap_player_vision(&game->minimap);
	render_minimap_tiles(&game->cub_data.map, &game->minimap);
	render_minimap_player(&game->minimap);
}
