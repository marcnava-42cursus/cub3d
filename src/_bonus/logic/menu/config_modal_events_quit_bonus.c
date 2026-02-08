/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_modal_events_quit_bonus.c                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 06:04:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/08 06:04:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config_bonus.h"
#include "structs.h"
#include "logic_bonus.h"

static bool	is_quit_hovered(t_game *game, int32_t mouse_x, int32_t mouse_y)
{
	if (!game)
		return (false);
	if (mouse_x < game->menu.quit_x || mouse_y < game->menu.quit_y)
		return (false);
	if (mouse_x >= game->menu.quit_x + game->menu.quit_w)
		return (false);
	if (mouse_y >= game->menu.quit_y + game->menu.quit_h)
		return (false);
	return (true);
}

static float	get_quit_progress(const t_game *game)
{
	float	progress;

	if (!game || game->menu.quit_hold_time <= 0.0)
		return (0.0f);
	progress = game->menu.quit_hold_time / CONFIG_MODAL_QUIT_HOLD_SECONDS;
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

	if (!game || !game->menu.modal)
		return ;
	rect = rect_make(game->menu.quit_x, game->menu.quit_y,
			game->menu.quit_w, game->menu.quit_h);
	color = CONFIG_MODAL_QUIT_BTN_COLOR;
	if (game->menu.quit_hover)
		color = CONFIG_MODAL_QUIT_BTN_HOVER_COLOR;
	draw_rect(game->menu.modal, rect, color);
	border.area = rect;
	border.thickness = CONFIG_MODAL_BORDER_THICKNESS;
	border.color = CONFIG_MODAL_BORDER_COLOR;
	draw_border(game->menu.modal, border);
	border.thickness = CONFIG_MODAL_QUIT_PROGRESS_THICKNESS;
	border.color = CONFIG_MODAL_QUIT_PROGRESS_COLOR;
	progress = get_quit_progress(game);
	if (progress > 0.0f)
		draw_progress_border(game->menu.modal, border, progress);
}

static bool	update_quit_hold_state(t_game *game)
{
	bool	holding_q;

	holding_q = mlx_is_key_down(game->mlx,
			game->menu.controls_key_codes[ACTION_QUIT])
		|| game->controller.menu_quit_held;
	if (game->menu.controls_rebinding)
		holding_q = false;
	if (holding_q)
		game->menu.quit_hold_time += game->mlx->delta_time;
	else
		game->menu.quit_hold_time = 0.0;
	if (game->menu.quit_hold_time < CONFIG_MODAL_QUIT_HOLD_SECONDS)
		return (false);
	mlx_close_window(game->mlx);
	return (true);
}

void	update_config_modal(t_game *game)
{
	int32_t	mouse_x;
	int32_t	mouse_y;
	bool	dragging;

	if (!game || !game->mlx || !game->menu.open)
		return ;
	if (update_quit_hold_state(game))
		return ;
	mlx_get_mouse_pos(game->mlx, &mouse_x, &mouse_y);
	game->menu.quit_hover = is_quit_hovered(game, mouse_x, mouse_y);
	dragging = config_option_drag_update(game, mouse_x, mouse_y,
			mlx_is_mouse_down(game->mlx, MLX_MOUSE_BUTTON_LEFT));
	if (dragging)
	{
		draw_modal_layout(game);
		return ;
	}
	render_quit_button(game);
}
