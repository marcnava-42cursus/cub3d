/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_modal_layout_calc_bonus.c                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/06 15:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/06 18:45:16 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config_bonus.h"
#include "structs.h"
#include "logic_bonus.h"

int	menu_layout_row_y(t_rect card, int index)
{
	return (card.y + CONFIG_MODAL_CARD_PADDING
		+ CONFIG_MODAL_SECTION_TITLE_GAP + index * CONFIG_MODAL_ROW_SPACING);
}

t_rect	menu_layout_slider_rect(t_menu_layout layout, int row_y)
{
	return (rect_make(layout.right.x + layout.right.w
			- CONFIG_MODAL_CARD_PADDING - CONFIG_MODAL_SLIDER_VALUE_W
			- CONFIG_MODAL_SLIDER_VALUE_GAP - layout.slider_w,
			row_y + (CONFIG_MODAL_ROW_HEIGHT - CONFIG_MODAL_SLIDER_H) / 2,
			layout.slider_w, CONFIG_MODAL_SLIDER_H));
}

t_menu_layout	menu_layout_build(t_menu_state *menu)
{
	t_menu_layout	layout;
	int				menu_w;
	int				min_right_w;

	layout.panel.w = (int)(menu->modal->width * 0.70f);
	layout.panel.h = (int)(menu->modal->height * 0.70f);
	layout.panel.x = ((int)menu->modal->width - layout.panel.w) / 2;
	layout.panel.y = ((int)menu->modal->height - layout.panel.h) / 2;
	layout.body = layout.panel;
	layout.content = rect_make(layout.body.x + CONFIG_MODAL_CONTENT_PADDING,
			layout.body.y + CONFIG_MODAL_CONTENT_PADDING,
			layout.body.w - 2 * CONFIG_MODAL_CONTENT_PADDING,
			layout.body.h - 2 * CONFIG_MODAL_CONTENT_PADDING);
	menu_w = layout.content.w * 30 / 100;
	if (menu_w < CONFIG_MODAL_MENU_MIN_W)
		menu_w = CONFIG_MODAL_MENU_MIN_W;
	if (menu_w > CONFIG_MODAL_MENU_MAX_W)
		menu_w = CONFIG_MODAL_MENU_MAX_W;
	min_right_w = 240;
	if (menu_w > layout.content.w - CONFIG_MODAL_COLUMN_GAP - min_right_w)
		menu_w = layout.content.w - CONFIG_MODAL_COLUMN_GAP - min_right_w;
	if (menu_w < 140)
		menu_w = 140;
	layout.left = rect_make(layout.content.x, layout.content.y, menu_w,
			layout.content.h - CONFIG_MODAL_FOOTER_SPACE);
	layout.right = rect_make(layout.left.x + layout.left.w
			+ CONFIG_MODAL_COLUMN_GAP, layout.content.y,
			layout.content.w - layout.left.w - CONFIG_MODAL_COLUMN_GAP,
			layout.content.h - CONFIG_MODAL_FOOTER_SPACE);
	if (layout.left.h < CONFIG_MODAL_ROW_SPACING * 3)
		layout.left.h = layout.content.h;
	if (layout.right.h < CONFIG_MODAL_ROW_SPACING * 3)
		layout.right.h = layout.content.h;
	layout.slider_w = CONFIG_MODAL_SLIDER_W;
	if (layout.slider_w > layout.right.w - CONFIG_MODAL_CARD_PADDING * 2
		- CONFIG_MODAL_SLIDER_VALUE_W - CONFIG_MODAL_SLIDER_VALUE_GAP)
		layout.slider_w = layout.right.w - CONFIG_MODAL_CARD_PADDING * 2
			- CONFIG_MODAL_SLIDER_VALUE_W - CONFIG_MODAL_SLIDER_VALUE_GAP;
	if (layout.slider_w < 80)
		layout.slider_w = layout.right.w - CONFIG_MODAL_CARD_PADDING * 2;
	return (layout);
}
