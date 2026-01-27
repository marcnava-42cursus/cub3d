/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fps_overlay.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 16:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/01/27 16:00:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "logic.h"
#include "structs.h"
#include <stdlib.h>

typedef struct s_fps_overlay
{
	mlx_image_t	*label;
	double		accum;
	int			frames;
	double		last_time;
} 	t_fps_overlay;

static t_fps_overlay	g_fps;

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
}

void	fps_overlay_update(t_game *game)
{
	double	now;
	double	dt;
	double	fps;

	if (!game || !game->mlx)
		return ;
	now = mlx_get_time();
	if (g_fps.last_time <= 0.0)
		g_fps.last_time = now;
	dt = now - g_fps.last_time;
	g_fps.last_time = now;
	if (dt <= 0.0)
		return ;
	g_fps.accum += dt;
	g_fps.frames += 1;
	if (g_fps.accum < 0.25)
		return ;
	fps = (double)g_fps.frames / g_fps.accum;
	g_fps.accum = 0.0;
	g_fps.frames = 0;
	update_fps_label(game, fps);
}
