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
#include "logic_bonus.h"

static void	draw_menu_item_border(mlx_image_t *img, t_rect item)
{
	t_border	border;

	border.area = item;
	border.thickness = 1;
	border.color = CONFIG_MODAL_MENU_ITEM_BORDER_COLOR;
	draw_border(img, border);
}

static void	draw_menu_labels(t_game *game, t_rect card)
{
	t_menu_state	*menu;
	int	i;

	menu = &game->menu;
	i = 0;
	while (i < CONFIG_MENU_SECTION_COUNT)
	{
		t_rect	item;
		int		row_y;

		row_y = card.y + CONFIG_MODAL_CARD_PADDING
			+ i * (CONFIG_MODAL_MENU_ITEM_H + CONFIG_MODAL_MENU_ITEM_GAP);
		item = rect_make(card.x + CONFIG_MODAL_CARD_PADDING, row_y,
				card.w - CONFIG_MODAL_CARD_PADDING * 2, CONFIG_MODAL_MENU_ITEM_H);
		draw_rect(game->menu.modal, item, (menu->current_tab == i)
			? CONFIG_MODAL_MENU_ITEM_ACTIVE_COLOR : CONFIG_MODAL_MENU_ITEM_COLOR);
		draw_menu_item_border(game->menu.modal, item);
		menu->labels.menu_entries[i]->instances[0].x = item.x
			+ CONFIG_MODAL_CARD_PADDING;
		menu->labels.menu_entries[i]->instances[0].y = item.y
			+ (item.h - (int)menu->labels.menu_entries[i]->height) / 2;
		set_image_enabled(menu->labels.menu_entries[i], true);
		i++;
	}
}

static void	draw_quit_section(t_game *game, t_rect panel)
{
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

static void	draw_current_tab_content(t_game *game, t_menu_layout layout)
{
	if (game->menu.current_tab == CONFIG_MENU_GENERAL
		|| game->menu.current_tab == CONFIG_MENU_TUNING)
	{
		draw_settings_options(game, layout.panel);
		hide_controls_options(game);
		return ;
	}
	hide_settings_options(game);
	draw_controls_options(game, layout.panel);
}

void	draw_modal_layout(t_game *game)
{
	t_menu_layout	layout;
	t_border		border;

	if (!game || !game->menu.modal)
		return ;
	layout = menu_layout_build(&game->menu);
	if (!ensure_menu_labels(game))
		return ;
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
	draw_card(game, layout.left);
	draw_menu_labels(game, layout.left);
	draw_current_tab_content(game, layout);
	draw_quit_section(game, layout.panel);
}
