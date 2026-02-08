/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 00:40:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/08 13:21:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <string.h>
# include <math.h>
# include <stdbool.h>
# include "structs.h"
# include "libft.h"

typedef struct s_link_info
{
	char		**target;
	const char	*label;
	size_t		id_len;
}	t_link_info;

typedef struct s_cub_data	t_cub_data;
typedef struct s_textures	t_textures;
typedef struct s_player		t_player;
typedef struct s_floor		t_floor;

// Core
int		parse_cub_file(const char *filename, t_cub_data *data);
int		find_map_start(char **lines, int line_count);
void	init_cub_data_parser_only(t_cub_data *data);
int		parse_link_line_bonus(const char *line, t_cub_data *data);
void	apply_default_colors_bonus(t_cub_data *data);
int		parse_elements_bonus(char **lines, int map_start, t_cub_data *data);
int		validate_parsed_data_bonus(const t_cub_data *data);
int		parse_map_with_errors_bonus(char **lines, int map_start,
			t_cub_data *data, const char *error_prefix);

// Colors
int		parse_color_line(const char *line, t_color *floor, t_color *ceiling);
int		is_color_identifier(const char *line);
int		validate_rgb_values(int r, int g, int b);
char	*extract_color_values(const char *line);
int		parse_rgb_values(const char *rgb_str, int *rgb);
int		assign_color(char identifier, t_color *floor, t_color *ceiling,
			int *rgb);

// Map
int		parse_map_section(char **lines, int start_line, t_cub_data *data);
int		validate_map(t_map *map, t_player *player);
int		validate_map_characters(t_map *map);
int		is_map_line(const char *line);
int		find_player_position(t_map *map, t_player *player);
int		is_map_closed(t_map *map);
int		process_file_data(char **lines, int line_count, t_cub_data *data);
int		is_link_identifier(const char *line);
int		parse_link_line(const char *line, t_cub_data *data);
int		is_valid_map_char_bonus(char c);

// Utils
void	free_cub_data(t_cub_data *data);
void	free_map(t_map *map);
void	free_lines(char **lines, int line_count);
char	*trim_whitespace(char *str);
int		is_empty_line(const char *line);
char	**read_file_lines(const char *filename, int *line_count);
int		count_file_lines(const char *filename);

// Validation
int		validate_file_extension(const char *filename);
int		validate_texture_path(const char *path);
int		validate_texture_id(const char *identifier);

// Textures
bool	load_textures(t_textures *textures);
void	free_textures(t_textures *textures);
int		extract_identifier_bonus(const char *line, char identifier[3]);
int		validate_identifier_bonus(const char *identifier);
char	*extract_texture_path_bonus(const char *line,
			const char *identifier);
int		parse_texture_line(const char *line, t_textures *textures);
int		is_texture_identifier(const char *line);
char	*extract_texture_path(const char *line, const char *identifier);
int		set_texture(t_textures *textures, const char *identifier, char *path);

// Floor
char	*normalize_path_bonus(const char *path);
void	free_path_tokens(char **tokens, int count);
int		collect_normalized_tokens_bonus(char **tokens, bool abs_path);
int		collect_floor_elevators_bonus(t_floor *floor);
int		register_floor_elevators_bonus(t_cub_data *data, t_floor *floor);
t_floor	*find_floor_by_path_bonus(t_floor *head, const char *path);
void	free_floor_node_bonus(t_floor *floor);
void	init_floor_node_fields_bonus(t_floor *floor,
			char *canonical_path, t_cub_data *src, int index);
void	reset_source_floor_data_bonus(t_cub_data *src);
int		create_floor_from_data_bonus(char *canonical_path,
			t_cub_data *src, t_floor **out, int index);
int		prepare_tmp_floor_parse_bonus(const char *path, char *canonical,
			t_cub_data *tmp);
void	assign_global_player_floor_bonus(t_cub_data *global,
			t_floor *floor, int index, const char *path);
int		handle_floor_player_bonus(t_cub_data *global,
			t_floor **out_floor, int index);
int		parse_floor_file_bonus(const char *path, int index,
			t_cub_data *global, t_floor **out_floor);
void	connect_neighbor_floors_bonus(t_floor *current,
			t_floor *neighbor, int is_up);
int		attach_existing_neighbor_bonus(t_floor *current,
			t_floor *neighbor, int is_up, int index);
void	push_floor_node_bonus(t_cub_data *data, t_floor *neighbor);
int		parse_neighbor_floor_bonus(t_floor *current,
			const char *neighbor_path, int is_up, t_cub_data *data);
int		parse_floor_neighbors_bonus(t_floor *floor, t_cub_data *data);
int		validate_global_elevators_bonus(t_cub_data *data);
int		setup_root_floor_bonus(const char *path, t_cub_data *data,
			t_floor **root);
int		validate_player_presence_bonus(t_cub_data *data);
void	sync_current_floor_visuals_bonus(t_cub_data *data);

// Debug
void	print_textures(const t_cub_data *data);
void	print_cub_data(const t_cub_data *data);

// Bonus functions
# ifdef BONUS

int		validate_map_bonus(t_map *map, t_player *player);
int		validate_map_characters_bonus(t_map *map);
int		is_map_closed_bonus(t_map *map);
int		find_player_position_bonus(t_map *map, t_player *player);
int		is_map_line_bonus(const char *line);
int		is_texture_identifier_bonus(const char *line);
int		parse_texture_line_bonus(const char *line, t_textures *textures);
int		find_map_start_bonus(char **lines, int line_count);
int		is_link_identifier_bonus(const char *line);
int		build_floor_graph_bonus(const char *path, t_cub_data *data);
int		process_file_data_bonus(char **lines, int line_count,
			t_cub_data *data);

#  define VALIDATE_MAP validate_map_bonus
#  define VALIDATE_MAP_CHARACTERS validate_map_characters_bonus
#  define IS_MAP_CLOSED is_map_closed_bonus
#  define FIND_PLAYER_POSITION find_player_position_bonus
#  define IS_MAP_LINE is_map_line_bonus
#  define IS_TEXTURE_IDENTIFIER is_texture_identifier_bonus
#  define PARSE_TEXTURE_LINE parse_texture_line_bonus
#  define FIND_MAP_START find_map_start_bonus

# else

#  define VALIDATE_MAP validate_map
#  define VALIDATE_MAP_CHARACTERS validate_map_characters
#  define IS_MAP_CLOSED is_map_closed
#  define FIND_PLAYER_POSITION find_player_position
#  define IS_MAP_LINE is_map_line
#  define IS_TEXTURE_IDENTIFIER is_texture_identifier
#  define PARSE_TEXTURE_LINE parse_texture_line
#  define FIND_MAP_START find_map_start

# endif

#endif
