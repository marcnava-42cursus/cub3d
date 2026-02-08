/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_modal_draw_rect_fill_bonus.c                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 05:26:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/08 05:26:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config_bonus.h"
#include "structs.h"
#include "logic_bonus.h"

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

static void	color_to_channels(uint32_t color, uint8_t out[4])
{
	out[0] = (uint8_t)(color >> 24);
	out[1] = (uint8_t)(color >> 16);
	out[2] = (uint8_t)(color >> 8);
	out[3] = (uint8_t)color;
}

void	draw_rect(mlx_image_t *img, t_rect rect, uint32_t color)
{
	int		yy;
	uint8_t	*row;
	uint8_t	channels[4];

	if (!clamp_rect(img, &rect))
		return ;
	color_to_channels(color, channels);
	yy = 0;
	while (yy < rect.h)
	{
		row = &img->pixels[((rect.y + yy) * img->width + rect.x) * 4];
		fill_row(row, channels, rect.w);
		yy++;
	}
}
