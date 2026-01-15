/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inventory.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 00:58:05 by marcnava          #+#    #+#             */
/*   Updated: 2026/01/15 18:07:59 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "logic.h"

bool	player_has_block(const t_player *player)
{
	if (!player)
		return (false);
	return (player->inventory != '\0');
}

bool	store_block_in_inventory(t_player *player, char block)
{
	if (!player || player_has_block(player))
		return (false);
	player->inventory = block;
	return (true);
}

char	consume_inventory_block(t_player *player)
{
	char	block;

	if (!player_has_block(player))
		return ('\0');
	block = player->inventory;
	player->inventory = '\0';
	return (block);
}
