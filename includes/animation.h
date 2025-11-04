/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   animation.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 22:23:57 by ivmirand          #+#    #+#             */
/*   Updated: 2025/10/31 02:16:10 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ANIMATION_H
# define ANIMATION_H

#include <math.h>

/*----------------------------- PLAYER_ANIM.C --------------------------------*/
void	idle_right_hand(t_game *game, int[2] start_pos, xpm_t **frames);
void	idle_left_hand(t_game *game, int[2] start_pos, xpm_t **frames);
#endif
