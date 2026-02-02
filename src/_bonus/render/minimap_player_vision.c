/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_player_vision.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/27 21:31:32 by ivmirand          #+#    #+#             */
/*   Updated: 2026/01/30 22:16:57 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

static void	cast_rays_for_vision(vertex_t ray_endpoints[16],
		t_minimap *minimap, vertex_t *player_position)
{
	t_rayhit	rayhit;
	float		angle_strt_ray_inc[3];
	int			i;

	angle_strt_ray_inc[0] = minimap->player->angle - 0.5f * PLAYER_FOV;
	angle_strt_ray_inc[2] = PLAYER_FOV / (float)(MINIMAP_RAYS - 1);
	i = 0;
	while (i < MINIMAP_RAYS)
	{
		angle_strt_ray_inc[1] = normalize_angle(
				angle_strt_ray_inc[0] + i * angle_strt_ray_inc[2]);
		rayhit = raycast_world(minimap->map, *player_position,
				angle_strt_ray_inc[1], MAX_TILE_RENDER * MINIMAP_TILE_SIZE);
		ray_endpoints[i] = world_to_minimap_vertex(minimap, rayhit.position);
		i++;
	}
}

static void	fill_vision_triangle(mlx_image_t *player_vision,
		vertex_t *player_to_minimap,
		vertex_t ray_endpoints[16])
{
	int	i;

	i = 0;
	while (i < MINIMAP_RAYS - 1)
	{
		fill_triangle_scanline(player_vision, *player_to_minimap,
			ray_endpoints[i], ray_endpoints[i + 1]);
		i++;
	}
	i = 0;
	while (i < MINIMAP_RAYS)
	{
		bresenham_clipped(player_to_minimap, &ray_endpoints[i],
			player_vision, MEDIUM_GREY);
		i++;
	}
}

void	render_minimap_player_vision(t_minimap *minimap)
{
	vertex_t	player_position;
	vertex_t	player_to_minimap;
	vertex_t	ray_endpoints[16];

	ft_bzero(minimap->player_vision->pixels, minimap->player_vision->width
		* minimap->player_vision->height * 4);
	player_position.x = (minimap->player->x) * WORLDMAP_TILE_SIZE;
	player_position.y = (minimap->player->y) * WORLDMAP_TILE_SIZE;
	player_to_minimap = world_to_minimap_vertex(minimap, player_position);
	cast_rays_for_vision(ray_endpoints, minimap, &player_position);
	fill_vision_triangle(minimap->player_vision, &player_to_minimap,
		ray_endpoints);
}
