/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_modal_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 00:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/12/14 00:00:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "logic.h"

# define CONFIG_MODAL_BG_COLOR 0x000000C0
# define CONFIG_MODAL_PANEL_COLOR 0x1E1E1ED0
# define CONFIG_MODAL_BORDER_COLOR 0xCCCCCCFF
# define CONFIG_MODAL_BORDER_THICKNESS 2
# define CONFIG_MODAL_QUIT_TEXT "QUIT (Q)"
# define CONFIG_MODAL_QUIT_BTN_COLOR 0x555555FF
# define CONFIG_MODAL_QUIT_BTN_HOVER_COLOR 0x444444FF
# define CONFIG_MODAL_QUIT_BTN_MARGIN 24
# define CONFIG_MODAL_QUIT_BTN_W 160
# define CONFIG_MODAL_QUIT_BTN_H 52
# define CONFIG_MODAL_QUIT_HOLD_SECONDS 1.2
# define CONFIG_MODAL_QUIT_PROGRESS_COLOR 0xB33000FF
# define CONFIG_MODAL_QUIT_PROGRESS_THICKNESS 4

static void	set_image_enabled(mlx_image_t *img, bool enabled)
{
	size_t	i;

	if (!img)
		return ;
	i = 0;
	while (i < img->count)
	{
		img->instances[i].enabled = enabled;
		i++;
	}
}

static void	draw_rect(mlx_image_t *img, int x, int y, int w, int h,
		uint32_t color)
{
	int		yy;
	int		xx;
	uint8_t	r;
	uint8_t	g;
	uint8_t	b;
	uint8_t	a;
	uint8_t	*row;

	if (!img || w <= 0 || h <= 0)
		return ;
	if (x < 0)
	{
		w += x;
		x = 0;
	}
	if (y < 0)
	{
		h += y;
		y = 0;
	}
	if (x >= (int)img->width || y >= (int)img->height)
		return ;
	if (x + w > (int)img->width)
		w = (int)img->width - x;
	if (y + h > (int)img->height)
		h = (int)img->height - y;
	if (w <= 0 || h <= 0)
		return ;
	r = (uint8_t)(color >> 24);
	g = (uint8_t)(color >> 16);
	b = (uint8_t)(color >> 8);
	a = (uint8_t)(color & 0xFF);
	yy = 0;
	while (yy < h)
	{
		row = &img->pixels[((y + yy) * img->width + x) * 4];
		xx = 0;
		while (xx < w)
		{
			row[0] = r;
			row[1] = g;
			row[2] = b;
			row[3] = a;
			row += 4;
			xx++;
		}
		yy++;
	}
}

static void	draw_border(mlx_image_t *img, int x, int y, int w, int h,
		int thickness, uint32_t color)
{
	if (thickness <= 0)
		return ;
	draw_rect(img, x, y, w, thickness, color);
	draw_rect(img, x, y + h - thickness, w, thickness, color);
	draw_rect(img, x, y, thickness, h, color);
	draw_rect(img, x + w - thickness, y, thickness, h, color);
}

static void	draw_progress_border(mlx_image_t *img, int x, int y, int w, int h,
		int thickness, uint32_t color, float progress)
{
	float	remaining;
	int		perimeter;
	int		edge;

	if (!img || thickness <= 0)
		return ;
	if (w <= 0 || h <= 0)
		return ;
	if (progress <= 0.0f)
		return ;
	if (progress > 1.0f)
		progress = 1.0f;
	perimeter = 2 * (w + h);
	remaining = progress * (float)perimeter;
	edge = (remaining > (float)w) ? w : (int)remaining;
	draw_rect(img, x, y, edge, thickness, color);
	remaining -= (float)edge;
	if (remaining <= 0.0f)
		return ;
	edge = (remaining > (float)h) ? h : (int)remaining;
	draw_rect(img, x + w - thickness, y, thickness, edge, color);
	remaining -= (float)edge;
	if (remaining <= 0.0f)
		return ;
	edge = (remaining > (float)w) ? w : (int)remaining;
	draw_rect(img, x + w - edge, y + h - thickness, edge, thickness, color);
	remaining -= (float)edge;
	if (remaining <= 0.0f)
		return ;
	edge = (remaining > (float)h) ? h : (int)remaining;
	draw_rect(img, x, y + h - edge, thickness, edge, color);
}

static float	get_quit_hold_progress(const t_game *game)
{
	if (!game)
		return (0.0f);
	if (game->config_quit_hold_time <= 0.0)
		return (0.0f);
	return ((float)(game->config_quit_hold_time / CONFIG_MODAL_QUIT_HOLD_SECONDS));
}

static void	render_quit_button(t_game *game)
{
	mlx_image_t	*img;
	uint32_t	btn_color;
	float		progress;

	if (!game || !game->config_modal)
		return ;
	img = game->config_modal;
	btn_color = CONFIG_MODAL_QUIT_BTN_COLOR;
	if (game->config_quit_hover)
		btn_color = CONFIG_MODAL_QUIT_BTN_HOVER_COLOR;
	draw_rect(img, game->config_quit_x, game->config_quit_y,
		game->config_quit_w, game->config_quit_h, btn_color);
	progress = get_quit_hold_progress(game);
	draw_border(img, game->config_quit_x, game->config_quit_y,
		game->config_quit_w, game->config_quit_h,
		CONFIG_MODAL_BORDER_THICKNESS, CONFIG_MODAL_BORDER_COLOR);
	draw_progress_border(img, game->config_quit_x, game->config_quit_y,
		game->config_quit_w, game->config_quit_h,
		CONFIG_MODAL_QUIT_PROGRESS_THICKNESS, CONFIG_MODAL_QUIT_PROGRESS_COLOR,
		progress);
}

static void	draw_modal_layout(t_game *game)
{
	mlx_image_t	*img;
	int			panel_w;
	int			panel_h;
	int			panel_x;
	int			panel_y;

	if (!game || !game->config_modal)
		return ;
	img = game->config_modal;
	draw_rect(img, 0, 0, (int)img->width, (int)img->height,
		CONFIG_MODAL_BG_COLOR);
	panel_w = (int)(img->width * 0.70f);
	panel_h = (int)(img->height * 0.70f);
	panel_x = ((int)img->width - panel_w) / 2;
	panel_y = ((int)img->height - panel_h) / 2;
	draw_rect(img, panel_x, panel_y, panel_w, panel_h, CONFIG_MODAL_PANEL_COLOR);
	draw_border(img, panel_x, panel_y, panel_w, panel_h,
		CONFIG_MODAL_BORDER_THICKNESS, CONFIG_MODAL_BORDER_COLOR);
	game->config_quit_w = CONFIG_MODAL_QUIT_BTN_W;
	game->config_quit_h = CONFIG_MODAL_QUIT_BTN_H;
	game->config_quit_x = panel_x + panel_w - CONFIG_MODAL_BORDER_THICKNESS
		- CONFIG_MODAL_QUIT_BTN_MARGIN - game->config_quit_w;
	game->config_quit_y = panel_y + panel_h - CONFIG_MODAL_BORDER_THICKNESS
		- CONFIG_MODAL_QUIT_BTN_MARGIN - game->config_quit_h;
	render_quit_button(game);
}

static bool	ensure_config_modal_ready(t_game *game)
{
	if (!game || !game->mlx)
		return (false);
	if (!game->config_modal)
	{
		game->config_modal = mlx_new_image(game->mlx, MAX_WINDOW_WIDTH,
				MAX_WINDOW_HEIGHT);
		if (!game->config_modal)
			return (false);
		draw_modal_layout(game);
	}
	if (!game->config_modal_attached)
	{
		if (mlx_image_to_window(game->mlx, game->config_modal, 0, 0) < 0)
			return (false);
		game->config_modal_attached = true;
	}
	return (true);
}

static bool	ensure_quit_label_ready(t_game *game)
{
	int32_t	label_x;
	int32_t	label_y;

	if (!game || !game->mlx)
		return (false);
	if (!game->config_quit_label)
	{
		game->config_quit_label = mlx_put_string(game->mlx,
				CONFIG_MODAL_QUIT_TEXT, 0, 0);
		if (!game->config_quit_label || game->config_quit_label->count < 1)
			return (false);
	}
	label_x = game->config_quit_x
		+ (game->config_quit_w - (int32_t)game->config_quit_label->width) / 2;
	label_y = game->config_quit_y
		+ (game->config_quit_h - (int32_t)game->config_quit_label->height) / 2;
	game->config_quit_label->instances[0].x = label_x;
	game->config_quit_label->instances[0].y = label_y;
	return (true);
}

static bool	is_inside_quit_button(const t_game *game, int32_t x, int32_t y)
{
	if (!game)
		return (false);
	return (x >= game->config_quit_x && y >= game->config_quit_y
		&& x < game->config_quit_x + game->config_quit_w
		&& y < game->config_quit_y + game->config_quit_h);
}

static void	clear_input_state(t_game *game)
{
	if (!game)
		return ;
	game->key_w_pressed = false;
	game->key_s_pressed = false;
	game->key_a_pressed = false;
	game->key_d_pressed = false;
	game->key_left_pressed = false;
	game->key_right_pressed = false;
	game->mouse_initialized = false;
	game->mouse_delta_accumulated = 0.0f;
}

void	set_config_modal_visible(t_game *game, bool visible)
{
	if (!game || !game->mlx)
		return ;
	if (visible)
	{
		if (!ensure_config_modal_ready(game))
			return ;
		game->config_quit_hover = false;
		game->config_quit_hold_time = 0.0;
		draw_modal_layout(game);
		set_image_enabled(game->config_modal, true);
		if (ensure_quit_label_ready(game))
			set_image_enabled(game->config_quit_label, true);
		game->config_modal_open = true;
		clear_input_state(game);
		mlx_set_cursor_mode(game->mlx, MLX_MOUSE_NORMAL);
	}
	else
	{
		if (game->config_modal)
			set_image_enabled(game->config_modal, false);
		if (game->config_quit_label)
			set_image_enabled(game->config_quit_label, false);
		game->config_modal_open = false;
		clear_input_state(game);
		mlx_set_cursor_mode(game->mlx, MLX_MOUSE_DISABLED);
	}
}

void	toggle_config_modal(t_game *game)
{
	if (!game)
		return ;
	set_config_modal_visible(game, !game->config_modal_open);
}

bool	is_config_modal_open(const t_game *game)
{
	if (!game)
		return (false);
	return (game->config_modal_open);
}

void	update_config_modal(t_game *game)
{
	int32_t	mouse_x;
	int32_t	mouse_y;
	bool	hover;
	bool	holding_q;

	if (!game || !game->mlx || !game->config_modal_open)
		return ;
	mlx_get_mouse_pos(game->mlx, &mouse_x, &mouse_y);
	hover = is_inside_quit_button(game, mouse_x, mouse_y);
	game->config_quit_hover = hover;
	holding_q = mlx_is_key_down(game->mlx, MLX_KEY_Q);
	if (holding_q)
		game->config_quit_hold_time += game->delta_time;
	else
		game->config_quit_hold_time = 0.0;
	if (game->config_quit_hold_time >= CONFIG_MODAL_QUIT_HOLD_SECONDS)
	{
		mlx_close_window(game->mlx);
		return ;
	}
	render_quit_button(game);
}
