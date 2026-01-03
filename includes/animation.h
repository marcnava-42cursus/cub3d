/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   animation.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 22:23:57 by ivmirand          #+#    #+#             */
/*   Updated: 2026/01/02 11:21:54 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ANIMATION_H
# define ANIMATION_H

/*-------------------------------- ANIM.C ------------------------------------*/
void	anim_update(t_anim *anim, t_atlas *atlas, float delta_time);
void	anim_init(t_anim *anim, const unsigned int *frames,
			const unsigned int *holds, bool loop, unsigned int fps);

/*----------------------------- PLAYER_ANIM.C --------------------------------*/
void	idle_right_hand(t_game *game, int start_pos[2], xpm_t **frames);
void	idle_left_hand(t_game *game, int start_pos[2], xpm_t **frames);
#endif
