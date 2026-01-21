/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_modal_border_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/06 15:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/01/06 15:00:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config_bonus.h"
#include "structs.h"
#include "logic.h"

void	draw_border(mlx_image_t *img, t_border border)
{
	t_rect	rect;

	if (border.thickness <= 0)
		return ;
	rect = border.area;
	rect.h = border.thickness;
	draw_rect(img, rect, border.color);
	rect.y = border.area.y + border.area.h - border.thickness;
	draw_rect(img, rect, border.color);
	rect = border.area;
	rect.w = border.thickness;
	draw_rect(img, rect, border.color);
	rect.x = border.area.x + border.area.w - border.thickness;
	draw_rect(img, rect, border.color);
}

void	draw_bevel_border(mlx_image_t *img, t_border border, uint32_t light,
			uint32_t dark)
{
	t_rect	rect;

	if (border.thickness <= 0)
		return ;
	rect = border.area;
	rect.h = border.thickness;
	draw_rect(img, rect, light);
	rect = border.area;
	rect.w = border.thickness;
	draw_rect(img, rect, light);
	rect = border.area;
	rect.y = border.area.y + border.area.h - border.thickness;
	rect.h = border.thickness;
	draw_rect(img, rect, dark);
	rect = border.area;
	rect.x = border.area.x + border.area.w - border.thickness;
	rect.w = border.thickness;
	draw_rect(img, rect, dark);
}

static void	draw_progress_edge(mlx_image_t *img, t_border border, int edge,
			int index)
{
	t_rect	rect;

	rect = border.area;
	if (index % 2 == 0)
	{
		if (index == 2)
			rect.x += border.area.w - edge;
		rect.y += (index / 2) * (border.area.h - border.thickness);
		rect.w = edge;
		rect.h = border.thickness;
	}
	else
	{
		if (index == 1)
			rect.x += border.area.w - border.thickness;
		else
			rect.y += border.area.h - edge;
		rect.w = border.thickness;
		rect.h = edge;
	}
	draw_rect(img, rect, border.color);
}

void	draw_progress_border(mlx_image_t *img, t_border border, float progress)
{
	float	remaining;
	int		edge;
	int		index;
	int		limit;

	index = 0;
	if (!img || border.thickness <= 0 || border.area.w <= 0
		|| border.area.h <= 0 || progress <= 0.0f)
		return ;
	remaining = progress * (float)(2 * (border.area.w + border.area.h));
	while (index < 4 && remaining > 0.0f)
	{
		limit = border.area.w;
		if (index % 2 == 1)
			limit = border.area.h;
		edge = (int)remaining;
		if (edge > limit)
			edge = limit;
		draw_progress_edge(img, border, edge, index);
		remaining -= edge;
		index++;
	}
}
