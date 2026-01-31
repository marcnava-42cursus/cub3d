/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_modal_labels_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/06 15:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/01/21 21:14:14 by marcnava         ###   ########.fr       */
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
			char *cache, size_t cache_size, const char *text)
{
	if (!game || !game->mlx || !text)
		return (false);
	if (*label && ft_strncmp(cache, text, cache_size) == 0)
		return (true);
	if (*label)
		mlx_delete_image(game->mlx, *label);
	*label = mlx_put_string(game->mlx, text, 0, 0);
	if (!*label)
		return (false);
	ft_strlcpy(cache, text, cache_size);
	return (true);
}

bool	ensure_settings_labels(t_game *game)
{
	size_t			i;
	const char		*text[CONFIG_MODAL_OPTION_COUNT];
	t_menu_state	*menu;

	if (!game || !game->mlx)
		return (false);
	menu = &game->menu;
	text[CONFIG_OPTION_SHOW_FPS] = "Show FPS";
	text[CONFIG_OPTION_CROSSHAIR] = "Show crosshair";
	text[CONFIG_OPTION_MINIMAP] = "Show minimap";
	text[CONFIG_OPTION_FULLSCREEN] = "Fullscreen";
	text[CONFIG_OPTION_DEBUG_MODE] = "DEBUG MODE";
	text[CONFIG_OPTION_GAME_SPEED] = "Game speed";
	text[CONFIG_OPTION_FPS_LIMIT] = "FPS limit";
	text[CONFIG_OPTION_MOUSE_SENS] = "Mouse sensitivity";
	text[CONFIG_OPTION_PROJECTILE_SPEED] = "Projectile speed";
	text[CONFIG_OPTION_QUALITY] = "Quality";
	if (!menu->labels.settings_sections[0])
		menu->labels.settings_sections[0] = mlx_put_string(
				game->mlx, "DISPLAY", 0, 0);
	if (!menu->labels.settings_sections[1])
		menu->labels.settings_sections[1] = mlx_put_string(
				game->mlx, "TUNING", 0, 0);
	if (!menu->labels.settings_sections[0]
		|| !menu->labels.settings_sections[1])
		return (false);
	i = 0;
	while (i < CONFIG_MODAL_OPTION_COUNT)
	{
		if (!menu->labels.settings_labels[i])
			menu->labels.settings_labels[i] = mlx_put_string(
					game->mlx, text[i], 0, 0);
		if (!menu->labels.settings_labels[i])
			return (false);
		i++;
	}
	disable_label_group(menu->labels.settings_labels,
		CONFIG_MODAL_OPTION_COUNT);
	set_image_enabled(menu->labels.settings_sections[0], false);
	set_image_enabled(menu->labels.settings_sections[1], false);
	return (true);
}

bool	ensure_controls_labels(t_game *game)
{
	size_t			i;
	const char		*text[CONFIG_MODAL_CONTROL_COUNT];
	t_menu_state	*menu;

	if (!game || !game->mlx)
		return (false);
	menu = &game->menu;
	text[0] = "Move forward";
	text[1] = "Move backward";
	text[2] = "Strafe right";
	text[3] = "Strafe left";
	text[4] = "Turn right";
	text[5] = "Turn left";
	text[6] = "Look up";
	text[7] = "Look down";
	text[8] = "Break";
	text[9] = "Place";
	text[10] = "Open menu";
	text[11] = "Open map";
	text[12] = "Accept";
	text[13] = "Quit game";
	if (!menu->labels.controls_header)
		menu->labels.controls_header = mlx_put_string(
				game->mlx, "KEYBOARD", 0, 0);
	if (!menu->labels.controls_controller_header)
		menu->labels.controls_controller_header = mlx_put_string(
				game->mlx, "CONTROLLER", 0, 0);
	if (!menu->labels.controls_prompt)
		menu->labels.controls_prompt = mlx_put_string(
				game->mlx, "PRESS A KEY...", 0, 0);
	if (!menu->labels.controls_controller_prompt)
		menu->labels.controls_controller_prompt = mlx_put_string(
				game->mlx, "PRESS A BUTTON...", 0, 0);
	if (!menu->labels.controls_header || !menu->labels.controls_prompt
		|| !menu->labels.controls_controller_header
		|| !menu->labels.controls_controller_prompt)
		return (false);
	i = 0;
	while (i < CONFIG_MODAL_CONTROL_COUNT)
	{
		if (!menu->labels.controls_labels[i])
			menu->labels.controls_labels[i] = mlx_put_string(
					game->mlx, text[i], 0, 0);
		if (!menu->labels.controls_controller_labels[i])
			menu->labels.controls_controller_labels[i] = mlx_put_string(
					game->mlx, text[i], 0, 0);
		if (!update_label_text(game, &menu->labels.controls_key_labels[i],
				menu->labels.controls_key_cache[i],
				sizeof(menu->labels.controls_key_cache[i]),
				config_controls_key_text(game, (int)i)))
			return (false);
		if (!update_label_text(game,
				&menu->labels.controls_controller_key_labels[i],
				menu->labels.controls_controller_key_cache[i],
				sizeof(menu->labels.controls_controller_key_cache[i]),
				config_controls_controller_text(game, (int)i)))
			return (false);
		if (!menu->labels.controls_labels[i]
			|| !menu->labels.controls_key_labels[i]
			|| !menu->labels.controls_controller_labels[i]
			|| !menu->labels.controls_controller_key_labels[i])
			return (false);
		i++;
	}
	disable_label_group(menu->labels.controls_labels,
		CONFIG_MODAL_CONTROL_COUNT);
	disable_label_group(menu->labels.controls_key_labels,
		CONFIG_MODAL_CONTROL_COUNT);
	disable_label_group(menu->labels.controls_controller_labels,
		CONFIG_MODAL_CONTROL_COUNT);
	disable_label_group(menu->labels.controls_controller_key_labels,
		CONFIG_MODAL_CONTROL_COUNT);
	set_image_enabled(menu->labels.controls_header, false);
	set_image_enabled(menu->labels.controls_prompt, false);
	set_image_enabled(menu->labels.controls_controller_header, false);
	set_image_enabled(menu->labels.controls_controller_prompt, false);
	return (true);
}
