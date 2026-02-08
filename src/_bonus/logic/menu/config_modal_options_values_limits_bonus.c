/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_modal_options_values_limits_bonus.c         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 05:40:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/08 05:40:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config_bonus.h"
#include "structs.h"
#include "logic_bonus.h"

static int	clamp_value(int value, int min, int max)
{
	if (value < min)
		return (min);
	if (value > max)
		return (max);
	return (value);
}

static float	config_quality_scale(int index)
{
	static const float	scales[CONFIG_QUALITY_COUNT] = {
		0.2f, 0.35f, 0.42f, 0.5f
	};

	index = clamp_value(index, 0, CONFIG_QUALITY_COUNT - 1);
	return (scales[index]);
}

int	config_fps_limit_value(int index)
{
	static const int	limits[6] = {10, 30, 60, 120, 240, -1};

	if (index < 0 || index >= (int)(sizeof(limits) / sizeof(limits[0])))
		return (-1);
	return (limits[index]);
}

float	config_mouse_sens_value(int raw)
{
	raw = clamp_value(raw, 1, 100);
	return ((float)raw / 10000.0f);
}

float	config_quality_scale_value_bonus(int index)
{
	return (config_quality_scale(index));
}
