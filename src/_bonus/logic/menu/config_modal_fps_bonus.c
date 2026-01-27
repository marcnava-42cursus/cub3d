/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_modal_fps_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 00:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/01/27 02:37:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config_bonus.h"
#include "structs.h"
#include "logic.h"
#include <stdlib.h>

typedef struct s_fps_overlay
{
	mlx_image_t	*label;
	double		accum;
	int			frames;
}	t_fps_overlay;

static t_fps_overlay	g_fps;

static void	hide_fps_label(void)
{
	if (!g_fps.label)
		return ;
	set_image_enabled(g_fps.label, false);
}

static void	build_fps_text(char *buffer, size_t size, int fps)
{
	char		*number;

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

void	fps_overlay_update(t_game *game)
{
	double	now;
	double	dt;
	double	fps;
	static double	last_time = 0.0;

	if (!game || !game->mlx)
		return ;
	if (!game->menu.options.show_fps)
	{
		hide_fps_label();
		return ;
	}
	now = mlx_get_time();
	if (last_time <= 0.0)
		last_time = now;
	dt = now - last_time;
	last_time = now;
	if (dt <= 0.0)
		return ;
	g_fps.accum += dt;
	g_fps.frames += 1;
	if (g_fps.accum < 0.25)
	{
		if (g_fps.label)
			set_image_enabled(g_fps.label, true);
		return ;
	}
	fps = (double)g_fps.frames / g_fps.accum;
	{
		int	limit;

		limit = config_fps_limit_value(game->menu.options.fps_limit_index);
		if (limit > 0 && fps > (double)limit)
			fps = (double)limit;
	}
	g_fps.accum = 0.0;
	g_fps.frames = 0;
	update_fps_label(game, fps);
}
