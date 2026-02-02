/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   effects_anim.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 22:48:35 by ivmirand          #+#    #+#             */
/*   Updated: 2026/02/02 23:02:01 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "animation.h"

void	free_effects_anims(t_effects *effects)
{
	free(effects->orb_anims);
	free(effects->absorb_anims);
	free(effects->door_anims);
}
