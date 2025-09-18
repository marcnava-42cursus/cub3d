/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 23:31:07 by ivmirand          #+#    #+#             */
/*   Updated: 2025/09/18 12:49:08 by ivmirand         ###   ########.fr       */
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
	const float TAU = 6.283185307179586f;

	while (angle < 0.0f)
		angle += TAU;
	while (angle >= TAU)
		angle -= TAU;
	return (angle);
}
