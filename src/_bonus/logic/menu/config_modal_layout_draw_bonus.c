/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_modal_layout_draw_bonus.c                   :+:      :+:    :+:   */
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

static void	draw_tab_icons(t_game *game, t_rect panel, int tab_w)
{
	t_icon	icon;

	icon.cx = panel.x + tab_w / 2;
	icon.cy = panel.y + TAB_HEIGHT / 2;
	icon.size = 24;
	icon.color = CONFIG_MODAL_MUTED_TEXT_COLOR;
	if (game->menu.current_tab == 0)
		icon.color = CONFIG_MODAL_ACCENT_COLOR;
	draw_settings_icon(game->menu.modal, icon);
	icon.cx = panel.x + tab_w + tab_w / 2;
	icon.size = 32;
	icon.color = CONFIG_MODAL_MUTED_TEXT_COLOR;
	if (game->menu.current_tab == 1)
		icon.color = CONFIG_MODAL_ACCENT_COLOR;
	draw_controls_icon(game->menu.modal, icon);
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
		if (game->menu.current_tab == i)
			color = TAB_ACTIVE_COLOR;
		border.area = rect_make(panel.x + i * tab_w, panel.y,
				tab_w, TAB_HEIGHT);
		draw_rect(game->menu.modal, border.area, color);
		draw_border(game->menu.modal, border);
		if (game->menu.current_tab == i)
		{
			indicator = rect_make(border.area.x,
					border.area.y + TAB_HEIGHT - 3, tab_w, 3);
			draw_rect(game->menu.modal, indicator,
				CONFIG_MODAL_ACCENT_COLOR);
		}
		i++;
	}
	draw_tab_icons(game, panel, tab_w);
}

static void	draw_quit_section(t_game *game, t_rect panel)
{
	if (game->menu.current_tab != 0)
	{
		set_image_enabled(game->menu.quit_label, false);
		return ;
	}
	game->menu.quit_x = panel.x + panel.w - CONFIG_MODAL_BORDER_THICKNESS
		- CONFIG_MODAL_QUIT_BTN_MARGIN - game->menu.quit_w;
	game->menu.quit_y = panel.y + panel.h - CONFIG_MODAL_BORDER_THICKNESS
		- CONFIG_MODAL_QUIT_BTN_MARGIN - game->menu.quit_h;
	render_quit_button(game);
	if (!game->menu.quit_label && game->mlx)
		game->menu.quit_label = mlx_put_string(game->mlx,
				CONFIG_MODAL_QUIT_TEXT, 0, 0);
	if (!game->menu.quit_label)
		return ;
	game->menu.quit_label->instances[0].x = game->menu.quit_x
		+ (game->menu.quit_w
			- (int32_t)game->menu.quit_label->width) / 2;
	game->menu.quit_label->instances[0].y = game->menu.quit_y
		+ (game->menu.quit_h
			- (int32_t)game->menu.quit_label->height) / 2;
	set_image_enabled(game->menu.quit_label, true);
}

void	draw_modal_layout(t_game *game)
{
	t_menu_layout	layout;
	t_border		border;

	if (!game || !game->menu.modal)
		return ;
	layout = menu_layout_build(&game->menu);
	draw_rect(game->menu.modal, rect_make(0, 0,
			(int)game->menu.modal->width, (int)game->menu.modal->height),
		CONFIG_MODAL_BG_COLOR);
	draw_vertical_gradient(game->menu.modal, layout.panel,
		CONFIG_MODAL_PANEL_TOP_COLOR, CONFIG_MODAL_PANEL_BOTTOM_COLOR);
	draw_rect(game->menu.modal, layout.body, CONFIG_MODAL_PANEL_COLOR);
	border.area = layout.panel;
	border.thickness = CONFIG_MODAL_BORDER_THICKNESS;
	draw_bevel_border(game->menu.modal, border,
		CONFIG_MODAL_CARD_BORDER_LIGHT, CONFIG_MODAL_CARD_BORDER_DARK);
	draw_tabs(game, layout.panel);
	draw_rect(game->menu.modal, rect_make(layout.panel.x,
			layout.panel.y + TAB_HEIGHT - 1, layout.panel.w, 1),
		TAB_BORDER_COLOR);
	if (game->menu.current_tab == 0)
	{
		draw_settings_options(game, layout.panel);
		hide_controls_options(game);
	}
	else
	{
		hide_settings_options(game);
		draw_controls_options(game, layout.panel);
	}
	draw_quit_section(game, layout.panel);
}
