/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_modal_draw_base_bonus.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/06 15:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/01/21 21:35:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config_bonus.h"

#include "structs.h"

#include "logic_bonus.h"

t_rect	rect_make(int x, int y, int w, int h)
{
	t_rect	rect;

	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;
	return (rect);
}

void	set_image_enabled(mlx_image_t *img, bool enabled)
{
	size_t	i;

	if (!img)
		return ;
	i = 0;
	while (i < img->count)
	{
		img->instances[i].enabled = enabled;
		i++;
	}
}
