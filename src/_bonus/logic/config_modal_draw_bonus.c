/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_modal_draw_bonus.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/06 15:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/01/06 15:00:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config_modal_bonus.h"

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

static bool	clamp_rect(mlx_image_t *img, t_rect *rect)
{
	if (!img || rect->w <= 0 || rect->h <= 0)
		return (false);
	if (rect->x < 0)
	{
		rect->w += rect->x;
		rect->x = 0;
	}
	if (rect->y < 0)
	{
		rect->h += rect->y;
		rect->y = 0;
	}
	if (rect->x >= (int)img->width || rect->y >= (int)img->height)
		return (false);
	if (rect->x + rect->w > (int)img->width)
		rect->w = (int)img->width - rect->x;
	if (rect->y + rect->h > (int)img->height)
		rect->h = (int)img->height - rect->y;
	return (rect->w > 0 && rect->h > 0);
}

static void	fill_row(uint8_t *row, uint8_t *channels, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		ft_memcpy(row, channels, 4);
		row += 4;
		i++;
	}
}

void	draw_rect(mlx_image_t *img, t_rect rect, uint32_t color)
{
	int		yy;
	uint8_t	*row;
	uint8_t	channels[4];

	if (!clamp_rect(img, &rect))
		return ;
	channels[0] = (uint8_t)(color >> 24);
	channels[1] = (uint8_t)(color >> 16);
	channels[2] = (uint8_t)(color >> 8);
	channels[3] = (uint8_t)color;
	yy = 0;
	while (yy < rect.h)
	{
		row = &img->pixels[((rect.y + yy) * img->width + rect.x) * 4];
		fill_row(row, channels, rect.w);
		yy++;
	}
}
