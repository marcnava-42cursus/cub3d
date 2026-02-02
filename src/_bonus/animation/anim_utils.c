/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   anim_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 22:48:35 by ivmirand          #+#    #+#             */
/*   Updated: 2026/02/02 22:50:31 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "animation.h"

void	set_current_anim(t_anim *anim_ptr, int *current_anim, int new_anim)
{
	if (*current_anim == new_anim)
		return ;
	*current_anim = new_anim;
	anim_start(&anim_ptr[*current_anim]);
}
