/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 23:31:07 by ivmirand          #+#    #+#             */
/*   Updated: 2025/09/23 02:24:50 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "render.h"

int	t_color_to_int(t_color *color, int alpha)
{
	return (color->r << 24 | color->g << 16 | color->b << 8 | alpha);
}

int	clamp(int value, int min, int max)
{
	if (value < min)
		return (min);
	else if (value > max)
		return (max);
	else
		return (value);
}

float	normalize_angle(float angle)
{
	while (angle < 0.0f)
		angle += TAU;
	while (angle >= TAU)
		angle -= TAU;
	return (angle);
}

//y[0] is start and y[1] is end
void paint_vertical_line(int x, int y[2], mlx_image_t *img, int color)
{
	int	current_y;
	
	if (x < 0 || x >= (int)img->width)
		return ;
	if (y[0] < 0)
		y[0] = 0;
	if (y[1] >= (int)img->height)
		y[1] = (int)img->height - 1;
	if (y[0] >= y[1])
		return ;
	current_y = y[0];
	while (current_y <= y[1])
	{
		mlx_put_pixel(img, x, current_y, color);
		current_y++;
	}
}
