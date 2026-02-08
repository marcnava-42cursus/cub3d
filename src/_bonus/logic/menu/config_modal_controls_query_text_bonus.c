/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_modal_controls_query_text_bonus.c           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 05:26:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/08 05:26:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config_bonus.h"
#include "structs.h"
#include "logic_bonus.h"

const char	*config_controls_controller_text(t_game *game, int index)
{
	t_menu_state	*menu;

	if (!game || index < 0 || index >= CONFIG_MODAL_CONTROL_COUNT)
		return ("");
	menu = &game->menu;
	return (menu->controls_controller_text[index]);
}
