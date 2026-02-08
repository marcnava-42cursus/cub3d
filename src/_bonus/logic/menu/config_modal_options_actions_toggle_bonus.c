/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_modal_options_actions_toggle_bonus.c        :+:      :+:    :+:   */
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

#include <stdio.h>

bool	*config_toggle_value_ptr_bonus(t_game *game, int index)
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

const char	*config_toggle_name_bonus(int index)
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

void	config_toggle_option_value_bonus(t_game *game, int index, bool *toggle)
{
	*toggle = !(*toggle);
	if (index == CONFIG_OPTION_CROSSHAIR && game->crosshair)
		set_image_enabled(game->crosshair, *toggle);
	if (*toggle)
		printf("Config: %s ON\n", config_toggle_name_bonus(index));
	else
		printf("Config: %s OFF\n", config_toggle_name_bonus(index));
}
