/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_bonus.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/06 15:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/01/21 18:35:23 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_BONUS_H
# define CONFIG_BONUS_H

# include <stdbool.h>
# include <stddef.h>
# include <stdint.h>
# include "MLX42/MLX42.h"

typedef struct s_game t_game;

# define CONFIG_MODAL_BG_COLOR 0x000000C0
# define CONFIG_MODAL_PANEL_COLOR 0x1E1E1ED0
# define CONFIG_MODAL_BORDER_COLOR 0xCCCCCCFF
# define CONFIG_MODAL_BORDER_THICKNESS 2

# define TAB_HEIGHT 40
# define TAB_INACTIVE_COLOR 0x333333FF
# define TAB_ACTIVE_COLOR 0x1E1E1ED0
# define TAB_BORDER_COLOR 0x999999FF
# define TAB_SELECTED_BORDER_COLOR 0xFFFFFFFF
# define TAB_SELECTED_BORDER_THICKNESS 3

# define CONFIG_MODAL_QUIT_TEXT "QUIT (Q)"
# define CONFIG_MODAL_QUIT_BTN_COLOR 0x555555FF
# define CONFIG_MODAL_QUIT_BTN_HOVER_COLOR 0x444444FF
# define CONFIG_MODAL_QUIT_BTN_MARGIN 24
# define CONFIG_MODAL_QUIT_BTN_W 160
# define CONFIG_MODAL_QUIT_BTN_H 52
# define CONFIG_MODAL_QUIT_HOLD_SECONDS 1.2
# define CONFIG_MODAL_QUIT_PROGRESS_COLOR 0xB33000FF
# define CONFIG_MODAL_QUIT_PROGRESS_THICKNESS 4

# define ICON_COLOR 0xFFFFFFFF
# define CONFIG_MODAL_OPTION_TOP 80
# define CONFIG_MODAL_OPTION_LEFT 32
# define CONFIG_MODAL_OPTION_RIGHT 32
# define CONFIG_MODAL_OPTION_LINE 40
# define CONFIG_MODAL_OPTION_BOX 22

# define CONFIG_MODAL_TOGGLE_COUNT 5
# define CONFIG_MODAL_SLIDER_COUNT 4
# define CONFIG_MODAL_OPTION_COUNT (CONFIG_MODAL_TOGGLE_COUNT \
		+ CONFIG_MODAL_SLIDER_COUNT)
# define CONFIG_MODAL_CONTROL_COUNT 10

# define CONFIG_OPTION_SHOW_FPS 0
# define CONFIG_OPTION_CROSSHAIR 1
# define CONFIG_OPTION_MINIMAP 2
# define CONFIG_OPTION_FULLSCREEN 3
# define CONFIG_OPTION_DEBUG_MODE 4
# define CONFIG_OPTION_GAME_SPEED 5
# define CONFIG_OPTION_FPS_LIMIT 6
# define CONFIG_OPTION_MOUSE_SENS 7
# define CONFIG_OPTION_PROJECTILE_SPEED 8

# define CONFIG_MODAL_CONTENT_PADDING 28
# define CONFIG_MODAL_COLUMN_GAP 26
# define CONFIG_MODAL_CARD_PADDING 18
# define CONFIG_MODAL_SECTION_TITLE_GAP 26
# define CONFIG_MODAL_ROW_SPACING 40
# define CONFIG_MODAL_ROW_HEIGHT 32
# define CONFIG_MODAL_TOGGLE_W 52
# define CONFIG_MODAL_TOGGLE_H 22
# define CONFIG_MODAL_SLIDER_W 180
# define CONFIG_MODAL_SLIDER_H 8
# define CONFIG_MODAL_SLIDER_KNOB 16
# define CONFIG_MODAL_SLIDER_STEP 5
# define CONFIG_MODAL_SLIDER_VALUE_W 80
# define CONFIG_MODAL_SLIDER_VALUE_GAP 10
# define CONFIG_MODAL_SLIDER_VALUE_LEN 16
# define CONFIG_MODAL_FOOTER_SPACE 90
# define CONFIG_MODAL_KEYCAP_W 64
# define CONFIG_MODAL_KEYCAP_H 24
# define CONFIG_MODAL_KEY_LABEL_LEN 16

# define CONFIG_SLIDER_GAME_SPEED 0
# define CONFIG_SLIDER_FPS_LIMIT 1
# define CONFIG_SLIDER_MOUSE_SENS 2
# define CONFIG_SLIDER_PROJECTILE_SPEED 3

# define CONFIG_MODAL_TEXT_COLOR 0xF2F0E6FF
# define CONFIG_MODAL_MUTED_TEXT_COLOR 0xA0A0A0FF
# define CONFIG_MODAL_ACCENT_COLOR 0xF2B134FF
# define CONFIG_MODAL_ACCENT_SOFT 0xF2B13455
# define CONFIG_MODAL_PANEL_TOP_COLOR 0x2E2E2ED8
# define CONFIG_MODAL_PANEL_BOTTOM_COLOR 0x1C1C1CD8
# define CONFIG_MODAL_CARD_TOP_COLOR 0x2B2B2BD8
# define CONFIG_MODAL_CARD_BOTTOM_COLOR 0x1E1E1ED8
# define CONFIG_MODAL_CARD_BORDER_LIGHT 0x4A4A4AFF
# define CONFIG_MODAL_CARD_BORDER_DARK 0x141414FF
# define CONFIG_MODAL_TOGGLE_OFF_COLOR 0x3A3A3AFF
# define CONFIG_MODAL_TOGGLE_ON_COLOR 0xF2B134FF
# define CONFIG_MODAL_SLIDER_TRACK_COLOR 0x3A3A3AFF
# define CONFIG_MODAL_SLIDER_FILL_COLOR 0xF2B134FF
# define CONFIG_MODAL_SLIDER_KNOB_COLOR 0xF5F5F5FF
# define CONFIG_MODAL_KEYCAP_COLOR 0x2F2F2FFF
# define CONFIG_MODAL_KEYCAP_BORDER_COLOR 0x121212FF

typedef struct s_rect
{
	int	x;
	int	y;
	int	w;
	int	h;
}	t_rect;

typedef struct s_border
{
	t_rect		area;
	int			thickness;
	uint32_t	color;
}	t_border;

typedef struct s_icon
{
	int			cx;
	int			cy;
	int			size;
	uint32_t	color;
}	t_icon;

typedef struct s_config_options
{
	bool	show_fps;
	bool	crosshair;
	bool	minimap;
	bool	fullscreen;
	bool	debug_mode;
	int		game_speed;
	int		fps_limit_index;
	int		mouse_sens;
	int		projectile_speed;
	int		selected;
}	t_config_options;

typedef struct s_menu_labels
{
	mlx_image_t	*settings_labels[CONFIG_MODAL_OPTION_COUNT];
	mlx_image_t	*controls_labels[CONFIG_MODAL_CONTROL_COUNT];
	mlx_image_t	*controls_key_labels[CONFIG_MODAL_CONTROL_COUNT];
	mlx_image_t	*settings_sections[2];
	mlx_image_t	*controls_header;
	mlx_image_t	*controls_prompt;
	mlx_image_t	*slider_value_labels[CONFIG_MODAL_SLIDER_COUNT];
	char		slider_value_cache[CONFIG_MODAL_SLIDER_COUNT]
		[CONFIG_MODAL_SLIDER_VALUE_LEN];
	char		controls_key_cache[CONFIG_MODAL_CONTROL_COUNT]
		[CONFIG_MODAL_KEY_LABEL_LEN];
}	t_menu_labels;

typedef struct s_menu_state
{
	bool		open;
	bool		attached;
	int			current_tab;
	mlx_image_t	*modal;
	int32_t		quit_x;
	int32_t		quit_y;
	int32_t		quit_w;
	int32_t		quit_h;
	bool		quit_hover;
	double		quit_hold_time;
	mlx_image_t	*quit_label;
	t_config_options	options;
	int			pending_slider_index;
	int			pending_slider_value;
	int			slider_drag_index;
	int			controls_selected;
	bool		controls_rebinding;
	int			controls_rebind_target;
	char		controls_key_text[CONFIG_MODAL_CONTROL_COUNT]
		[CONFIG_MODAL_KEY_LABEL_LEN];
	t_menu_labels	labels;
}	t_menu_state;

typedef struct s_menu_layout
{
	t_rect	panel;
	t_rect	body;
	t_rect	content;
	t_rect	left;
	t_rect	right;
	int		slider_w;
}	t_menu_layout;

t_rect	rect_make(int x, int y, int w, int h);
void	set_image_enabled(mlx_image_t *img, bool enabled);
void	draw_rect(mlx_image_t *img, t_rect rect, uint32_t color);
void	draw_border(mlx_image_t *img, t_border border);
void	draw_bevel_border(mlx_image_t *img, t_border border, uint32_t light,
			uint32_t dark);
void	draw_progress_border(mlx_image_t *img, t_border border,
			float progress);
void	draw_vertical_gradient(mlx_image_t *img, t_rect rect,
			uint32_t top_color, uint32_t bottom_color);
void	draw_settings_icon(mlx_image_t *img, t_icon icon);
void	draw_controls_icon(mlx_image_t *img, t_icon icon);
void	disable_label_group(mlx_image_t **labels, size_t count);
bool	update_label_text(t_game *game, mlx_image_t **label, char *cache,
			size_t cache_size, const char *text);
bool	ensure_settings_labels(t_game *game);
bool	ensure_controls_labels(t_game *game);
void	draw_card(t_game *game, t_rect card);
void	draw_row_highlight(t_game *game, t_rect card, int row_y);
void	draw_toggle_switch(t_game *game, t_rect rect, bool enabled,
			bool selected);
void	draw_slider(t_game *game, t_rect track, int value, bool selected);
int		menu_layout_row_y(t_rect card, int index);
t_rect	menu_layout_slider_rect(t_menu_layout layout, int row_y);
t_menu_layout	menu_layout_build(t_menu_state *menu);
void	render_quit_button(t_game *game);
void	draw_modal_layout(t_game *game);
void	draw_settings_options(t_game *game, t_rect panel);
void	draw_controls_options(t_game *game, t_rect panel);
void	hide_settings_options(t_game *game);
void	hide_controls_options(t_game *game);
void	config_options_init(t_game *game);
void	config_options_reset(t_game *game);
void	config_option_toggle(t_game *game, int index);
void	config_option_adjust(t_game *game, int delta);
void	config_option_select(t_game *game, int delta);
int		config_option_from_pos(t_game *game, int32_t mx, int32_t my);
bool	config_option_toggle_state(t_game *game, int index);
int		config_option_slider_value(t_game *game, int index);
int		config_option_slider_raw(t_game *game, int slider);
void	config_option_set_slider_raw(t_game *game, int slider, int value);
int		config_fps_limit_value(int index);
void	config_option_set_slider_percent(t_game *game, int slider,
			int percent);
void	config_option_step_slider(t_game *game, int slider, int delta);
void	config_option_slider_text(t_game *game, int index, char *buffer,
			size_t buffer_size);
bool	config_option_drag_update(t_game *game, int32_t mx, int32_t my,
			bool mouse_down);
void	config_controls_select(t_game *game, int delta);
void	config_controls_begin_rebind(t_game *game);
void	config_controls_cancel_rebind(t_game *game);
bool	config_controls_handle_key(t_game *game, mlx_key_data_t keydata);
int		config_controls_selected(t_game *game);
bool	config_controls_is_rebinding(t_game *game);
const char	*config_controls_key_text(t_game *game, int index);

#endif
