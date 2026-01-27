/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 00:40:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/12/23 15:04:54 by marcnava         ###   ########.fr       */
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

# include "libft.h"

// Estructura para colores RGB
typedef struct s_color
{
	int	r;
	int	g;
	int	b;
}	t_color;

// Estructura para el mapa
typedef struct s_map
{
	char	**grid;
	int		width;
	int		height;
}	t_map;

typedef struct s_link_info
{
	char		**target;
	const char	*label;
	size_t		id_len;
}	t_link_info;

typedef struct s_cub_data	t_cub_data;
typedef struct s_textures	t_textures;
typedef struct s_player	t_player;

// Parser principal
int		parse_cub_file(const char *filename, t_cub_data *data);
int		find_map_start(char **lines, int line_count);
void	init_cub_data_parser_only(t_cub_data *data);

// Parser de texturas
int		parse_texture_line(const char *line, t_textures *textures);
int		is_texture_identifier(const char *line);
char	*extract_texture_path(const char *line, const char *identifier);
int		set_texture(t_textures *textures, const char *identifier, char *path);

// Parser de colores
int		parse_color_line(const char *line, t_color *floor, t_color *ceiling);
int		is_color_identifier(const char *line);
int		validate_rgb_values(int r, int g, int b);
char	*extract_color_values(const char *line);
int		parse_rgb_values(const char *rgb_str, int *rgb);
int		assign_color(char identifier, t_color *floor, t_color *ceiling, int *rgb);

// Parser de mapa
int		parse_map_section(char **lines, int start_line, t_cub_data *data);
int		validate_map(t_map *map, t_player *player);
int		validate_map_characters(t_map *map);
int		is_map_line(const char *line);
int		find_player_position(t_map *map, t_player *player);
int		is_map_closed(t_map *map);
int		process_file_data(char **lines, int line_count, t_cub_data *data);
int		is_link_identifier(const char *line);
int		parse_link_line(const char *line, t_cub_data *data);

// Utilidades
void	free_cub_data(t_cub_data *data);
void	free_map(t_map *map);
void	free_lines(char **lines, int line_count);
char	*trim_whitespace(char *str);
int		is_empty_line(const char *line);
char	**read_file_lines(const char *filename, int *line_count);
int		count_file_lines(const char *filename);

// Validación
int		validate_file_extension(const char *filename);
int		validate_texture_path(const char *path);
int		validate_texture_id(const char *identifier);

// Texture loading
int		load_textures(t_textures *textures);
void	free_textures(t_textures *textures);

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
int		process_file_data_bonus(char **lines, int line_count, t_cub_data *data);

// Remap functions to bonus versions
#  define validate_map validate_map_bonus
#  define validate_map_characters validate_map_characters_bonus
#  define is_map_closed is_map_closed_bonus
#  define find_player_position find_player_position_bonus
#  define is_map_line is_map_line_bonus
#  define is_texture_identifier is_texture_identifier_bonus
#  define parse_texture_line parse_texture_line_bonus
#  define find_map_start find_map_start_bonus

# endif

#endif
