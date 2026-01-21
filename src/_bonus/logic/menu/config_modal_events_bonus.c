/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_modal_events_bonus.c                        :+:      :+:    :+:   */
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

static bool	is_quit_hovered(t_game *game, int32_t mouse_x, int32_t mouse_y)
{
	if (!game)
		return (false);
	if (mouse_x < game->config_quit_x || mouse_y < game->config_quit_y)
		return (false);
	if (mouse_x >= game->config_quit_x + game->config_quit_w)
		return (false);
	if (mouse_y >= game->config_quit_y + game->config_quit_h)
		return (false);
	return (true);
}

static float	get_quit_progress(const t_game *game)
{
	float	progress;

	if (!game || game->config_quit_hold_time <= 0.0)
		return (0.0f);
	progress = game->config_quit_hold_time / CONFIG_MODAL_QUIT_HOLD_SECONDS;
	if (progress > 1.0f)
		return (1.0f);
	return (progress);
}

void	render_quit_button(t_game *game)
{
	t_border	border;
	t_rect		rect;
	float		progress;
	uint32_t	color;

	if (!game || !game->config_modal || game->config_current_tab != 0)
		return ;
	rect = rect_make(game->config_quit_x, game->config_quit_y,
			game->config_quit_w, game->config_quit_h);
	color = CONFIG_MODAL_QUIT_BTN_COLOR;
	if (game->config_quit_hover)
		color = CONFIG_MODAL_QUIT_BTN_HOVER_COLOR;
	draw_rect(game->config_modal, rect, color);
	border.area = rect;
	border.thickness = CONFIG_MODAL_BORDER_THICKNESS;
	border.color = CONFIG_MODAL_BORDER_COLOR;
	draw_border(game->config_modal, border);
	border.thickness = CONFIG_MODAL_QUIT_PROGRESS_THICKNESS;
	border.color = CONFIG_MODAL_QUIT_PROGRESS_COLOR;
	progress = get_quit_progress(game);
	if (progress > 0.0f)
		draw_progress_border(game->config_modal, border, progress);
}

void	update_config_modal(t_game *game)
{
	int32_t	mouse_x;
	int32_t	mouse_y;
	bool	holding_q;
	bool	dragging;

	if (!game || !game->mlx || !game->config_modal_open)
		return ;
	if (game->config_current_tab != 0)
		return ;
	mlx_get_mouse_pos(game->mlx, &mouse_x, &mouse_y);
	game->config_quit_hover = is_quit_hovered(game, mouse_x, mouse_y);
	holding_q = mlx_is_key_down(game->mlx, MLX_KEY_Q);
	if (holding_q)
		game->config_quit_hold_time += game->mlx->delta_time;
	else
		game->config_quit_hold_time = 0.0;
	if (game->config_quit_hold_time >= CONFIG_MODAL_QUIT_HOLD_SECONDS)
	{
		mlx_close_window(game->mlx);
		return ;
	}
	dragging = config_option_drag_update(game, mouse_x, mouse_y,
			mlx_is_mouse_down(game->mlx, MLX_MOUSE_BUTTON_LEFT));
	if (dragging)
	{
		draw_modal_layout(game);
		return ;
	}
	render_quit_button(game);
}

void	config_modal_handle_key(t_game *game, mlx_key_data_t keydata)
{
	if (!game || !game->config_modal_open)
		return ;
	if (keydata.action != MLX_PRESS)
		return ;
	if (config_controls_handle_key(game, keydata))
		return ;
	if (keydata.key == MLX_KEY_LEFT && game->config_current_tab > 0)
	{
		game->config_current_tab--;
		config_controls_cancel_rebind();
		draw_modal_layout(game);
	}
	else if (keydata.key == MLX_KEY_RIGHT && game->config_current_tab < 1)
	{
		game->config_current_tab++;
		config_controls_cancel_rebind();
		draw_modal_layout(game);
	}
	if (game->config_current_tab != 0)
	{
		if (keydata.key == MLX_KEY_UP)
			config_controls_select(game, -1);
		else if (keydata.key == MLX_KEY_DOWN)
			config_controls_select(game, 1);
		else if (keydata.key == MLX_KEY_ENTER
			|| keydata.key == MLX_KEY_KP_ENTER
			|| keydata.key == MLX_KEY_SPACE)
			config_controls_begin_rebind(game);
		return ;
	}
	if (keydata.key == MLX_KEY_UP)
		config_option_select(game, -1);
	else if (keydata.key == MLX_KEY_DOWN)
		config_option_select(game, 1);
	else if (keydata.key == MLX_KEY_ENTER || keydata.key == MLX_KEY_KP_ENTER
		|| keydata.key == MLX_KEY_SPACE)
		config_option_toggle(game, game->config_options.selected);
	else if (keydata.key == MLX_KEY_A)
		config_option_adjust(game, -1);
	else if (keydata.key == MLX_KEY_D)
		config_option_adjust(game, 1);
}
