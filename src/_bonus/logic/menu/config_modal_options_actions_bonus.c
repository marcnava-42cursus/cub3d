/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_modal_options_actions_bonus.c               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/06 15:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/01/21 21:16:01 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config_bonus.h"
#include "structs.h"
#include "logic.h"
#include <stdio.h>

static bool	*toggle_value_ptr(t_game *game, int index)
{
	if (!game)
		return (NULL);
	if (index == CONFIG_OPTION_SHOW_FPS)
		return (&game->menu.options.show_fps);
	if (index == CONFIG_OPTION_CROSSHAIR)
		return (&game->menu.options.crosshair);
	if (index == CONFIG_OPTION_MINIMAP)
		return (&game->menu.options.minimap);
	if (index == CONFIG_OPTION_FULLSCREEN)
		return (&game->menu.options.fullscreen);
	if (index == CONFIG_OPTION_DEBUG_MODE)
		return (&game->menu.options.debug_mode);
	return (NULL);
}

void	config_option_select(t_game *game, int delta)
{
	int				selected;
	t_menu_state	*menu;

	if (!game)
		return ;
	menu = &game->menu;
	selected = menu->options.selected + delta;
	if (selected < 0)
		selected = CONFIG_MODAL_OPTION_COUNT - 1;
	else if (selected >= CONFIG_MODAL_OPTION_COUNT)
		selected = 0;
	menu->options.selected = selected;
	draw_modal_layout(game);
}

void	config_option_toggle(t_game *game, int index)
{
	int				slider;
	const char		*name;
	t_menu_state	*menu;
	bool			*toggle;
	char			value_text[CONFIG_MODAL_SLIDER_VALUE_LEN];

	if (!game || index < 0 || index >= CONFIG_MODAL_OPTION_COUNT)
		return ;
	menu = &game->menu;
	menu->options.selected = index;
	if (index < CONFIG_MODAL_TOGGLE_COUNT)
	{
		toggle = toggle_value_ptr(game, index);
		if (!toggle)
			return ;
		if (index == CONFIG_OPTION_SHOW_FPS)
			name = "FPS";
		else if (index == CONFIG_OPTION_CROSSHAIR)
			name = "Crosshair";
		else if (index == CONFIG_OPTION_MINIMAP)
			name = "Minimap";
		else if (index == CONFIG_OPTION_DEBUG_MODE)
			name = "Debug Mode";
		else
			name = "Fullscreen";
		*toggle = !(*toggle);
		if (index == CONFIG_OPTION_CROSSHAIR && game->crosshair)
			set_image_enabled(game->crosshair, *toggle);
		if (*toggle)
			printf("Config: %s ON\n", name);
		else
			printf("Config: %s OFF\n", name);
	}
	else
	{
		slider = index - CONFIG_MODAL_TOGGLE_COUNT;
		if (menu->pending_slider_index == index
			&& menu->pending_slider_value >= 0)
			config_option_set_slider_percent(game, slider,
				menu->pending_slider_value);
		else
			config_option_step_slider(game, slider, 1);
		menu->pending_slider_index = -1;
		menu->pending_slider_value = -1;
		if (index == CONFIG_OPTION_GAME_SPEED)
			name = "Game Speed";
		else if (index == CONFIG_OPTION_FPS_LIMIT)
			name = "FPS Limit";
		else if (index == CONFIG_OPTION_PROJECTILE_SPEED)
			name = "Projectile Speed";
		else
			name = "Mouse Sensitivity";
		config_option_slider_text(game, index, value_text,
			sizeof(value_text));
		printf("Config: %s %s\n", name, value_text);
	}
	draw_modal_layout(game);
}

void	config_option_adjust(t_game *game, int delta)
{
	int				index;
	int				slider;
	t_menu_state	*menu;

	if (!game)
		return ;
	menu = &game->menu;
	index = menu->options.selected;
	if (index < CONFIG_MODAL_TOGGLE_COUNT
		|| index >= CONFIG_MODAL_OPTION_COUNT)
		return ;
	slider = index - CONFIG_MODAL_TOGGLE_COUNT;
	config_option_step_slider(game, slider, delta);
	menu->pending_slider_index = -1;
	menu->pending_slider_value = -1;
	draw_modal_layout(game);
}
