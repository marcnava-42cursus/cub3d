/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_modal_labels_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/06 15:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/08 00:09:56 by marcnava         ###   ########.fr       */
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
			menu->labels.menu_entries[i] = mlx_put_string(game->mlx, text[i],
					0, 0);
		if (!menu->labels.menu_entries[i])
			return (false);
		set_image_enabled(menu->labels.menu_entries[i], false);
		i++;
	}
	return (true);
}

static bool	ensure_label_image(t_game *game, mlx_image_t **label,
				const char *text)
{
	if (!*label)
		*label = mlx_put_string(game->mlx, text, 0, 0);
	return (*label != NULL);
}

static bool	ensure_label_group_text(t_game *game, mlx_image_t **labels,
				const char **text, size_t count)
{
	size_t	i;

	i = 0;
	while (i < count)
	{
		if (!ensure_label_image(game, &labels[i], text[i]))
			return (false);
		i++;
	}
	return (true);
}

static void	settings_label_texts(const char **text)
{
	text[CONFIG_OPTION_SHOW_FPS] = "Show FPS";
	text[CONFIG_OPTION_CROSSHAIR] = "Show crosshair";
	text[CONFIG_OPTION_MINIMAP] = "Show minimap";
	text[CONFIG_OPTION_FULLSCREEN] = "Fullscreen";
	text[CONFIG_OPTION_DEBUG_MODE] = "DEBUG MODE";
	text[CONFIG_OPTION_FPS_LIMIT] = "FPS limit";
	text[CONFIG_OPTION_MOUSE_SENS] = "Mouse sensitivity";
	text[CONFIG_OPTION_QUALITY] = "Quality";
}

static void	hide_settings_labels(t_menu_state *menu)
{
	disable_label_group(menu->labels.settings_labels,
		CONFIG_MODAL_OPTION_COUNT);
	set_image_enabled(menu->labels.settings_sections[0], false);
	set_image_enabled(menu->labels.settings_sections[1], false);
}

bool	ensure_settings_labels(t_game *game)
{
	const char		*text[CONFIG_MODAL_OPTION_COUNT];
	t_menu_state	*menu;

	if (!game || !game->mlx)
		return (false);
	menu = &game->menu;
	settings_label_texts(text);
	if (!ensure_label_image(game, &menu->labels.settings_sections[0],
			"GENERAL"))
		return (false);
	if (!ensure_label_image(game, &menu->labels.settings_sections[1],
			"TUNING"))
		return (false);
	if (!ensure_label_group_text(game, menu->labels.settings_labels, text,
			CONFIG_MODAL_OPTION_COUNT))
		return (false);
	hide_settings_labels(menu);
	return (true);
}

static bool	ensure_control_headers(t_game *game, t_menu_state *menu)
{
	if (!ensure_label_image(game, &menu->labels.controls_header, "KEYBOARD"))
		return (false);
	if (!ensure_label_image(game, &menu->labels.controls_controller_header,
			"CONTROLLER"))
		return (false);
	if (!ensure_label_image(game, &menu->labels.controls_prompt,
			"PRESS A KEY..."))
		return (false);
	if (!ensure_label_image(game, &menu->labels.controls_controller_prompt,
			"PRESS A BUTTON..."))
		return (false);
	return (true);
}

static t_label_cache	label_cache(char *text, size_t size)
{
	t_label_cache	cache;

	cache.text = text;
	cache.size = size;
	return (cache);
}

static bool	update_control_key_labels_at(t_game *game, t_menu_state *menu,
				size_t i)
{
	if (!update_label_text(game, &menu->labels.controls_key_labels[i],
			label_cache(menu->labels.controls_key_cache[i],
				sizeof(menu->labels.controls_key_cache[i])),
			config_controls_key_text(game, (int)i)))
		return (false);
	if (!update_label_text(game,
			&menu->labels.controls_controller_key_labels[i],
			label_cache(menu->labels.controls_controller_key_cache[i],
				sizeof(menu->labels.controls_controller_key_cache[i])),
			config_controls_controller_text(game, (int)i)))
		return (false);
	if (!menu->labels.controls_labels[i] || !menu->labels.controls_key_labels[i]
		|| !menu->labels.controls_controller_labels[i]
		|| !menu->labels.controls_controller_key_labels[i])
		return (false);
	return (true);
}

static bool	update_control_key_labels(t_game *game, t_menu_state *menu)
{
	size_t	i;

	i = 0;
	while (i < CONFIG_MODAL_CONTROL_COUNT)
	{
		if (!update_control_key_labels_at(game, menu, i))
			return (false);
		i++;
	}
	return (true);
}

static void	control_label_texts(const char **text)
{
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
}

static void	hide_controls_labels(t_menu_state *menu)
{
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
}

bool	ensure_controls_labels(t_game *game)
{
	const char		*text[CONFIG_MODAL_CONTROL_COUNT];
	t_menu_state	*menu;

	if (!game || !game->mlx)
		return (false);
	menu = &game->menu;
	control_label_texts(text);
	if (!ensure_control_headers(game, menu))
		return (false);
	if (!ensure_label_group_text(game, menu->labels.controls_labels, text,
			CONFIG_MODAL_CONTROL_COUNT))
		return (false);
	if (!ensure_label_group_text(game, menu->labels.controls_controller_labels,
			text, CONFIG_MODAL_CONTROL_COUNT))
		return (false);
	if (!update_control_key_labels(game, menu))
		return (false);
	hide_controls_labels(menu);
	return (true);
}
