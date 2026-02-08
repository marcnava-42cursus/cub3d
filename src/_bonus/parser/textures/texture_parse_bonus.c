/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_parse_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 03:45:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/08 15:23:46 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static int	add_custom_texture(t_textures *textures, const char *id, char *path)
{
	t_custom_texture	*new;
	t_custom_texture	*current;

	new = malloc(sizeof(t_custom_texture));
	if (!new)
		return (0);
	ft_strlcpy(new->id, id, 3);
	new->path = path;
	new->texture = NULL;
	new->next = NULL;
	if (!textures->custom)
		textures->custom = new;
	else
	{
		current = textures->custom;
		while (current->next)
			current = current->next;
		current->next = new;
	}
	return (1);
}

static int	assign_texture_path_bonus(t_textures *textures,
		char *identifier, char *path)
{
	if (ft_strcmp(identifier, "NO") == 0 && !textures->north_path)
		textures->north_path = path;
	else if (ft_strcmp(identifier, "SO") == 0 && !textures->south_path)
		textures->south_path = path;
	else if (ft_strcmp(identifier, "WE") == 0 && !textures->west_path)
		textures->west_path = path;
	else if (ft_strcmp(identifier, "EA") == 0 && !textures->east_path)
		textures->east_path = path;
	else if (!add_custom_texture(textures, identifier, path))
		return (free(path), 0);
	return (1);
}

int	parse_texture_line_bonus(const char *line, t_textures *textures)
{
	char	identifier[3];
	char	*path;

	if (!extract_identifier_bonus(line, identifier))
		return (0);
	if (!validate_identifier_bonus(identifier))
		return (0);
	path = extract_texture_path_bonus(line, identifier);
	if (!path)
	{
		printf("Error\nNo texture path found for %s\n", identifier);
		return (0);
	}
	if (!validate_texture_path(path))
	{
		printf("Error\nInvalid texture path: %s\n", path);
		free(path);
		return (0);
	}
	return (assign_texture_path_bonus(textures, identifier, path));
}
