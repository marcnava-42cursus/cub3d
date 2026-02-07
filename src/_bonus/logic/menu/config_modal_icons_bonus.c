/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_modal_icons_bonus.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/06 15:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/07 23:53:16 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config_bonus.h"
#include "structs.h"
#include "logic_bonus.h"

static void	draw_gear_ring(mlx_image_t *img, t_icon icon, int outer, int inner)
{
	int	dx;
	int	dy;
	int	dist_sq;

	dy = -outer;
	while (dy <= outer)
	{
		dx = -outer;
		while (dx <= outer)
		{
			dist_sq = dx * dx + dy * dy;
			if (dist_sq <= outer * outer && dist_sq >= inner * inner)
				draw_rect(img, rect_make(icon.cx + dx, icon.cy + dy, 1, 1),
					icon.color);
			dx++;
		}
		dy++;
	}
}

static void	draw_controls_icon_marks(mlx_image_t *img, t_rect body,
	int w, int h)
{
	draw_rect(img, rect_make(body.x + w / 4 - h / 6,
			body.y + h / 2 - h / 6, h / 3, h / 3), 0x000000FF);
	draw_rect(img, rect_make(body.x + w / 4 - h / 6,
			body.y + h / 2 - h / 18, h / 3, h / 9), 0x000000FF);
	draw_rect(img, rect_make(body.x + w / 4 - h / 18,
			body.y + h / 2 - h / 6, h / 9, h / 3), 0x000000FF);
	draw_rect(img, rect_make(body.x + 3 * w / 4, body.y + h / 2 - h / 5,
			h / 5, h / 5), 0x000000FF);
	draw_rect(img, rect_make(body.x + 3 * w / 4 - (h / 5) * 3 / 2,
			body.y + h / 2 + (h / 5) / 2, h / 5, h / 5), 0x000000FF);
}

void	draw_settings_icon(mlx_image_t *img, t_icon icon)
{
	int	outer;
	int	inner;
	int	tooth;
	int	core;

	if (!img || icon.size <= 0)
		return ;
	outer = icon.size / 2;
	inner = icon.size / 4;
	tooth = outer / 2;
	core = inner / 2;
	draw_gear_ring(img, icon, outer, inner);
	draw_rect(img, rect_make(icon.cx - tooth / 2, icon.cy - outer - 2,
			tooth, icon.size / 4), icon.color);
	draw_rect(img, rect_make(icon.cx - tooth / 2, icon.cy + outer - 2,
			tooth, icon.size / 4), icon.color);
	draw_rect(img, rect_make(icon.cx - outer - 2, icon.cy - tooth / 2,
			icon.size / 4, tooth), icon.color);
	draw_rect(img, rect_make(icon.cx + outer - 2, icon.cy - tooth / 2,
			icon.size / 4, tooth), icon.color);
	draw_rect(img, rect_make(icon.cx - core / 2, icon.cy - core / 2,
			core, core), icon.color);
}

void	draw_controls_icon(mlx_image_t *img, t_icon icon)
{
	int		w;
	int		h;
	t_rect	body;

	if (!img || icon.size <= 0)
		return ;
	w = icon.size;
	h = (icon.size * 2) / 3;
	body = rect_make(icon.cx - w / 2, icon.cy - h / 2, w, h);
	draw_rect(img, body, icon.color);
	draw_rect(img, rect_make(body.x - w / 6, body.y + h / 4,
			w / 6, h / 2), icon.color);
	draw_rect(img, rect_make(body.x + w, body.y + h / 4,
			w / 6, h / 2), icon.color);
	draw_controls_icon_marks(img, body, w, h);
}
