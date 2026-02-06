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
#include "logic_bonus.h"

static void	sync_settings_selection(t_game *game)
{
	if (!game)
		return ;
	if (game->menu.current_tab == CONFIG_MENU_GENERAL
		&& game->menu.options.selected >= CONFIG_MODAL_TOGGLE_COUNT)
		game->menu.options.selected = CONFIG_OPTION_SHOW_FPS;
	else if (game->menu.current_tab == CONFIG_MENU_TUNING
		&& game->menu.options.selected < CONFIG_MODAL_TOGGLE_COUNT)
		game->menu.options.selected = CONFIG_OPTION_FPS_LIMIT;
}

static void	advance_menu_section(t_game *game, int delta)
{
	int	next;

	if (!game)
		return ;
	next = game->menu.current_tab + delta;
	if (next < CONFIG_MENU_GENERAL)
		next = CONFIG_MENU_SECTION_COUNT - 1;
	else if (next >= CONFIG_MENU_SECTION_COUNT)
		next = CONFIG_MENU_GENERAL;
	if (next == game->menu.current_tab)
		return ;
	game->menu.current_tab = next;
	sync_settings_selection(game);
	config_controls_cancel_rebind(game);
	draw_modal_layout(game);
}

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

void	update_config_modal(t_game *game)
{
	int32_t	mouse_x;
	int32_t	mouse_y;
	bool	holding_q;
	bool	dragging;

	if (!game || !game->mlx || !game->menu.open)
		return ;
	holding_q = mlx_is_key_down(game->mlx,
			game->menu.controls_key_codes[ACTION_QUIT])
		|| game->controller.menu_quit_held;
	if (game->menu.controls_rebinding)
		holding_q = false;
	if (holding_q)
		game->menu.quit_hold_time += game->mlx->delta_time;
	else
		game->menu.quit_hold_time = 0.0;
	if (game->menu.quit_hold_time >= CONFIG_MODAL_QUIT_HOLD_SECONDS)
	{
		mlx_close_window(game->mlx);
		return ;
	}
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

void	config_modal_handle_key(t_game *game, mlx_key_data_t keydata)
{
	keys_t	accept_key;
	bool	confirm_pressed;
	int		column;

	if (!game || !game->menu.open)
		return ;
	if (keydata.action != MLX_PRESS)
		return ;
	if (config_controls_handle_key(game, keydata))
		return ;
	accept_key = game->menu.controls_key_codes[ACTION_ACCEPT];
	confirm_pressed = (keydata.key == accept_key || keydata.key == MLX_KEY_SPACE);
	if (keydata.key == MLX_KEY_LEFT
		&& game->menu.current_column == CONFIG_MENU_COLUMN_RIGHT)
	{
		config_controls_cancel_rebind(game);
		game->menu.current_column = CONFIG_MENU_COLUMN_LEFT;
		draw_modal_layout(game);
		return ;
	}
	if (keydata.key == MLX_KEY_RIGHT
		&& game->menu.current_column == CONFIG_MENU_COLUMN_LEFT)
	{
		config_controls_cancel_rebind(game);
		game->menu.current_column = CONFIG_MENU_COLUMN_RIGHT;
		draw_modal_layout(game);
		return ;
	}
	if (game->menu.current_column == CONFIG_MENU_COLUMN_LEFT)
	{
		if (keydata.key == MLX_KEY_UP)
			advance_menu_section(game, -1);
		else if (keydata.key == MLX_KEY_DOWN)
			advance_menu_section(game, 1);
		else if (confirm_pressed)
		{
			game->menu.current_column = CONFIG_MENU_COLUMN_RIGHT;
			draw_modal_layout(game);
		}
		return ;
	}
	if (game->menu.current_tab == CONFIG_MENU_KEYBOARD_CONTROLS
		|| game->menu.current_tab == CONFIG_MENU_CONTROLLER_CONTROLS)
	{
		if (game->menu.current_tab == CONFIG_MENU_CONTROLLER_CONTROLS)
			column = CONTROLS_COLUMN_CONTROLLER;
		else
			column = CONTROLS_COLUMN_KEYBOARD;
		game->menu.controls_column = column;
		if (keydata.key == MLX_KEY_UP)
			config_controls_select(game, -1);
		else if (keydata.key == MLX_KEY_DOWN)
			config_controls_select(game, 1);
		else if (confirm_pressed)
			config_controls_begin_rebind(game);
		return ;
	}
	if (keydata.key == MLX_KEY_UP)
		config_option_select(game, -1);
	else if (keydata.key == MLX_KEY_DOWN)
		config_option_select(game, 1);
	else if (confirm_pressed)
		config_option_toggle(game, game->menu.options.selected);
	else if (keydata.key == MLX_KEY_A
		&& game->menu.current_tab == CONFIG_MENU_TUNING)
		config_option_adjust(game, -1);
	else if (keydata.key == MLX_KEY_D
		&& game->menu.current_tab == CONFIG_MENU_TUNING)
		config_option_adjust(game, 1);
}
