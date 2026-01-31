/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   animation.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 22:23:57 by ivmirand          #+#    #+#             */
/*   Updated: 2026/01/31 19:24:17 by ivmirand         ###   ########.fr       */
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

typedef enum e_player_anim
{
	ANIM_EMPTY = 0,
	ANIM_TAKE = 1,
	ANIM_HOLD = 2,
	ANIM_THROW = 3
}	t_player_anim;

typedef enum e_living_block_anim
{
	ANIM_CREATE = 0,
	ANIM_BREATHE = 1
}	t_living_block_anim;

typedef enum e_effects_anim
{
	ANIM_ABSORB = 0,
	ANIM_SPAWN = 1
}	t_effects_anim;

/*-------------------------------- ANIM.C ------------------------------------*/
void	anim_init(t_anim *anim, t_atlas *atlas, bool loop);
void	store_anim_frame_data(t_anim *anim, const unsigned int *frames,
			const unsigned int *holds, unsigned int count);
void	anim_start(t_anim *anim);
bool	anim_update(t_anim *anim, float delta_time);

/*----------------------------- PLAYER_ANIM.C --------------------------------*/
void	set_player_anim(t_anim *anim_ptr, int *current_anim, int new_anim);
void	init_player_anims(t_player *player);
void	update_player_anims(t_player *player, float delta_time);
void	free_player_anims(t_player *player);

/*------------------------ PLAYER_LEFT_HAND_ANIMS.C --------------------------*/
void	set_player_left_hand_anims(t_player *player);
void	init_player_left_hand_anims(t_player *player);

/*----------------------- PLAYER_RIGHT_HAND_ANIMS.C --------------------------*/
void	set_player_right_hand_anims(t_player *player);
void	init_player_right_hand_anims(t_player *player);

/*-------------------------- LIVING_BLOCK_ANIMS.C ----------------------------*/
void	init_living_block_anims(t_living_block *block, t_atlas *atlas);
void	update_living_block_anims(t_living_block *block, float delta_time);
void	free_living_block_anims(t_living_block *block);

/*----------------------------- ABSORB_ANIMS.C -------------------------------*/
void	init_absorb_anims(t_effects *effects);
void	update_absorb_anims(t_player *player, t_effects *effects,
		float delta_time);
void	free_effects_anims(t_anim *effects);
#endif
