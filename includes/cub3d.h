/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 00:20:30 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/06 17:50:20 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include <stdio.h>
# include "structs.h"
# include "parser.h"
# include "render.h"

// Funciones principales del juego
int	init_game(t_game *game, const char *map_file);
void	cleanup_game(t_game *game);
int	run_game(t_game *game);

#endif
