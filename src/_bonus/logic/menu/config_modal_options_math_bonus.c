/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_modal_options_math_bonus.c                  :+:      :+:    :+:   */
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

static int	clamp_value(int value, int min, int max)
{
	if (value < min)
		return (min);
	if (value > max)
		return (max);
	return (value);
}

void	config_option_set_slider_percent(t_game *game, int slider, int percent)
{
	int	value;

	percent = clamp_value(percent, 0, 100);
	if (slider == CONFIG_SLIDER_FPS_LIMIT)
	{
		value = (percent * (6 - 1) + 50) / 100;
		config_option_set_slider_raw(game, slider,
			clamp_value(value, 0, 6 - 1));
		return ;
	}
	if (slider == CONFIG_SLIDER_QUALITY)
	{
		value = (percent * (CONFIG_QUALITY_COUNT - 1) + 50) / 100;
		config_option_set_slider_raw(game, slider,
			clamp_value(value, 0, CONFIG_QUALITY_COUNT - 1));
		return ;
	}
	value = 1 + (percent * 99 + 50) / 100;
	config_option_set_slider_raw(game, slider,
		clamp_value(value, 1, 100));
}

void	config_option_step_slider(t_game *game, int slider, int delta)
{
	int	value;

	value = config_option_slider_raw(game, slider);
	if (slider == CONFIG_SLIDER_FPS_LIMIT)
	{
		value = clamp_value(value + delta, 0, 6 - 1);
		config_option_set_slider_raw(game, slider, value);
		return ;
	}
	if (slider == CONFIG_SLIDER_QUALITY)
	{
		value = clamp_value(value + delta, 0, CONFIG_QUALITY_COUNT - 1);
		config_option_set_slider_raw(game, slider, value);
		return ;
	}
	value = clamp_value(
			value + delta * CONFIG_MODAL_SLIDER_STEP,
			1, 100);
	config_option_set_slider_raw(game, slider, value);
}
