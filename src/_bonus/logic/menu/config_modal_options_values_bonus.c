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
#include "logic.h"
#include <stdio.h>

int	config_fps_limit_value(int index)
{
	static const int	limits[6] = {10, 30, 60, 120, 240, -1};

	if (index < 0 || index >= (int)(sizeof(limits) / sizeof(limits[0])))
		return (-1);
	return (limits[index]);
}

int	config_option_slider_raw(t_game *game, int slider)
{
	if (!game)
		return (0);
	if (slider == CONFIG_SLIDER_GAME_SPEED)
		return (game->menu.options.game_speed);
	if (slider == CONFIG_SLIDER_FPS_LIMIT)
		return (game->menu.options.fps_limit_index);
	if (slider == CONFIG_SLIDER_MOUSE_SENS)
		return (game->menu.options.mouse_sens);
	if (slider == CONFIG_SLIDER_PROJECTILE_SPEED)
		return (game->menu.options.projectile_speed);
	return (0);
}

void	config_option_set_slider_raw(t_game *game, int slider, int value)
{
	if (!game)
		return ;
	if (slider == CONFIG_SLIDER_GAME_SPEED)
		game->menu.options.game_speed = value;
	else if (slider == CONFIG_SLIDER_FPS_LIMIT)
		game->menu.options.fps_limit_index = value;
	else if (slider == CONFIG_SLIDER_MOUSE_SENS)
		game->menu.options.mouse_sens = value;
	else if (slider == CONFIG_SLIDER_PROJECTILE_SPEED)
		game->menu.options.projectile_speed = value;
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
			snprintf(buffer, buffer_size, "UNLIMITED");
		else
			snprintf(buffer, buffer_size, "%d", value);
		return ;
	}
	snprintf(buffer, buffer_size, "%d", raw);
}
