/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_modal_bonus.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/06 15:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/01/06 15:00:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_MODAL_BONUS_H
# define CONFIG_MODAL_BONUS_H

# include "logic.h"

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

t_rect	rect_make(int x, int y, int w, int h);
void	set_image_enabled(mlx_image_t *img, bool enabled);
void	draw_rect(mlx_image_t *img, t_rect rect, uint32_t color);
void	draw_border(mlx_image_t *img, t_border border);
void	draw_progress_border(mlx_image_t *img, t_border border,
			float progress);
void	draw_settings_icon(mlx_image_t *img, t_icon icon);
void	draw_controls_icon(mlx_image_t *img, t_icon icon);
void	render_quit_button(t_game *game);
void	draw_modal_layout(t_game *game);
void	draw_settings_options(t_game *game, t_rect panel);
void	hide_settings_options(t_game *game);
void	config_options_reset(t_game *game);
void	config_option_toggle(t_game *game, int index);
void	config_option_select(t_game *game, int delta);
int		config_option_from_pos(t_game *game, int32_t mx, int32_t my);

#endif
