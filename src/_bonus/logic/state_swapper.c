/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state_swapper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 20:15:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/08 00:05:06 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "structs.h"
#include "logic_bonus.h"

/**
 * @brief Apply interaction to the targeted cell.
 * Returns true when the cell changes state.
 */
static bool	modify_interactive_cell(t_game *game, int x, int y)
{
	char	*cell;

	if (!game || !game->cub_data.map.grid)
		return (false);
	if (y < 0 || y >= game->cub_data.map.height)
		return (false);
	if (x < 0 || x >= (int)ft_strlen(game->cub_data.map.grid[y]))
		return (false);
	cell = &game->cub_data.map.grid[y][x];
	if (*cell == 'D')
	{
		*cell = 'd';
		return (true);
	}
	if (*cell == 'd')
	{
		*cell = 'D';
		return (true);
	}
	return (false);
}

int	get_elevator_index(char elevator_id)
{
	const char	*set;
	char		*pos;

	set = "!\"·$%&/()=?¿";
	pos = ft_strchr(set, elevator_id);
	if (!pos)
		return (-1);
	return ((int)(pos - set));
}

static bool	is_elevator(char c)
{
	return (get_elevator_index(c) >= 0);
}

static int	get_elevator_slot(t_cub_data *data, char id)
{
	int	i;

	if (!data)
		return (-1);
	i = 0;
	while (i < data->elevator_id_count)
	{
		if (data->elevator_ids[i] == id)
			return (i);
		i++;
	}
	return (-1);
}

static bool	target_inside_map(t_game *game, int x, int y)
{
	size_t	row_len;

	if (!game || !game->cub_data.map.grid)
		return (false);
	if (y < 0 || y >= game->cub_data.map.height)
		return (false);
	row_len = ft_strlen(game->cub_data.map.grid[y]);
	return (x >= 0 && x < (int)row_len);
}

static bool	target_intersects_player(t_game *game, int x, int y)
{
	float	nearest_x;
	float	nearest_y;
	float	dx;
	float	dy;

	nearest_x = fmaxf((float)x, fminf(game->cub_data.player.x,
				(float)x + 1.0f));
	nearest_y = fmaxf((float)y, fminf(game->cub_data.player.y,
				(float)y + 1.0f));
	dx = game->cub_data.player.x - nearest_x;
	dy = game->cub_data.player.y - nearest_y;
	return ((dx * dx + dy * dy) <= game->player_radius * game->player_radius);
}

static bool	is_valid_target_cell(t_game *game, int x, int y)
{
	int		player_x;
	int		player_y;

	if (!target_inside_map(game, x, y))
		return (false);
	player_x = (int)floorf(game->cub_data.player.x);
	player_y = (int)floorf(game->cub_data.player.y);
	if (x == player_x && y == player_y)
		return (false);
	if (target_intersects_player(game, x, y))
		return (false);
	return (game->cub_data.map.grid[y][x] == '0');
}

static bool	get_attachment_coords(t_game *game, t_rayhit *hit, int *x, int *y)
{
	vertex_t	dir;
	vertex_t	point_before_hit;

	if (!game || !hit || !hit->hit)
		return (false);
	dir.x = cosf(game->cub_data.player.angle);
	dir.y = sinf(game->cub_data.player.angle);
	point_before_hit.x = hit->position.x - dir.x;
	point_before_hit.y = hit->position.y - dir.y;
	*x = (int)floorf(point_before_hit.x / WORLDMAP_TILE_SIZE);
	*y = (int)floorf(point_before_hit.y / WORLDMAP_TILE_SIZE);
	return (true);
}

static bool	handle_elevator_orb_place(t_game *game, t_rayhit *hit, char cell)
{
	int		slot;
	char	payload;

	if (!is_elevator(cell))
		return (false);
	slot = get_elevator_slot(&game->cub_data, cell);
	if (slot < 0 || game->cub_data.elevator_orb[slot])
		return (true);
	payload = consume_inventory_block(&game->cub_data.player);
	if (payload == '\0')
		return (true);
	if (!orb_projectile_start_elevator_place(game, hit->cell, payload, slot))
	{
		store_block_in_inventory(&game->cub_data.player, payload);
		return (true);
	}
	if (game->menu.options.debug_mode)
		printf("Injected orb into elevator '%c'\n", cell);
	return (true);
}

static void	finalize_elevator_orb_take(t_game *game, int slot, char cell)
{
	int	elevator_index;

	game->orb.elevator_shot = true;
	game->orb.elevator_slot = slot;
	game->cub_data.elevator_orb[slot] = false;
	game->cub_data.elevator_orb_payload[slot] = '\0';
	elevator_index = get_elevator_index(cell);
	if (elevator_index >= 0 && elevator_index < ELEVATOR_STATE_SLOTS)
		game->cub_data.map.elevator_states[elevator_index] = ELEVATOR_CLOSED;
	game->cub_data.player.state = STATE_TAKE;
	if (game->menu.options.debug_mode)
		printf("Removed orb from elevator '%c'\n", cell);
}

static bool	handle_elevator_orb_take(t_game *game, t_rayhit *hit, char cell)
{
	int		slot;
	char	payload;

	if (!is_elevator(cell))
		return (false);
	slot = get_elevator_slot(&game->cub_data, cell);
	if (slot < 0 || !game->cub_data.elevator_orb[slot])
		return (true);
	if (player_has_block(&game->cub_data.player))
		return (true);
	payload = game->cub_data.elevator_orb_payload[slot];
	if (payload == '\0')
		payload = '2';
	if (!orb_projectile_start_take(game, hit->cell[X], hit->cell[Y], payload))
		return (true);
	finalize_elevator_orb_take(game, slot, cell);
	return (true);
}

static bool	get_place_hit(t_game *game, t_rayhit *hit, char *cell)
{
	vertex_t	start;

	start.x = game->cub_data.player.x * WORLDMAP_TILE_SIZE;
	start.y = game->cub_data.player.y * WORLDMAP_TILE_SIZE;
	*hit = raycast_world(&game->cub_data.map, start,
			game->cub_data.player.angle,
			PLACE_BLOCK_DISTANCE * WORLDMAP_TILE_SIZE);
	if (!hit->hit)
		return (false);
	if (!target_inside_map(game, hit->cell[0], hit->cell[1]))
		return (false);
	*cell = game->cub_data.map.grid[hit->cell[1]][hit->cell[0]];
	return (true);
}

static bool	begin_block_creation(t_game *game, int target_x, int target_y,
	char cell)
{
	if (!orb_projectile_spawn_ghost(game, target_x, target_y))
	{
		store_block_in_inventory(&game->cub_data.player, cell);
		return (false);
	}
	game->cub_data.block.is_creating = true;
	game->cub_data.block.pending_payload = cell;
	game->cub_data.block.pending_x = target_x;
	game->cub_data.block.pending_y = target_y;
	anim_start(&game->cub_data.block.anims[ANIM_CREATE]);
	game->cub_data.player.state = STATE_THROW;
	return (true);
}

static bool	can_place_block_now(t_game *game)
{
	if (!game)
		return (false);
	if (game->menu.options.debug_mode)
		printf("Attempting to place block\n");
	if (orb_projectile_is_active(game))
		return (false);
	if (game->cub_data.block.is_creating)
		return (false);
	return (player_has_block(&game->cub_data.player));
}

static bool	resolve_place_target(t_game *game, t_rayhit *hit, char *cell,
				int target[2])
{
	if (!get_place_hit(game, hit, cell))
		return (false);
	if (handle_elevator_orb_place(game, hit, *cell))
		return (false);
	if (*cell != '1' && *cell != '2')
		return (false);
	if (!get_attachment_coords(game, hit, &target[0], &target[1]))
		return (false);
	return (is_valid_target_cell(game, target[0], target[1]));
}

/**
 * @brief Places a breakable block ('2') on the face of the wall the player
 * is looking at, if the adjacent cell is empty floor.
 */
void	place_breakable_block(t_game *game)
{
	t_rayhit	hit;
	char		cell;
	char		block;
	int			target[2];

	if (!can_place_block_now(game))
		return ;
	if (!resolve_place_target(game, &hit, &cell, target))
		return ;
	block = consume_inventory_block(&game->cub_data.player);
	if (block == '\0')
		return ;
	if (!begin_block_creation(game, target[0], target[1], block))
		return ;
	if (game->menu.options.debug_mode)
		printf("Started block creation at (%d, %d)\n", target[0], target[1]);
}

static bool	get_pending_block_row(t_game *game, t_living_block *block,
	char **row)
{
	if (!game || !block || !row || !game->cub_data.map.grid)
		return (false);
	if (!block->is_creating || !block->anims
		|| !block->anims[ANIM_CREATE].finished)
		return (false);
	if (block->pending_y < 0 || block->pending_y >= game->cub_data.map.height)
		return (false);
	*row = game->cub_data.map.grid[block->pending_y];
	if (!*row || block->pending_x < 0 || block->pending_x
		>= (int)ft_strlen(*row))
		return (false);
	return (true);
}

static void	reset_pending_block(t_living_block *block)
{
	block->is_creating = false;
	block->pending_payload = '\0';
	block->pending_x = -1;
	block->pending_y = -1;
	anim_start(&block->anims[ANIM_CREATE]);
}

void	update_creating_block_state(t_game *game)
{
	t_living_block	*block;
	char			*row;
	char			block_cell;

	block = &game->cub_data.block;
	if (!get_pending_block_row(game, block, &row))
		return ;
	if (block->pending_payload == '\0')
		block_cell = '2';
	else
		block_cell = block->pending_payload;
	if (row[block->pending_x] == ORB_GHOST_BLOCK_CELL)
		row[block->pending_x] = block_cell;
	reset_pending_block(block);
}

static void	handle_breakable_cell(t_game *game, t_rayhit *hit, char cell)
{
	if (cell != '2')
		return ;
	if (orb_projectile_is_active(game))
		return ;
	if (player_has_block(&game->cub_data.player))
		return ;
	game->cub_data.map.grid[hit->cell[1]][hit->cell[0]] = '0';
	if (!orb_projectile_start_take(game, hit->cell[0], hit->cell[1], cell))
	{
		game->cub_data.map.grid[hit->cell[1]][hit->cell[0]] = cell;
		return ;
	}
	if (game->menu.options.debug_mode)
		printf("Broke block at (%d, %d)\n", hit->cell[0], hit->cell[1]);
	game->cub_data.player.state = STATE_TAKE;
}

/**
 * @brief Breaks wall in front of player (only if it's a wall '1')
 *
 * This function casts a ray from the player's position in the direction
 * they're facing. If the ray hits a wall ('1'), it changes it to floor ('0').
 * After breaking the wall, it forces a re-render to show the change
 * immediately.
 *
 * @param game Game state structure
 */
void	test_break_wall_in_front(t_game *game)
{
	t_rayhit	hit;
	vertex_t	start;
	char		cell;

	if (!game)
		return ;
	if (game->menu.options.debug_mode)
		printf("Attempting to break block\n");
	start.x = game->cub_data.player.x * WORLDMAP_TILE_SIZE;
	start.y = game->cub_data.player.y * WORLDMAP_TILE_SIZE;
	hit = raycast_world(&game->cub_data.map, start, game->cub_data.player.angle,
			BREAK_BLOCK_DISTANCE * WORLDMAP_TILE_SIZE);
	if (!hit.hit)
		return ;
	cell = game->cub_data.map.grid[hit.cell[1]][hit.cell[0]];
	if (handle_elevator_orb_take(game, &hit, cell))
		return ;
	if (cell == 'D' || cell == 'd')
		modify_interactive_cell(game, hit.cell[X], hit.cell[Y]);
	else
		handle_breakable_cell(game, &hit, cell);
}
