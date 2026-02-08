/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_modal_options_actions_select_bonus.c        :+:      :+:    :+:   */
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
	int	selected;
	int	min;
	int	max;

	if (!game)
		return ;
	if (!get_section_option_range(game, &min, &max))
		return ;
	if (game->menu.options.selected < min || game->menu.options.selected > max)
	{
		game->menu.options.selected = min;
		draw_modal_layout(game);
		return ;
	}
	selected = game->menu.options.selected + delta;
	if (selected < min)
		selected = max;
	else if (selected > max)
		selected = min;
	game->menu.options.selected = selected;
	draw_modal_layout(game);
}
