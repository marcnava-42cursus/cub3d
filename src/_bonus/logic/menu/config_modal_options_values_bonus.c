/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_modal_options_values_bonus.c                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/06 15:00:00 by marcnava          #+#    #+#             */
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

int	config_option_slider_raw(t_game *game, int slider)
{
	if (!game)
		return (0);
	if (slider == CONFIG_SLIDER_FPS_LIMIT)
		return (game->menu.options.fps_limit_index);
	if (slider == CONFIG_SLIDER_MOUSE_SENS)
		return (game->menu.options.mouse_sens);
	if (slider == CONFIG_SLIDER_QUALITY)
		return (game->menu.options.quality_index);
	return (0);
}

void	config_option_set_slider_raw(t_game *game, int slider, int value)
{
	if (!game)
		return ;
	if (slider == CONFIG_SLIDER_FPS_LIMIT)
		game->menu.options.fps_limit_index = value;
	else if (slider == CONFIG_SLIDER_MOUSE_SENS)
	{
		value = clamp_value(value, 1, 100);
		game->menu.options.mouse_sens = value;
		game->mouse_sensitivity = config_mouse_sens_value(value);
	}
	else if (slider == CONFIG_SLIDER_QUALITY)
	{
		value = clamp_value(value, 0, CONFIG_QUALITY_COUNT - 1);
		game->menu.options.quality_index = value;
		game->resolution_scale = config_quality_scale_value_bonus(value);
	}
}

bool	config_option_toggle_state(t_game *game, int index)
{
	if (!game || index < 0 || index >= CONFIG_MODAL_TOGGLE_COUNT)
		return (false);
	if (index == CONFIG_OPTION_SHOW_FPS)
		return (game->menu.options.show_fps);
	if (index == CONFIG_OPTION_CROSSHAIR)
		return (game->menu.options.crosshair);
	if (index == CONFIG_OPTION_MINIMAP)
		return (game->menu.options.minimap);
	if (index == CONFIG_OPTION_FULLSCREEN)
		return (game->menu.options.fullscreen);
	if (index == CONFIG_OPTION_DEBUG_MODE)
		return (game->menu.options.debug_mode);
	return (false);
}

int	config_option_slider_value(t_game *game, int index)
{
	int	slider;
	int	value;

	if (index < CONFIG_MODAL_TOGGLE_COUNT || index >= CONFIG_MODAL_OPTION_COUNT)
		return (0);
	slider = index - CONFIG_MODAL_TOGGLE_COUNT;
	value = config_option_slider_raw(game, slider);
	if (slider == CONFIG_SLIDER_FPS_LIMIT)
		return ((value * 100) / (6 - 1));
	if (slider == CONFIG_SLIDER_QUALITY)
	{
		value = clamp_value(value, 0, CONFIG_QUALITY_COUNT - 1);
		return ((value * 100) / (CONFIG_QUALITY_COUNT - 1));
	}
	return (((value - 1) * 100) / 99);
}
