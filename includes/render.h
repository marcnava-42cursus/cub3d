/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 16:24:48 by ivmirand          #+#    #+#             */
/*   Updated: 2025/09/07 02:21:41 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RENDER_H
# define RENDER_H

# include "MLX42/MLX42.h"
# include "MLX42/MLX42_Int.h"
# include "structs.h"

# define MAX_WINDOW_WIDTH 1920
# define MAX_WINDOW_HEIGHT 1080
# define MINIMAP_WIDTH 256
# define MINIMAP_HEIGHT 256
# define MINIMAP_TILE_SIZE 16

# define RED 0xFF0000FF
# define GREEN 0x00FF00FF
# define BLUE 0x0000FFFF
# define YELLOW 0xFFFF00FF
# define WHITE 0xFFFFFFFF
# define BLACK 0x000000FF

/*-------------------------------- MINIMAP.C ---------------------------------*/
void	minimap_init(t_game *game);
void	render_minimap_bg(mlx_t *mlx, t_minimap *minimap);
void	render_minimap_player(mlx_t *mlx, t_minimap *minimap);
void	minimap_free(mlx_t *mlx, t_minimap *minimap);

/*--------------------------------- WINDOW.C ---------------------------------*/
bool	window_init(t_game *game);
void	render_bg(t_game *game);
void	render_atmospheric_perspective(t_game *game);
void	window_free(t_game *game);

/*--------------------------------- UTILS.C ----------------------------------*/
int		t_color_to_int(t_color *color, int alpha);
#endif
