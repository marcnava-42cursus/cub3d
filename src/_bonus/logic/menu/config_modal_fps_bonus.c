/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_modal_fps_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 00:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/01/31 16:20:20 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config_bonus.h"
#include "structs.h"
#include "logic_bonus.h"
#include <stdlib.h>

static t_fps_overlay	g_fps;

static void	hide_fps_label(void)
{
	if (!g_fps.label)
		return ;
	set_image_enabled(g_fps.label, false);
}

static void	build_fps_text(char *buffer, size_t size, int fps)
{
	char	*number;

	if (!buffer || size == 0)
		return ;
	number = ft_itoa(fps);
	if (!number)
		return ;
	buffer[0] = '\0';
	ft_strlcpy(buffer, "FPS: ", size);
	ft_strlcat(buffer, number, size);
	free(number);
}

static void	update_fps_label(t_game *game, double fps)
{
	char	text[32];

	if (!game || !game->mlx)
		return ;
	build_fps_text(text, sizeof(text), (int)(fps + 0.5));
	if (g_fps.label)
		mlx_delete_image(game->mlx, g_fps.label);
	g_fps.label = mlx_put_string(game->mlx, text, 12, 12);
	if (!g_fps.label)
		return ;
		set_image_enabled(g_fps.label, true);
}

static bool	fps_overlay_begin_sample(t_game *game, double *dt)
{
	double	now;

	if (!game || !game->mlx)
		return (false);
	if (!game->menu.options.show_fps)
	{
		hide_fps_label();
		return (false);
	}
	now = mlx_get_time();
	if (g_fps.last_time <= 0.0)
		g_fps.last_time = now;
	*dt = now - g_fps.last_time;
	g_fps.last_time = now;
	return (*dt > 0.0);
}

static bool	fps_overlay_needs_refresh(double dt)
{
	g_fps.accum += dt;
	g_fps.frames += 1;
	if (g_fps.accum >= 0.25)
		return (true);
	if (g_fps.label)
		set_image_enabled(g_fps.label, true);
	return (false);
}

static double	fps_overlay_clamped_value(t_game *game)
{
	double	fps;
	int		limit;

	fps = (double)g_fps.frames / g_fps.accum;
	limit = config_fps_limit_value(game->menu.options.fps_limit_index);
	if (limit > 0 && fps > (double)limit)
		return ((double)limit);
	return (fps);
}

void	fps_overlay_update(t_game *game)
{
	double	dt;
	double	fps;

	if (!fps_overlay_begin_sample(game, &dt))
		return ;
	if (!fps_overlay_needs_refresh(dt))
		return ;
	fps = fps_overlay_clamped_value(game);
	g_fps.accum = 0.0;
	g_fps.frames = 0;
	update_fps_label(game, fps);
}
