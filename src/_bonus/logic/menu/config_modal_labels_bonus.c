/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_modal_labels_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/06 15:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/08 05:52:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config_bonus.h"
#include "structs.h"
#include "logic_bonus.h"

void	disable_label_group(mlx_image_t **labels, size_t count)
{
	size_t	i;

	i = 0;
	while (i < count)
	{
		set_image_enabled(labels[i], false);
		i++;
	}
}

bool	update_label_text(t_game *game, mlx_image_t **label,
			t_label_cache cache, const char *text)
{
	if (!game || !game->mlx || !text)
		return (false);
	if (*label && ft_strncmp(cache.text, text, cache.size) == 0)
		return (true);
	if (*label)
		mlx_delete_image(game->mlx, *label);
	*label = mlx_put_string(game->mlx, text, 0, 0);
	if (!*label)
		return (false);
	ft_strlcpy(cache.text, text, cache.size);
	return (true);
}

bool	ensure_menu_labels(t_game *game)
{
	const char		*text[CONFIG_MENU_SECTION_COUNT];
	t_menu_state	*menu;
	size_t			i;

	if (!game || !game->mlx)
		return (false);
	menu = &game->menu;
	text[CONFIG_MENU_GENERAL] = "General";
	text[CONFIG_MENU_TUNING] = "Tuning";
	text[CONFIG_MENU_KEYBOARD_CONTROLS] = "Keyboard Controls";
	text[CONFIG_MENU_CONTROLLER_CONTROLS] = "Controller Controls";
	i = 0;
	while (i < CONFIG_MENU_SECTION_COUNT)
	{
		if (!menu->labels.menu_entries[i])
			menu->labels.menu_entries[i] = mlx_put_string(game->mlx, text[i], 0,
					0);
		if (!menu->labels.menu_entries[i])
			return (false);
		set_image_enabled(menu->labels.menu_entries[i], false);
		i++;
	}
	return (true);
}

bool	config_ensure_label_image_bonus(t_game *game, mlx_image_t **label,
			const char *text)
{
	if (!*label)
		*label = mlx_put_string(game->mlx, text, 0, 0);
	return (*label != NULL);
}

bool	config_ensure_label_group_text_bonus(t_game *game, mlx_image_t **labels,
			const char **text, size_t count)
{
	size_t	i;

	i = 0;
	while (i < count)
	{
		if (!config_ensure_label_image_bonus(game, &labels[i], text[i]))
			return (false);
		i++;
	}
	return (true);
}
