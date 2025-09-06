/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 16:24:48 by ivmirand          #+#    #+#             */
/*   Updated: 2025/09/07 00:23:36 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RENDER_H
# define RENDER_H

# include "MLX42/MLX42.h"
# include "MLX42/MLX42_Int.h"
# include "structs.h"

# define MAX_WINDOW_WIDTH 1920
# define MAX_WINDOW_HEIGHT 1080
# define MINIMAP_WIDTH 512
# define MINIMAP_HEIGHT 512

# define RED 0xFF0000FF
# define GREEN 0x00FF00FF
# define BLUE 0x0000FFFF
# define YELLOW 0xFFFF00FF
# define WHITE 0xFFFFFFFF
# define BLACK 0x000000FF

/*--------------------------------- WINDOW.C ---------------------------------*/
bool	window_init(t_game *game);
void	render_bg(t_game *game);
void	render_atmospheric_perspective(t_game *game);
void	render_minimap(t_game *game);
void	window_free(t_game *game);

/*--------------------------------- UTILS.C ----------------------------------*/
int		t_color_to_int(t_color *color, int alpha);
#endif
