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
#include "logic_bonus.h"
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

static bool	get_section_option_range(t_game *game, int *min, int *max)
{
	if (!game || !min || !max)
		return (false);
	if (game->menu.current_tab == CONFIG_MENU_GENERAL)
	{
		*min = 0;
		*max = CONFIG_MODAL_TOGGLE_COUNT - 1;
		return (true);
	}
	if (game->menu.current_tab == CONFIG_MENU_TUNING)
	{
		*min = CONFIG_MODAL_TOGGLE_COUNT;
		*max = CONFIG_MODAL_OPTION_COUNT - 1;
		return (true);
	}
	return (false);
}

void	config_option_select(t_game *game, int delta)
{
	int				selected;
	int				min;
	int				max;
	t_menu_state	*menu;

	if (!game)
		return ;
	if (!get_section_option_range(game, &min, &max))
		return ;
	menu = &game->menu;
	if (menu->options.selected < min || menu->options.selected > max)
	{
		menu->options.selected = min;
		draw_modal_layout(game);
		return ;
	}
	selected = menu->options.selected + delta;
	if (selected < min)
		selected = max;
	else if (selected > max)
		selected = min;
	menu->options.selected = selected;
	draw_modal_layout(game);
}

static const char	*toggle_name(int index)
{
	if (index == CONFIG_OPTION_SHOW_FPS)
		return ("FPS");
	if (index == CONFIG_OPTION_CROSSHAIR)
		return ("Crosshair");
	if (index == CONFIG_OPTION_MINIMAP)
		return ("Minimap");
	if (index == CONFIG_OPTION_DEBUG_MODE)
		return ("Debug Mode");
	return ("Fullscreen");
}

static const char	*slider_name(int index)
{
	if (index == CONFIG_OPTION_FPS_LIMIT)
		return ("FPS Limit");
	if (index == CONFIG_OPTION_MOUSE_SENS)
		return ("Mouse Sensitivity");
	if (index == CONFIG_OPTION_QUALITY)
		return ("Quality");
	return ("Slider");
}

static void	toggle_option_value(t_game *game, int index, bool *toggle)
{
	*toggle = !(*toggle);
	if (index == CONFIG_OPTION_CROSSHAIR && game->crosshair)
		set_image_enabled(game->crosshair, *toggle);
	if (*toggle)
		printf("Config: %s ON\n", toggle_name(index));
	else
		printf("Config: %s OFF\n", toggle_name(index));
}

static void	adjust_slider_option(t_game *game, int index, t_menu_state *menu)
{
	int		slider;
	char	value_text[CONFIG_MODAL_SLIDER_VALUE_LEN];

	slider = index - CONFIG_MODAL_TOGGLE_COUNT;
	if (menu->pending_slider_index == index && menu->pending_slider_value >= 0)
		config_option_set_slider_percent(game, slider, menu->pending_slider_value);
	else
		config_option_step_slider(game, slider, 1);
	menu->pending_slider_index = -1;
	menu->pending_slider_value = -1;
	config_option_slider_text(game, index, value_text, sizeof(value_text));
	printf("Config: %s %s\n", slider_name(index), value_text);
}

void	config_option_toggle(t_game *game, int index)
{
	t_menu_state	*menu;
	bool			*toggle;

	if (!game || index < 0 || index >= CONFIG_MODAL_OPTION_COUNT)
		return ;
	menu = &game->menu;
	menu->options.selected = index;
	if (index < CONFIG_MODAL_TOGGLE_COUNT)
	{
		toggle = toggle_value_ptr(game, index);
		if (!toggle)
			return ;
		toggle_option_value(game, index, toggle);
	}
	else
		adjust_slider_option(game, index, menu);
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
