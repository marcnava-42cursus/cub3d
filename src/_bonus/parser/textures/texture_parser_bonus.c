/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_parser_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 00:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/11/13 20:04:25 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	is_texture_identifier_bonus(const char *line)
{
	char	*trimmed;
	int		result;

	trimmed = trim_whitespace((char *)line);
	if (!trimmed)
		return (0);
	if (is_link_identifier_bonus(trimmed))
		return (0);
	if (ft_strlen(trimmed) < 3)
		return (0);
	result = (ft_isgraph(trimmed[0])
			&& ft_isalpha(trimmed[1])
			&& trimmed[2] == ' ');
	return (result);
}

static char	*extract_texture_path_bonus(const char *line,
		const char *identifier)
{
	char	*trimmed;
	char	*path_start;
	char	*path_end;
	char	*path;
	int		len;

	trimmed = trim_whitespace((char *)line);
	if (!trimmed)
		return (NULL);
	path_start = trimmed + ft_strlen(identifier);
	while (*path_start == ' ' || *path_start == '\t')
		path_start++;
	if (*path_start == '\0')
		return (NULL);
	path_end = path_start + ft_strlen(path_start) - 1;
	while (path_end > path_start && (*path_end == ' '
			|| *path_end == '\t' || *path_end == '\n'))
		path_end--;
	len = path_end - path_start + 1;
	path = malloc(len + 1);
	if (!path)
		return (NULL);
	ft_strlcpy(path, path_start, len + 1);
	return (path);
}

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

int	parse_texture_line_bonus(const char *line, t_textures *textures)
{
	char	identifier[3];
	char	*path;
	char	*trimmed;

	trimmed = trim_whitespace((char *)line);
	if (!trimmed)
		return (0);
	if (ft_strlen(trimmed) < 3)
		return (0);
	identifier[0] = trimmed[0];
	identifier[1] = trimmed[1];
	identifier[2] = '\0';
	if (!ft_isalpha(identifier[1]) || !ft_isgraph(identifier[0]))
	{
		printf("Error: Invalid texture identifier: %s\n", identifier);
		return (0);
	}
	path = extract_texture_path_bonus(line, identifier);
	if (!path)
	{
		printf("Error: No texture path found for %s\n", identifier);
		return (0);
	}
	if (!validate_texture_path(path))
	{
		printf("Error: Invalid texture path: %s\n", path);
		free(path);
		return (0);
	}
	if (ft_strcmp(identifier, "NO") == 0 && !textures->north_path)
		textures->north_path = path;
	else if (ft_strcmp(identifier, "SO") == 0 && !textures->south_path)
		textures->south_path = path;
	else if (ft_strcmp(identifier, "WE") == 0 && !textures->west_path)
		textures->west_path = path;
	else if (ft_strcmp(identifier, "EA") == 0 && !textures->east_path)
		textures->east_path = path;
	else
	{
		if (!add_custom_texture(textures, identifier, path))
		{
			free(path);
			return (0);
		}
	}
	return (1);
}
