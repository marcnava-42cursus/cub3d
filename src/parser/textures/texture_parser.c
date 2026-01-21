/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_parser.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 00:50:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/12/23 19:45:19 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "structs.h"

int	is_texture_identifier(const char *line)
{
	char	*trimmed;
	int		result;

	trimmed = trim_whitespace((char *)line);
	if (!trimmed)
		return (0);
	if (is_link_identifier(trimmed))
		return (0);
	if (ft_strlen(trimmed) < 3)
		return (0);
	result = ((ft_isalpha(trimmed[0]) || ft_isdigit(trimmed[0]))
			&& ft_isalpha(trimmed[1])
			&& trimmed[2] == ' ');
	return (result);
}

static int	validate_texture_id(const char *identifier)
{
	if (!ft_isalpha(identifier[1])
		|| (!ft_isalpha(identifier[0]) && !ft_isdigit(identifier[0])))
	{
		printf("Error: Invalid texture identifier: %s\n", identifier);
		return (0);
	}
	return (1);
}

static char	*get_validated_path(const char *line, const char *identifier)
{
	char	*path;

	path = extract_texture_path(line, identifier);
	if (!path)
	{
		printf("Error: No texture path found for %s\n", identifier);
		return (NULL);
	}
	if (!validate_texture_path(path))
	{
		printf("Error: Invalid texture path: %s\n", path);
		free(path);
		return (NULL);
	}
	return (path);
}

static int	add_custom_texture(t_textures *textures, const char *id, char *path)
{
	t_custom_texture	*new;
	t_custom_texture	*current;

	current = textures->custom;
	while (current)
	{
		if (ft_strcmp(current->id, id) == 0)
		{
			printf("Error: Duplicate texture identifier: %s\n", id);
			free(path);
			return (0);
		}
		if (!current->next)
			break ;
		current = current->next;
	}
	new = malloc(sizeof(t_custom_texture));
	if (!new)
	{
		free(path);
		return (0);
	}
	ft_strlcpy(new->id, id, 3);
	new->path = path;
	new->texture = NULL;
	new->next = NULL;
	if (!textures->custom)
		textures->custom = new;
	else
		current->next = new;
	return (1);
}

int	parse_texture_line(const char *line, t_textures *textures)
{
	char	identifier[3];
	char	*path;
	char	*trimmed;

	trimmed = trim_whitespace((char *)line);
	if (!trimmed || ft_strlen(trimmed) < 3)
		return (0);
	identifier[0] = trimmed[0];
	identifier[1] = trimmed[1];
	identifier[2] = '\0';
	if (!validate_texture_id(identifier))
		return (0);
	path = get_validated_path(line, identifier);
	if (!path)
		return (0);
	if (ft_strcmp(identifier, "NO") == 0
		|| ft_strcmp(identifier, "SO") == 0
		|| ft_strcmp(identifier, "WE") == 0
		|| ft_strcmp(identifier, "EA") == 0)
		return (set_texture(textures, identifier, path));
	return (add_custom_texture(textures, identifier, path));
}
