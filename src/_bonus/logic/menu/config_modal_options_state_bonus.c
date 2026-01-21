/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_modal_options_state_bonus.c                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/06 15:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/01/06 15:00:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config_bonus.h"
#include "structs.h"
#include "logic.h"
#include <stdio.h>

enum
{
	FPS_LIMIT_STEP_COUNT = 6
};

static const int	g_fps_limits[FPS_LIMIT_STEP_COUNT]
	= {10, 30, 60, 120, 240, -1};
static int			g_pending_slider_index = -1;
static int			g_pending_slider_value = -1;
static int			g_slider_drag_index = -1;
static int			g_controls_selected = 0;
static bool			g_controls_rebinding = false;
static int			g_controls_rebind_target = -1;
static char			g_controls_key_text[CONFIG_MODAL_CONTROL_COUNT]
	[CONFIG_MODAL_KEY_LABEL_LEN] = {
		"W", "S", "D", "A", "RIGHT", "LEFT", "E", "R", "ESC", "M"
	};

static int	clamp_value(int value, int min, int max)
{
	if (value < min)
		return (min);
	if (value > max)
		return (max);
	return (value);
}

static bool	*toggle_value_ptr(t_game *game, int index)
{
	if (!game)
		return (NULL);
	if (index == CONFIG_OPTION_SHOW_FPS)
		return (&game->config_options.show_fps);
	if (index == CONFIG_OPTION_CROSSHAIR)
		return (&game->config_options.crosshair);
	if (index == CONFIG_OPTION_MINIMAP)
		return (&game->config_options.minimap);
	if (index == CONFIG_OPTION_FULLSCREEN)
		return (&game->config_options.fullscreen);
	return (NULL);
}

static int	*slider_value_ptr(t_game *game, int slider)
{
	if (!game)
		return (NULL);
	if (slider == CONFIG_SLIDER_GAME_SPEED)
		return (&game->config_options.game_speed);
	if (slider == CONFIG_SLIDER_FPS_LIMIT)
		return (&game->config_options.fps_limit_index);
	if (slider == CONFIG_SLIDER_MOUSE_SENS)
		return (&game->config_options.mouse_sens);
	return (NULL);
}

static int	slider_percent(t_game *game, int slider)
{
	int	*value;

	value = slider_value_ptr(game, slider);
	if (!value)
		return (0);
	if (slider == CONFIG_SLIDER_FPS_LIMIT)
		return (((*value) * 100) / (FPS_LIMIT_STEP_COUNT - 1));
	return (((*value - 1) * 100) / 99);
}

static void	slider_set_from_percent(t_game *game, int slider, int percent)
{
	int	value;
	int	*target;

	target = slider_value_ptr(game, slider);
	if (!target)
		return ;
	percent = clamp_value(percent, 0, 100);
	if (slider == CONFIG_SLIDER_FPS_LIMIT)
	{
		value = (percent * (FPS_LIMIT_STEP_COUNT - 1) + 50) / 100;
		*target = clamp_value(value, 0, FPS_LIMIT_STEP_COUNT - 1);
		return ;
	}
	value = 1 + (percent * 99 + 50) / 100;
	*target = clamp_value(value, 1, 100);
}

static void	slider_step(t_game *game, int slider, int delta)
{
	int	*target;

	target = slider_value_ptr(game, slider);
	if (!target)
		return ;
	if (slider == CONFIG_SLIDER_FPS_LIMIT)
	{
		*target = clamp_value(*target + delta,
				0, FPS_LIMIT_STEP_COUNT - 1);
		return ;
	}
	*target = clamp_value(
			*target + delta * CONFIG_MODAL_SLIDER_STEP,
			1, 100);
}

static const char	*key_name_from_mlx(keys_t key, char *buffer,
			size_t buffer_size)
{
	if (key >= MLX_KEY_A && key <= MLX_KEY_Z)
	{
		buffer[0] = (char)('A' + (key - MLX_KEY_A));
		buffer[1] = '\0';
		return (buffer);
	}
	if (key >= MLX_KEY_0 && key <= MLX_KEY_9)
	{
		buffer[0] = (char)('0' + (key - MLX_KEY_0));
		buffer[1] = '\0';
		return (buffer);
	}
	if (key >= MLX_KEY_F1 && key <= MLX_KEY_F12)
	{
		snprintf(buffer, buffer_size, "F%d", 1 + (key - MLX_KEY_F1));
		return (buffer);
	}
	if (key == MLX_KEY_SPACE)
		return ("SPACE");
	if (key == MLX_KEY_ENTER || key == MLX_KEY_KP_ENTER)
		return ("ENTER");
	if (key == MLX_KEY_LEFT)
		return ("LEFT");
	if (key == MLX_KEY_RIGHT)
		return ("RIGHT");
	if (key == MLX_KEY_UP)
		return ("UP");
	if (key == MLX_KEY_DOWN)
		return ("DOWN");
	if (key == MLX_KEY_TAB)
		return ("TAB");
	if (key == MLX_KEY_BACKSPACE)
		return ("BACK");
	if (key == MLX_KEY_ESCAPE)
		return ("ESC");
	if (key == MLX_KEY_LEFT_SHIFT)
		return ("LSHIFT");
	if (key == MLX_KEY_RIGHT_SHIFT)
		return ("RSHIFT");
	if (key == MLX_KEY_LEFT_CONTROL)
		return ("LCTRL");
	if (key == MLX_KEY_RIGHT_CONTROL)
		return ("RCTRL");
	if (key == MLX_KEY_LEFT_ALT)
		return ("LALT");
	if (key == MLX_KEY_RIGHT_ALT)
		return ("RALT");
	if (key == MLX_KEY_LEFT_SUPER)
		return ("LGUI");
	if (key == MLX_KEY_RIGHT_SUPER)
		return ("RGUI");
	if (key == MLX_KEY_CAPS_LOCK)
		return ("CAPS");
	if (key == MLX_KEY_MINUS)
		return ("-");
	if (key == MLX_KEY_EQUAL)
		return ("=");
	if (key == MLX_KEY_COMMA)
		return (",");
	if (key == MLX_KEY_PERIOD)
		return (".");
	snprintf(buffer, buffer_size, "KEY%d", (int)key);
	return (buffer);
}

bool	config_option_toggle_state(t_game *game, int index)
{
	bool	*value;

	if (index < 0 || index >= CONFIG_MODAL_TOGGLE_COUNT)
		return (false);
	value = toggle_value_ptr(game, index);
	if (!value)
		return (false);
	return (*value);
}

int	config_option_slider_value(t_game *game, int index)
{
	int	slider;

	if (index < CONFIG_MODAL_TOGGLE_COUNT
		|| index >= CONFIG_MODAL_OPTION_COUNT)
		return (0);
	slider = index - CONFIG_MODAL_TOGGLE_COUNT;
	return (slider_percent(game, slider));
}

void	config_option_slider_text(t_game *game, int index, char *buffer,
			size_t buffer_size)
{
	int	slider;
	int	value;
	int	*slider_value;

	if (!buffer || buffer_size == 0)
		return ;
	buffer[0] = '\0';
	if (index < CONFIG_MODAL_TOGGLE_COUNT
		|| index >= CONFIG_MODAL_OPTION_COUNT)
		return ;
	slider = index - CONFIG_MODAL_TOGGLE_COUNT;
	slider_value = slider_value_ptr(game, slider);
	if (!slider_value)
		return ;
	if (slider == CONFIG_SLIDER_FPS_LIMIT)
	{
		value = g_fps_limits[*slider_value];
		if (value < 0)
			snprintf(buffer, buffer_size, "UNLIMITED");
		else
			snprintf(buffer, buffer_size, "%d", value);
		return ;
	}
	snprintf(buffer, buffer_size, "%d", *slider_value);
}

void	config_options_init(t_game *game)
{
	if (!game)
		return ;
	game->config_options.show_fps = false;
	game->config_options.crosshair = true;
	game->config_options.minimap = true;
	game->config_options.fullscreen = false;
	game->config_options.game_speed = 70;
	game->config_options.fps_limit_index = 2;
	game->config_options.mouse_sens = 50;
	game->config_options.selected = 0;
}

void	config_options_reset(t_game *game)
{
	if (!game)
		return ;
	game->config_options.selected = 0;
	g_controls_selected = 0;
	g_controls_rebinding = false;
	g_controls_rebind_target = -1;
	g_pending_slider_index = -1;
	g_pending_slider_value = -1;
	g_slider_drag_index = -1;
}

void	config_option_select(t_game *game, int delta)
{
	int	selected;

	if (!game)
		return ;
	selected = game->config_options.selected + delta;
	if (selected < 0)
		selected = CONFIG_MODAL_OPTION_COUNT - 1;
	else if (selected >= CONFIG_MODAL_OPTION_COUNT)
		selected = 0;
	game->config_options.selected = selected;
	draw_modal_layout(game);
}

void	config_option_toggle(t_game *game, int index)
{
	int			slider;
	const char	*name;

	if (!game || index < 0 || index >= CONFIG_MODAL_OPTION_COUNT)
		return ;
	game->config_options.selected = index;
	if (index < CONFIG_MODAL_TOGGLE_COUNT)
	{
		bool	*toggle;

		toggle = toggle_value_ptr(game, index);
		if (!toggle)
			return ;
		if (index == CONFIG_OPTION_SHOW_FPS)
			name = "FPS";
		else if (index == CONFIG_OPTION_CROSSHAIR)
			name = "Crosshair";
		else if (index == CONFIG_OPTION_MINIMAP)
			name = "Minimap";
		else
			name = "Fullscreen";
		*toggle = !(*toggle);
		printf("Config: %s %s\n", name,
			*toggle ? "ON" : "OFF");
	}
	else
	{
		char	value_text[CONFIG_MODAL_SLIDER_VALUE_LEN];

		slider = index - CONFIG_MODAL_TOGGLE_COUNT;
		if (g_pending_slider_index == index && g_pending_slider_value >= 0)
			slider_set_from_percent(game, slider, g_pending_slider_value);
		else
			slider_step(game, slider, 1);
		g_pending_slider_index = -1;
		g_pending_slider_value = -1;
		if (index == CONFIG_OPTION_GAME_SPEED)
			name = "Game Speed";
		else if (index == CONFIG_OPTION_FPS_LIMIT)
			name = "FPS Limit";
		else
			name = "Mouse Sensitivity";
		config_option_slider_text(game, index, value_text, sizeof(value_text));
		printf("Config: %s %s\n", name, value_text);
	}
	draw_modal_layout(game);
}

void	config_option_adjust(t_game *game, int delta)
{
	int	index;
	int	slider;

	if (!game)
		return ;
	index = game->config_options.selected;
	if (index < CONFIG_MODAL_TOGGLE_COUNT
		|| index >= CONFIG_MODAL_OPTION_COUNT)
		return ;
	slider = index - CONFIG_MODAL_TOGGLE_COUNT;
	slider_step(game, slider, delta);
	g_pending_slider_index = -1;
	g_pending_slider_value = -1;
	draw_modal_layout(game);
}

static t_rect	build_content_rect(t_game *game)
{
	t_rect	body;

	body = rect_make(game->config_panel_x, game->config_panel_y + TAB_HEIGHT,
			game->config_panel_w, game->config_panel_h - TAB_HEIGHT);
	return (rect_make(body.x + CONFIG_MODAL_CONTENT_PADDING,
			body.y + CONFIG_MODAL_CONTENT_PADDING,
			body.w - 2 * CONFIG_MODAL_CONTENT_PADDING,
			body.h - 2 * CONFIG_MODAL_CONTENT_PADDING));
}

static void	build_cards(t_game *game, t_rect *left, t_rect *right)
{
	t_rect	content;
	int		card_w;
	int		card_h;

	content = build_content_rect(game);
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

static int	slider_width_for_card(t_rect card)
{
	int	track_w;
	int	slider_w;

	track_w = card.w - 2 * CONFIG_MODAL_CARD_PADDING
		- CONFIG_MODAL_SLIDER_VALUE_W - CONFIG_MODAL_SLIDER_VALUE_GAP;
	slider_w = CONFIG_MODAL_SLIDER_W;
	if (slider_w > track_w)
		slider_w = track_w;
	if (slider_w < 80)
		slider_w = card.w - 2 * CONFIG_MODAL_CARD_PADDING;
	return (slider_w);
}

int	config_option_from_pos(t_game *game, int32_t mx, int32_t my)
{
	t_rect	left;
	t_rect	right;
	t_rect	slider;
	int		index;
	int		row_y;
	int		option;
	int		slider_w;
	int		track_max;

	if (!game || game->config_current_tab != 0)
		return (-1);
	build_cards(game, &left, &right);
	index = 0;
	while (index < CONFIG_MODAL_TOGGLE_COUNT)
	{
		row_y = row_y_from_card(left, index);
		if (mx >= left.x && mx <= left.x + left.w
			&& my >= row_y && my <= row_y + CONFIG_MODAL_ROW_HEIGHT)
		{
			g_pending_slider_index = -1;
			g_pending_slider_value = -1;
			return (index);
		}
		index++;
	}
	slider_w = slider_width_for_card(right);
	index = 0;
	while (index < CONFIG_MODAL_SLIDER_COUNT)
	{
		row_y = row_y_from_card(right, index);
		option = CONFIG_MODAL_TOGGLE_COUNT + index;
		if (mx >= right.x && mx <= right.x + right.w
			&& my >= row_y && my <= row_y + CONFIG_MODAL_ROW_HEIGHT)
		{
			game->config_options.selected = option;
			slider = rect_make(right.x + right.w - CONFIG_MODAL_CARD_PADDING
					- CONFIG_MODAL_SLIDER_VALUE_W
					- CONFIG_MODAL_SLIDER_VALUE_GAP - slider_w,
					row_y + (CONFIG_MODAL_ROW_HEIGHT - CONFIG_MODAL_SLIDER_H)
					/ 2, slider_w, CONFIG_MODAL_SLIDER_H);
			if (mx < slider.x || mx > slider.x + slider.w)
			{
				g_pending_slider_index = -1;
				g_pending_slider_value = -1;
				g_slider_drag_index = -1;
				draw_modal_layout(game);
				return (-1);
			}
			track_max = slider.w - 1;
			if (track_max <= 0)
				g_pending_slider_value = 0;
			else
				g_pending_slider_value = ((mx - slider.x) * 100) / track_max;
			g_pending_slider_index = option;
			g_slider_drag_index = option;
			return (option);
		}
		index++;
	}
	return (-1);
}

bool	config_option_drag_update(t_game *game, int32_t mx, int32_t my,
			bool mouse_down)
{
	t_rect	left;
	t_rect	right;
	t_rect	slider;
	int		index;
	int		row_y;
	int		slider_w;
	int		track_max;
	int		option;
	int		slider_idx;
	int		percent;
	int		before;

	if (!game || game->config_current_tab != 0)
		return (false);
	if (!mouse_down)
	{
		g_slider_drag_index = -1;
		g_pending_slider_index = -1;
		g_pending_slider_value = -1;
		return (false);
	}
	build_cards(game, &left, &right);
	slider_w = slider_width_for_card(right);
	if (g_slider_drag_index < CONFIG_MODAL_TOGGLE_COUNT
		|| g_slider_drag_index >= CONFIG_MODAL_OPTION_COUNT)
	{
		index = 0;
		while (index < CONFIG_MODAL_SLIDER_COUNT)
		{
			row_y = row_y_from_card(right, index);
			option = CONFIG_MODAL_TOGGLE_COUNT + index;
			slider = rect_make(right.x + right.w - CONFIG_MODAL_CARD_PADDING
					- CONFIG_MODAL_SLIDER_VALUE_W
					- CONFIG_MODAL_SLIDER_VALUE_GAP - slider_w,
					row_y + (CONFIG_MODAL_ROW_HEIGHT - CONFIG_MODAL_SLIDER_H)
					/ 2, slider_w, CONFIG_MODAL_SLIDER_H);
			if (mx >= slider.x && mx <= slider.x + slider.w
				&& my >= row_y && my <= row_y + CONFIG_MODAL_ROW_HEIGHT)
			{
				g_slider_drag_index = option;
				break ;
			}
			index++;
		}
	}
	if (g_slider_drag_index < CONFIG_MODAL_TOGGLE_COUNT
		|| g_slider_drag_index >= CONFIG_MODAL_OPTION_COUNT)
		return (false);
	slider_idx = g_slider_drag_index - CONFIG_MODAL_TOGGLE_COUNT;
	row_y = row_y_from_card(right, slider_idx);
	slider = rect_make(right.x + right.w - CONFIG_MODAL_CARD_PADDING
			- CONFIG_MODAL_SLIDER_VALUE_W - CONFIG_MODAL_SLIDER_VALUE_GAP
			- slider_w,
			row_y + (CONFIG_MODAL_ROW_HEIGHT - CONFIG_MODAL_SLIDER_H) / 2,
			slider_w, CONFIG_MODAL_SLIDER_H);
	track_max = slider.w - 1;
	if (track_max <= 0)
		percent = 0;
	else if (mx <= slider.x)
		percent = 0;
	else if (mx >= slider.x + slider.w)
		percent = 100;
	else
		percent = ((mx - slider.x) * 100) / track_max;
	before = 0;
	{
		int	*target;

		target = slider_value_ptr(game, slider_idx);
		if (target)
			before = *target;
	}
	slider_set_from_percent(game, slider_idx, percent);
	game->config_options.selected = g_slider_drag_index;
	{
		int	*target;

		target = slider_value_ptr(game, slider_idx);
		if (!target)
			return (false);
		return (before != *target);
	}
}

void	config_controls_select(t_game *game, int delta)
{
	int	selected;

	if (!game || g_controls_rebinding)
		return ;
	selected = g_controls_selected + delta;
	if (selected < 0)
		selected = CONFIG_MODAL_CONTROL_COUNT - 1;
	else if (selected >= CONFIG_MODAL_CONTROL_COUNT)
		selected = 0;
	g_controls_selected = selected;
	draw_modal_layout(game);
}

void	config_controls_begin_rebind(t_game *game)
{
	if (!game)
		return ;
	g_controls_rebinding = true;
	g_controls_rebind_target = g_controls_selected;
	draw_modal_layout(game);
}

void	config_controls_cancel_rebind(void)
{
	g_controls_rebinding = false;
	g_controls_rebind_target = -1;
}

bool	config_controls_handle_key(t_game *game, mlx_key_data_t keydata)
{
	char		name_buf[CONFIG_MODAL_KEY_LABEL_LEN];
	const char	*name;

	if (!g_controls_rebinding)
		return (false);
	if (keydata.action != MLX_PRESS)
		return (true);
	if (keydata.key == MLX_KEY_BACKSPACE)
	{
		config_controls_cancel_rebind();
		draw_modal_layout(game);
		return (true);
	}
	if (g_controls_rebind_target < 0
		|| g_controls_rebind_target >= CONFIG_MODAL_CONTROL_COUNT)
	{
		config_controls_cancel_rebind();
		draw_modal_layout(game);
		return (true);
	}
	name = key_name_from_mlx(keydata.key, name_buf, sizeof(name_buf));
	ft_strlcpy(g_controls_key_text[g_controls_rebind_target], name,
		CONFIG_MODAL_KEY_LABEL_LEN);
	config_controls_cancel_rebind();
	draw_modal_layout(game);
	return (true);
}

int	config_controls_selected(void)
{
	return (g_controls_selected);
}

bool	config_controls_is_rebinding(void)
{
	return (g_controls_rebinding);
}

const char	*config_controls_key_text(int index)
{
	if (index < 0 || index >= CONFIG_MODAL_CONTROL_COUNT)
		return ("");
	return (g_controls_key_text[index]);
}
