/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   animation.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 22:23:57 by ivmirand          #+#    #+#             */
/*   Updated: 2026/01/04 15:44:27 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ANIMATION_H
# define ANIMATION_H

# include "MLX42/MLX42.h"
# include "MLX42/MLX42_Int.h"
# include "structs.h"
# include "render.h"
# include "libft.h"

# define FPS 12.0f

/*-------------------------------- ANIM.C ------------------------------------*/
void	get_next_frame(t_anim *anim, int increment);
void	anim_update(t_anim *anim, float delta_time);
void	anim_init(t_anim *anim, const unsigned int *frames,
			const unsigned int *holds, bool loop, unsigned int fps);

/*----------------------------- PLAYER_ANIM.C --------------------------------*/
void	init_empty_left_hand(t_game *game, int start_pos[2], xpm_t **frames);
#endif
