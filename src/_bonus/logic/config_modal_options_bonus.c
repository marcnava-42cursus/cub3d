/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_modal_options_bonus.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/06 15:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/01/06 15:00:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config_modal_bonus.h"

static bool	ensure_option_labels(t_game *game)
{
	size_t		i;
	const char	*text[3];

	if (!game || !game->mlx)
		return (false);
	text[0] = "Enable/Disable crosshair";
	text[1] = "Enable/Disable minimap";
	text[2] = "Toggle fullscreen";
	i = 0;
	while (i < 3)
	{
		if (!game->config_option_labels[i])
			game->config_option_labels[i] = mlx_put_string(game->mlx,
					text[i], 0, 0);
		if (!game->config_option_labels[i])
			return (false);
		i++;
	}
	return (true);
}

static bool	option_checked(t_game *game, int index)
{
	if (!game)
		return (false);
	if (index == 0 && game->config_options.crosshair)
		return (true);
	if (index == 1 && game->config_options.minimap)
		return (true);
	if (index == 2 && game->config_options.fullscreen)
		return (true);
	return (false);
}

static void	draw_option_checkbox(t_game *game, t_rect rect, bool checked,
			bool selected)
{
	t_border	border;
	t_rect		fill;
	uint32_t	color;

	draw_rect(game->config_modal, rect, CONFIG_MODAL_PANEL_COLOR);
	color = CONFIG_MODAL_BORDER_COLOR;
	if (selected)
		color = TAB_SELECTED_BORDER_COLOR;
	border.area = rect;
	border.thickness = CONFIG_MODAL_BORDER_THICKNESS;
	border.color = color;
	draw_border(game->config_modal, border);
	if (!checked)
		return ;
	fill = rect_make(rect.x + CONFIG_MODAL_BORDER_THICKNESS + 2,
			rect.y + CONFIG_MODAL_BORDER_THICKNESS + 2,
			rect.w - (CONFIG_MODAL_BORDER_THICKNESS + 2) * 2,
			rect.h - (CONFIG_MODAL_BORDER_THICKNESS + 2) * 2);
	draw_rect(game->config_modal, fill, TAB_SELECTED_BORDER_COLOR);
}

void	hide_settings_options(t_game *game)
{
	size_t	i;

	if (!game)
		return ;
	i = 0;
	while (i < 3)
	{
		set_image_enabled(game->config_option_labels[i], false);
		i++;
	}
}

void	draw_settings_options(t_game *game, t_rect panel)
{
	int		i;
	int		y;
	int		x_box;

	if (!game || !game->config_modal || !ensure_option_labels(game))
		return ;
	x_box = panel.x + panel.w - CONFIG_MODAL_OPTION_RIGHT
		- CONFIG_MODAL_OPTION_BOX;
	i = 0;
	while (i < 3)
	{
		y = panel.y + TAB_HEIGHT + CONFIG_MODAL_OPTION_TOP
			+ i * CONFIG_MODAL_OPTION_LINE;
		game->config_option_labels[i]->instances[0].x = panel.x
			+ CONFIG_MODAL_OPTION_LEFT;
		game->config_option_labels[i]->instances[0].y = y;
		set_image_enabled(game->config_option_labels[i], true);
		draw_option_checkbox(game, rect_make(x_box, y, CONFIG_MODAL_OPTION_BOX,
				CONFIG_MODAL_OPTION_BOX), option_checked(game, i),
			game->config_options.selected == i);
		i++;
	}
}
