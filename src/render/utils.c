/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 23:31:07 by ivmirand          #+#    #+#             */
/*   Updated: 2025/09/06 23:31:32 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "render.h"

int		t_color_to_int(t_color *color, int alpha)
{
	return (color->r << 24 | color->g << 16 | color->b << 8 | alpha);
}
