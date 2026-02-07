/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_modal_options_values_bonus.c                :+:      :+:    :+:   */
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

static void	write_int_value(char *buffer, size_t buffer_size, int value)
{
	char	*num;

	if (!buffer || buffer_size == 0)
		return ;
	buffer[0] = '\0';
	num = ft_itoa(value);
	if (!num)
		return ;
	ft_strlcpy(buffer, num, buffer_size);
	free(num);
}

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

static const char	*config_quality_label(int index)
{
	static const char	*labels[CONFIG_QUALITY_COUNT] = {
		"LOW", "MEDIUM", "HIGH", "ULTRA"
	};

	index = clamp_value(index, 0, CONFIG_QUALITY_COUNT - 1);
	return (labels[index]);
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
		game->resolution_scale = config_quality_scale(value);
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

	if (index < CONFIG_MODAL_TOGGLE_COUNT
		|| index >= CONFIG_MODAL_OPTION_COUNT)
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

void	config_option_slider_text(t_game *game, int index, char *buffer,
			size_t buffer_size)
{
	int	slider;
	int	value;
	int	raw;

	if (!buffer || buffer_size == 0)
		return ;
	buffer[0] = '\0';
	if (index < CONFIG_MODAL_TOGGLE_COUNT
		|| index >= CONFIG_MODAL_OPTION_COUNT)
		return ;
	slider = index - CONFIG_MODAL_TOGGLE_COUNT;
	raw = config_option_slider_raw(game, slider);
	if (slider == CONFIG_SLIDER_FPS_LIMIT)
	{
		if (raw < 0)
			raw = 0;
		if (raw > 5)
			raw = 5;
		value = config_fps_limit_value(raw);
		if (value < 0)
			ft_strlcpy(buffer, "UNLIMITED", buffer_size);
		else
			write_int_value(buffer, buffer_size, value);
		return ;
	}
	if (slider == CONFIG_SLIDER_QUALITY)
	{
		ft_strlcpy(buffer, config_quality_label(raw), buffer_size);
		return ;
	}
	write_int_value(buffer, buffer_size, raw);
}
