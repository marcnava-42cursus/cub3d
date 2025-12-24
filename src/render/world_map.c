/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   world_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 09:02:52 by ivmirand          #+#    #+#             */
/*   Updated: 2025/12/24 10:00:00 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

bool	world_map_init(t_game *game)
{
	(void)game;
	// Mandatory part does not need world map initialization
	return (true);
}

void	set_map_overlay_visible(t_game *game, bool visible)
{
	(void)game;
	(void)visible;
	// Mandatory part does not support map overlay
}

void	toggle_map_overlay(t_game *game)
{
	(void)game;
	// Mandatory part does not support map overlay
}