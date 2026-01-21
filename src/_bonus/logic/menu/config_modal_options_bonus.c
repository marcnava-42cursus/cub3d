/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_modal_options_bonus.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/06 15:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/01/21 17:07:55 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config_bonus.h"
#include "structs.h"
#include "logic.h"

static mlx_image_t	*g_settings_labels[CONFIG_MODAL_OPTION_COUNT];
static mlx_image_t	*g_controls_labels[CONFIG_MODAL_CONTROL_COUNT];
static mlx_image_t	*g_controls_key_labels[CONFIG_MODAL_CONTROL_COUNT];
static mlx_image_t	*g_settings_sections[2];
static mlx_image_t	*g_controls_header;
static mlx_image_t	*g_controls_prompt;
static mlx_image_t	*g_slider_value_labels[CONFIG_MODAL_SLIDER_COUNT];
static char			g_slider_value_cache[CONFIG_MODAL_SLIDER_COUNT]
	[CONFIG_MODAL_SLIDER_VALUE_LEN];
static char			g_controls_key_cache[CONFIG_MODAL_CONTROL_COUNT]
	[CONFIG_MODAL_KEY_LABEL_LEN];

static void	disable_label_group(mlx_image_t **labels, size_t count)
{
	size_t	i;

	i = 0;
	while (i < count)
	{
		set_image_enabled(labels[i], false);
		i++;
	}
}

static bool	update_label_text(t_game *game, mlx_image_t **label,
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

static bool	ensure_settings_labels(t_game *game)
{
	size_t		i;
	const char	*text[CONFIG_MODAL_OPTION_COUNT];

	if (!game || !game->mlx)
		return (false);
	text[CONFIG_OPTION_SHOW_FPS] = "Show FPS";
	text[CONFIG_OPTION_CROSSHAIR] = "Show crosshair";
	text[CONFIG_OPTION_MINIMAP] = "Show minimap";
	text[CONFIG_OPTION_FULLSCREEN] = "Fullscreen";
	text[CONFIG_OPTION_GAME_SPEED] = "Game speed";
	text[CONFIG_OPTION_FPS_LIMIT] = "FPS limit";
	text[CONFIG_OPTION_MOUSE_SENS] = "Mouse sensitivity";
	if (!g_settings_sections[0])
		g_settings_sections[0] = mlx_put_string(game->mlx, "DISPLAY", 0, 0);
	if (!g_settings_sections[1])
		g_settings_sections[1] = mlx_put_string(game->mlx, "TUNING", 0, 0);
	if (!g_settings_sections[0] || !g_settings_sections[1])
		return (false);
	i = 0;
	while (i < CONFIG_MODAL_OPTION_COUNT)
	{
		if (!g_settings_labels[i])
			g_settings_labels[i] = mlx_put_string(game->mlx, text[i], 0, 0);
		if (!g_settings_labels[i])
			return (false);
		i++;
	}
	disable_label_group(g_settings_labels, CONFIG_MODAL_OPTION_COUNT);
	set_image_enabled(g_settings_sections[0], false);
	set_image_enabled(g_settings_sections[1], false);
	return (true);
}

static bool	ensure_controls_labels(t_game *game)
{
	size_t		i;
	const char	*text[CONFIG_MODAL_CONTROL_COUNT];

	if (!game || !game->mlx)
		return (false);
	text[0] = "Move forward";
	text[1] = "Move backward";
	text[2] = "Strafe right";
	text[3] = "Strafe left";
	text[4] = "Turn right";
	text[5] = "Turn left";
	text[6] = "Break";
	text[7] = "Place";
	text[8] = "Open menu";
	text[9] = "Open map";
	if (!g_controls_header)
		g_controls_header = mlx_put_string(game->mlx, "CONTROLS", 0, 0);
	if (!g_controls_prompt)
		g_controls_prompt = mlx_put_string(game->mlx, "PRESS A KEY...", 0, 0);
	if (!g_controls_header || !g_controls_prompt)
		return (false);
	i = 0;
	while (i < CONFIG_MODAL_CONTROL_COUNT)
	{
		if (!g_controls_labels[i])
			g_controls_labels[i] = mlx_put_string(game->mlx, text[i], 0, 0);
		if (!update_label_text(game, &g_controls_key_labels[i],
				g_controls_key_cache[i], sizeof(g_controls_key_cache[i]),
				config_controls_key_text((int)i)))
			return (false);
		if (!g_controls_labels[i] || !g_controls_key_labels[i])
			return (false);
		i++;
	}
	disable_label_group(g_controls_labels, CONFIG_MODAL_CONTROL_COUNT);
	disable_label_group(g_controls_key_labels, CONFIG_MODAL_CONTROL_COUNT);
	set_image_enabled(g_controls_header, false);
	set_image_enabled(g_controls_prompt, false);
	return (true);
}

static t_rect	build_body_rect(t_rect panel)
{
	return (rect_make(panel.x, panel.y + TAB_HEIGHT,
			panel.w, panel.h - TAB_HEIGHT));
}

static t_rect	build_content_rect(t_rect panel)
{
	t_rect	body;

	body = build_body_rect(panel);
	return (rect_make(body.x + CONFIG_MODAL_CONTENT_PADDING,
			body.y + CONFIG_MODAL_CONTENT_PADDING,
			body.w - 2 * CONFIG_MODAL_CONTENT_PADDING,
			body.h - 2 * CONFIG_MODAL_CONTENT_PADDING));
}

static void	build_cards(t_rect panel, t_rect *left, t_rect *right)
{
	t_rect	content;
	int		card_w;
	int		card_h;

	content = build_content_rect(panel);
	card_w = (content.w - CONFIG_MODAL_COLUMN_GAP) / 2;
	card_h = content.h - CONFIG_MODAL_FOOTER_SPACE;
	if (card_h < CONFIG_MODAL_ROW_SPACING * 3)
		card_h = content.h;
	*left = rect_make(content.x, content.y, card_w, card_h);
	*right = rect_make(content.x + card_w + CONFIG_MODAL_COLUMN_GAP,
			content.y, card_w, card_h);
}

static int	row_y_from_card(t_rect card, int index)
{
	return (card.y + CONFIG_MODAL_CARD_PADDING
		+ CONFIG_MODAL_SECTION_TITLE_GAP + index * CONFIG_MODAL_ROW_SPACING);
}

static void	draw_card(t_game *game, t_rect card)
{
	t_border	border;

	draw_vertical_gradient(game->config_modal, card,
		CONFIG_MODAL_CARD_TOP_COLOR, CONFIG_MODAL_CARD_BOTTOM_COLOR);
	border.area = card;
	border.thickness = CONFIG_MODAL_BORDER_THICKNESS;
	draw_bevel_border(game->config_modal, border,
		CONFIG_MODAL_CARD_BORDER_LIGHT, CONFIG_MODAL_CARD_BORDER_DARK);
}

static void	draw_row_highlight(t_game *game, t_rect card, int row_y)
{
	t_rect	highlight;

	highlight = rect_make(card.x + 6, row_y + 2, card.w - 12,
			CONFIG_MODAL_ROW_HEIGHT - 4);
	draw_rect(game->config_modal, highlight, CONFIG_MODAL_ACCENT_SOFT);
	draw_rect(game->config_modal, rect_make(highlight.x, highlight.y,
			3, highlight.h), CONFIG_MODAL_ACCENT_COLOR);
}

static void	draw_toggle_switch(t_game *game, t_rect rect, bool enabled,
			bool selected)
{
	t_border	border;
	t_rect		knob;
	uint32_t	track;

	track = CONFIG_MODAL_TOGGLE_OFF_COLOR;
	if (enabled)
		track = CONFIG_MODAL_TOGGLE_ON_COLOR;
	draw_rect(game->config_modal, rect, track);
	border.area = rect;
	border.thickness = 1;
	border.color = CONFIG_MODAL_CARD_BORDER_DARK;
	draw_border(game->config_modal, border);
	knob.w = rect.h - 4;
	knob.h = rect.h - 4;
	knob.y = rect.y + 2;
	if (enabled)
		knob.x = rect.x + rect.w - knob.w - 2;
	else
		knob.x = rect.x + 2;
	draw_rect(game->config_modal, knob, CONFIG_MODAL_SLIDER_KNOB_COLOR);
	if (selected)
	{
		border.area = knob;
		border.thickness = 1;
		border.color = CONFIG_MODAL_ACCENT_COLOR;
		draw_border(game->config_modal, border);
	}
}

static void	draw_slider(t_game *game, t_rect track, int value, bool selected)
{
	t_rect		fill;
	t_rect		knob;
	t_border	border;
	int			knob_x;

	draw_rect(game->config_modal, track, CONFIG_MODAL_SLIDER_TRACK_COLOR);
	fill = track;
	fill.w = (track.w * value) / 100;
	draw_rect(game->config_modal, fill, CONFIG_MODAL_SLIDER_FILL_COLOR);
	knob_x = track.x + (track.w - 1) * value / 100
		- CONFIG_MODAL_SLIDER_KNOB / 2;
	if (knob_x < track.x)
		knob_x = track.x;
	if (knob_x + CONFIG_MODAL_SLIDER_KNOB > track.x + track.w)
		knob_x = track.x + track.w - CONFIG_MODAL_SLIDER_KNOB;
	knob = rect_make(knob_x,
			track.y + track.h / 2 - CONFIG_MODAL_SLIDER_KNOB / 2,
			CONFIG_MODAL_SLIDER_KNOB, CONFIG_MODAL_SLIDER_KNOB);
	draw_rect(game->config_modal, knob, CONFIG_MODAL_SLIDER_KNOB_COLOR);
	border.area = knob;
	border.thickness = 1;
	border.color = CONFIG_MODAL_CARD_BORDER_DARK;
	draw_border(game->config_modal, border);
	if (selected)
	{
		border.color = CONFIG_MODAL_ACCENT_COLOR;
		draw_border(game->config_modal, border);
	}
}

void	hide_settings_options(t_game *game)
{
	(void)game;
	disable_label_group(g_settings_labels, CONFIG_MODAL_OPTION_COUNT);
	disable_label_group(g_slider_value_labels, CONFIG_MODAL_SLIDER_COUNT);
	set_image_enabled(g_settings_sections[0], false);
	set_image_enabled(g_settings_sections[1], false);
}

void	hide_controls_options(t_game *game)
{
	(void)game;
	disable_label_group(g_controls_labels, CONFIG_MODAL_CONTROL_COUNT);
	disable_label_group(g_controls_key_labels, CONFIG_MODAL_CONTROL_COUNT);
	set_image_enabled(g_controls_header, false);
	set_image_enabled(g_controls_prompt, false);
}

void	draw_settings_options(t_game *game, t_rect panel)
{
	t_rect	left;
	t_rect	right;
	int		i;
	int		row_y;
	int		slider_w;
	t_rect	toggle_rect;
	t_rect	slider_rect;
	t_rect	value_rect;
	char	value_text[CONFIG_MODAL_SLIDER_VALUE_LEN];
	bool	selected;

	if (!game || !game->config_modal || !ensure_settings_labels(game))
		return ;
	build_cards(panel, &left, &right);
	draw_card(game, left);
	draw_card(game, right);
	g_settings_sections[0]->instances[0].x = left.x + CONFIG_MODAL_CARD_PADDING;
	g_settings_sections[0]->instances[0].y = left.y + CONFIG_MODAL_CARD_PADDING;
	g_settings_sections[1]->instances[0].x = right.x
		+ CONFIG_MODAL_CARD_PADDING;
	g_settings_sections[1]->instances[0].y = right.y + CONFIG_MODAL_CARD_PADDING;
	set_image_enabled(g_settings_sections[0], true);
	set_image_enabled(g_settings_sections[1], true);
	i = 0;
	while (i < CONFIG_MODAL_TOGGLE_COUNT)
	{
		row_y = row_y_from_card(left, i);
		selected = (game->config_options.selected == i);
		if (selected)
			draw_row_highlight(game, left, row_y);
		g_settings_labels[i]->instances[0].x = left.x
			+ CONFIG_MODAL_CARD_PADDING;
		g_settings_labels[i]->instances[0].y = row_y + 4;
		set_image_enabled(g_settings_labels[i], true);
		toggle_rect = rect_make(left.x + left.w - CONFIG_MODAL_CARD_PADDING
				- CONFIG_MODAL_TOGGLE_W,
				row_y + (CONFIG_MODAL_ROW_HEIGHT - CONFIG_MODAL_TOGGLE_H) / 2,
				CONFIG_MODAL_TOGGLE_W, CONFIG_MODAL_TOGGLE_H);
		draw_toggle_switch(game, toggle_rect,
			config_option_toggle_state(game, i), selected);
		i++;
	}
	slider_w = CONFIG_MODAL_SLIDER_W;
	if (slider_w > right.w - CONFIG_MODAL_CARD_PADDING * 2
		- CONFIG_MODAL_SLIDER_VALUE_W - CONFIG_MODAL_SLIDER_VALUE_GAP)
		slider_w = right.w - CONFIG_MODAL_CARD_PADDING * 2
			- CONFIG_MODAL_SLIDER_VALUE_W - CONFIG_MODAL_SLIDER_VALUE_GAP;
	if (slider_w < 80)
		slider_w = right.w - CONFIG_MODAL_CARD_PADDING * 2;
	i = 0;
	while (i < CONFIG_MODAL_SLIDER_COUNT)
	{
		row_y = row_y_from_card(right, i);
		selected = (game->config_options.selected
				== CONFIG_MODAL_TOGGLE_COUNT + i);
		if (selected)
			draw_row_highlight(game, right, row_y);
		g_settings_labels[CONFIG_MODAL_TOGGLE_COUNT + i]->instances[0].x
			= right.x + CONFIG_MODAL_CARD_PADDING;
		g_settings_labels[CONFIG_MODAL_TOGGLE_COUNT + i]->instances[0].y
			= row_y + 4;
		set_image_enabled(g_settings_labels[CONFIG_MODAL_TOGGLE_COUNT + i], true);
		slider_rect = rect_make(right.x + right.w - CONFIG_MODAL_CARD_PADDING
				- CONFIG_MODAL_SLIDER_VALUE_W
				- CONFIG_MODAL_SLIDER_VALUE_GAP - slider_w,
				row_y + (CONFIG_MODAL_ROW_HEIGHT - CONFIG_MODAL_SLIDER_H) / 2,
				slider_w, CONFIG_MODAL_SLIDER_H);
		draw_slider(game, slider_rect,
			config_option_slider_value(game, CONFIG_MODAL_TOGGLE_COUNT + i),
			selected);
		config_option_slider_text(game, CONFIG_MODAL_TOGGLE_COUNT + i,
			value_text, sizeof(value_text));
		if (update_label_text(game, &g_slider_value_labels[i],
				g_slider_value_cache[i], sizeof(g_slider_value_cache[i]),
				value_text))
		{
			value_rect = rect_make(slider_rect.x + slider_rect.w
					+ CONFIG_MODAL_SLIDER_VALUE_GAP,
					row_y + (CONFIG_MODAL_ROW_HEIGHT
						- (int)g_slider_value_labels[i]->height) / 2,
					CONFIG_MODAL_SLIDER_VALUE_W,
					(int)g_slider_value_labels[i]->height);
			if (value_rect.x + (int)g_slider_value_labels[i]->width
				> right.x + right.w - CONFIG_MODAL_CARD_PADDING)
				value_rect.x = right.x + right.w
					- CONFIG_MODAL_CARD_PADDING
					- (int)g_slider_value_labels[i]->width;
			g_slider_value_labels[i]->instances[0].x = value_rect.x;
			g_slider_value_labels[i]->instances[0].y = value_rect.y;
			set_image_enabled(g_slider_value_labels[i], true);
		}
		i++;
	}
}

void	draw_controls_options(t_game *game, t_rect panel)
{
	t_rect	card;
	t_rect	content;
	t_rect	keycap;
	t_border	border;
	int		i;
	int		row_y;
	int		selected;

	if (!game || !game->config_modal || !ensure_controls_labels(game))
		return ;
	content = build_content_rect(panel);
	card = rect_make(content.x, content.y, content.w, content.h);
	draw_card(game, card);
	g_controls_header->instances[0].x = card.x + CONFIG_MODAL_CARD_PADDING;
	g_controls_header->instances[0].y = card.y + CONFIG_MODAL_CARD_PADDING;
	set_image_enabled(g_controls_header, true);
	selected = config_controls_selected();
	if (config_controls_is_rebinding())
	{
		g_controls_prompt->instances[0].x = card.x + CONFIG_MODAL_CARD_PADDING;
		g_controls_prompt->instances[0].y = card.y + card.h
			- CONFIG_MODAL_CARD_PADDING - (int)g_controls_prompt->height;
		set_image_enabled(g_controls_prompt, true);
	}
	else
		set_image_enabled(g_controls_prompt, false);
	i = 0;
	while (i < CONFIG_MODAL_CONTROL_COUNT)
	{
		row_y = row_y_from_card(card, i);
		if (selected == i)
			draw_row_highlight(game, card, row_y);
		g_controls_labels[i]->instances[0].x = card.x
			+ CONFIG_MODAL_CARD_PADDING;
		g_controls_labels[i]->instances[0].y = row_y + 4;
		set_image_enabled(g_controls_labels[i], true);
		keycap = rect_make(card.x + card.w - CONFIG_MODAL_CARD_PADDING
				- CONFIG_MODAL_KEYCAP_W,
				row_y + (CONFIG_MODAL_ROW_HEIGHT - CONFIG_MODAL_KEYCAP_H) / 2,
				CONFIG_MODAL_KEYCAP_W, CONFIG_MODAL_KEYCAP_H);
		draw_rect(game->config_modal, keycap, CONFIG_MODAL_KEYCAP_COLOR);
		border.area = keycap;
		border.thickness = 1;
		border.color = CONFIG_MODAL_KEYCAP_BORDER_COLOR;
		draw_border(game->config_modal, border);
		g_controls_key_labels[i]->instances[0].x = keycap.x
			+ (keycap.w - (int)g_controls_key_labels[i]->width) / 2;
		g_controls_key_labels[i]->instances[0].y = keycap.y
			+ (keycap.h - (int)g_controls_key_labels[i]->height) / 2;
		set_image_enabled(g_controls_key_labels[i], true);
		i++;
	}
}
