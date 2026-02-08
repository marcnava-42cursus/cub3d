/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_modal_layout_calc_bonus.c                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/06 15:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/08 05:26:00 by marcnava         ###   ########.fr       */
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
