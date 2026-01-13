/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_modal_options_state_bonus.c                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/06 15:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/01/06 15:00:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config_modal_bonus.h"

static bool	*config_option_ref(t_game *game, int index)
{
	if (!game)
		return (NULL);
	if (index == 0)
		return (&game->config_options.crosshair);
	if (index == 1)
		return (&game->config_options.minimap);
	if (index == 2)
		return (&game->config_options.fullscreen);
	return (NULL);
}

void	config_options_reset(t_game *game)
{
	if (!game)
		return ;
	game->config_options.selected = 0;
}

void	config_option_select(t_game *game, int delta)
{
	int	selected;

	if (!game)
		return ;
	selected = game->config_options.selected + delta;
	if (selected < 0)
		selected = 2;
	else if (selected > 2)
		selected = 0;
	game->config_options.selected = selected;
	draw_modal_layout(game);
}

void	config_option_toggle(t_game *game, int index)
{
	bool			*flag;
	const char		*name;
	const char		*state;

	flag = config_option_ref(game, index);
	if (!flag)
		return ;
	game->config_options.selected = index;
	if (index == 0)
		name = "Crosshair";
	else if (index == 1)
		name = "Minimap";
	else
		name = "Fullscreen";
	*flag = !(*flag);
	state = "OFF";
	if (*flag)
		state = "ON";
	printf("Config: %s %s\n", name, state);
	draw_modal_layout(game);
}

int	config_option_from_pos(t_game *game, int32_t mx, int32_t my)
{
	int		y_base;
	int		index;
	int		y;

	if (!game || game->config_current_tab != 0)
		return (-1);
	y_base = game->config_panel_y + TAB_HEIGHT + CONFIG_MODAL_OPTION_TOP;
	if (my < y_base || my > game->config_panel_y + game->config_panel_h)
		return (-1);
	index = 0;
	while (index < 3)
	{
		y = y_base + index * CONFIG_MODAL_OPTION_LINE;
		if (my >= y && my < y + CONFIG_MODAL_OPTION_BOX
			&& mx >= game->config_panel_x + CONFIG_MODAL_OPTION_LEFT
			&& mx <= game->config_panel_x + game->config_panel_w
			- CONFIG_MODAL_OPTION_RIGHT)
			return (index);
		index++;
	}
	return (-1);
}
