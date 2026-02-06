/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   living_block_anims.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 20:24:26 by ivmirand          #+#    #+#             */
/*   Updated: 2026/02/06 19:20:24 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "animation.h"

void	init_living_block_anims(t_living_block *block)
{
	static const unsigned int	create_frames[9] = {7, 8, 9, 10, 11, 12, 13,
		14, 15};
	static const unsigned int	create_holds[9] = {1, 1, 1, 1, 1, 1, 1, 1, 1};
	static const unsigned int	breathe_frames[10] = {0, 1, 2, 3, 4, 6, 2, 5,
		2, 5};
	static const unsigned int	breathe_holds[10] = {4, 1, 2, 3, 6, 1, 1, 1, 1,
		2};

	block->anims = ft_calloc(2, sizeof(t_anim));
	if (!block->anims)
		return ;
	anim_init(&block->anims[ANIM_CREATE], &block->atlas, false);
	store_anim_frame_data(&block->anims[ANIM_CREATE], create_frames,
		create_holds, 9);
	anim_init(&block->anims[ANIM_BREATHE], &block->atlas, true);
	store_anim_frame_data(&block->anims[ANIM_BREATHE], breathe_frames,
		breathe_holds, 10);
}

void	update_living_block_anims(t_living_block *block, float delta_time)
{
	bool	create_finished;

	create_finished = false;
	anim_update(&block->anims[ANIM_BREATHE], delta_time);
	if (block->is_creating)
	{
		create_finished = anim_update(&block->anims[ANIM_CREATE], delta_time);
		if (create_finished)
			block->is_creating = false;
	}
}

void	free_living_block_anims(t_living_block *block)
{
	free(block->anims);
}
