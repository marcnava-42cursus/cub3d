/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_validation.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 01:55:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/10/07 19:54:26 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/parser.h"

int	validate_rgb_values(int r, int g, int b)
{
	if (r < 0 || r > 255)
		return (0);
	if (g < 0 || g > 255)
		return (0);
	if (b < 0 || b > 255)
		return (0);
	return (1);
}

static int	assign_floor_color(t_color *floor, int r, int g, int b)
{
	if (floor->r != -1)
		return (0);
	floor->r = r;
	floor->g = g;
	floor->b = b;
	return (1);
}

static int	assign_ceiling_color(t_color *ceiling, int r, int g, int b)
{
	if (ceiling->r != -1)
		return (0);
	ceiling->r = r;
	ceiling->g = g;
	ceiling->b = b;
	return (1);
}

int	assign_color(char identifier, t_color *floor, t_color *ceiling, int *rgb)
{
	if (identifier == 'F')
		return (assign_floor_color(floor, rgb[0], rgb[1], rgb[2]));
	else if (identifier == 'C')
		return (assign_ceiling_color(ceiling, rgb[0], rgb[1], rgb[2]));
	return (0);
}
