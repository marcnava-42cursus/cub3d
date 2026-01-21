/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_modal_layout_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/06 15:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/01/06 15:00:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config_bonus.h"
#include "structs.h"
#include "logic.h"

static void	draw_tab_icons(t_game *game, t_rect panel, int tab_w)
{
	t_icon	icon;

	icon.cx = panel.x + tab_w / 2;
	icon.cy = panel.y + TAB_HEIGHT / 2;
	icon.size = 24;
	icon.color = CONFIG_MODAL_MUTED_TEXT_COLOR;
	if (game->config_current_tab == 0)
		icon.color = CONFIG_MODAL_ACCENT_COLOR;
	draw_settings_icon(game->config_modal, icon);
	icon.cx = panel.x + tab_w + tab_w / 2;
	icon.size = 32;
	icon.color = CONFIG_MODAL_MUTED_TEXT_COLOR;
	if (game->config_current_tab == 1)
		icon.color = CONFIG_MODAL_ACCENT_COLOR;
	draw_controls_icon(game->config_modal, icon);
}

static void	draw_tabs(t_game *game, t_rect panel)
{
	int			tab_w;
	int			i;
	uint32_t	color;
	t_border	border;
	t_rect		indicator;

	tab_w = panel.w / 2;
	i = 0;
	border.thickness = 1;
	border.color = TAB_BORDER_COLOR;
	while (i < 2)
	{
		color = TAB_INACTIVE_COLOR;
		if (game->config_current_tab == i)
			color = TAB_ACTIVE_COLOR;
		border.area = rect_make(panel.x + i * tab_w, panel.y,
				tab_w, TAB_HEIGHT);
		draw_rect(game->config_modal, border.area, color);
		draw_border(game->config_modal, border);
		if (game->config_current_tab == i)
		{
			indicator = rect_make(border.area.x,
					border.area.y + TAB_HEIGHT - 3, tab_w, 3);
			draw_rect(game->config_modal, indicator,
				CONFIG_MODAL_ACCENT_COLOR);
		}
		i++;
	}
	draw_tab_icons(game, panel, tab_w);
}

static void	draw_quit_section(t_game *game, t_rect panel)
{
	if (game->config_current_tab != 0)
	{
		set_image_enabled(game->config_quit_label, false);
		return ;
	}
	game->config_quit_x = panel.x + panel.w - CONFIG_MODAL_BORDER_THICKNESS
		- CONFIG_MODAL_QUIT_BTN_MARGIN - game->config_quit_w;
	game->config_quit_y = panel.y + panel.h - CONFIG_MODAL_BORDER_THICKNESS
		- CONFIG_MODAL_QUIT_BTN_MARGIN - game->config_quit_h;
	render_quit_button(game);
	if (!game->config_quit_label && game->mlx)
		game->config_quit_label = mlx_put_string(game->mlx,
				CONFIG_MODAL_QUIT_TEXT, 0, 0);
	if (!game->config_quit_label)
		return ;
	game->config_quit_label->instances[0].x = game->config_quit_x
		+ (game->config_quit_w
			- (int32_t)game->config_quit_label->width) / 2;
	game->config_quit_label->instances[0].y = game->config_quit_y
		+ (game->config_quit_h
			- (int32_t)game->config_quit_label->height) / 2;
	set_image_enabled(game->config_quit_label, true);
}

static t_rect	build_panel_rect(t_game *game)
{
	t_rect	panel;

	panel.w = (int)(game->config_modal->width * 0.70f);
	panel.h = (int)(game->config_modal->height * 0.70f);
	panel.x = ((int)game->config_modal->width - panel.w) / 2;
	panel.y = ((int)game->config_modal->height - panel.h) / 2;
	game->config_panel_x = panel.x;
	game->config_panel_y = panel.y;
	game->config_panel_w = panel.w;
	game->config_panel_h = panel.h;
	return (panel);
}

void	draw_modal_layout(t_game *game)
{
	t_rect		panel;
	t_rect		body;
	t_border	border;

	if (!game || !game->config_modal)
		return ;
	panel = build_panel_rect(game);
	draw_rect(game->config_modal, rect_make(0, 0,
			(int)game->config_modal->width, (int)game->config_modal->height),
		CONFIG_MODAL_BG_COLOR);
	draw_vertical_gradient(game->config_modal, panel,
		CONFIG_MODAL_PANEL_TOP_COLOR, CONFIG_MODAL_PANEL_BOTTOM_COLOR);
	body = rect_make(panel.x, panel.y + TAB_HEIGHT,
			panel.w, panel.h - TAB_HEIGHT);
	draw_rect(game->config_modal, body, CONFIG_MODAL_PANEL_COLOR);
	border.area = panel;
	border.thickness = CONFIG_MODAL_BORDER_THICKNESS;
	draw_bevel_border(game->config_modal, border,
		CONFIG_MODAL_CARD_BORDER_LIGHT, CONFIG_MODAL_CARD_BORDER_DARK);
	draw_tabs(game, panel);
	draw_rect(game->config_modal, rect_make(panel.x,
			panel.y + TAB_HEIGHT - 1, panel.w, 1), TAB_BORDER_COLOR);
	if (game->config_current_tab == 0)
	{
		draw_settings_options(game, panel);
		hide_controls_options(game);
	}
	else
	{
		hide_settings_options(game);
		draw_controls_options(game, panel);
	}
	draw_quit_section(game, panel);
}
