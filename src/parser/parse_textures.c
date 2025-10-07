/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_textures.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 00:50:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/02 01:47:14 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parser.h"

int	is_texture_identifier(const char *line)
{
	char	*trimmed;
	int		result;

	trimmed = trim_whitespace((char *)line);
	if (!trimmed)
		return (0);

	result = (strncmp(trimmed, "NO ", 3) == 0
		|| strncmp(trimmed, "SO ", 3) == 0
		|| strncmp(trimmed, "WE ", 3) == 0
		|| strncmp(trimmed, "EA ", 3) == 0);

	return (result);
}

static char	*extract_texture_path(const char *line, const char *identifier)
{
	char	*trimmed;
	char	*path;
	char	*path_start;
	char	*path_end;
	int		len;
	int		i;

	trimmed = trim_whitespace((char *)line);
	if (!trimmed)
		return (NULL);

	// Saltar el identificador (NO, SO, WE, EA)
	path_start = trimmed + strlen(identifier);

	// Saltar espacios después del identificador
	while (*path_start == ' ' || *path_start == '\t')
		path_start++;

	if (*path_start == '\0')
		return (NULL);

	// Encontrar el final del path (eliminar espacios finales)
	path_end = path_start + strlen(path_start) - 1;
	while (path_end > path_start && (*path_end == ' '
		|| *path_end == '\t' || *path_end == '\n'))
		path_end--;

	len = path_end - path_start + 1;
	path = malloc(len + 1);
	if (!path)
		return (NULL);

	i = 0;
	while (i < len)
	{
		path[i] = path_start[i];
		i++;
	}
	path[len] = '\0';

	return (path);
}

static int	set_texture(t_textures *textures, const char *identifier, char *path)
{
	if (strcmp(identifier, "NO") == 0)
	{
		if (textures->north_path)
		{
			printf("Error: North texture already defined\n");
			free(path);
			return (0);
		}
		textures->north_path = path;
	}
	else if (strcmp(identifier, "SO") == 0)
	{
		if (textures->south_path)
		{
			printf("Error: South texture already defined\n");
			free(path);
			return (0);
		}
		textures->south_path = path;
	}
	else if (strcmp(identifier, "WE") == 0)
	{
		if (textures->west_path)
		{
			printf("Error: West texture already defined\n");
			free(path);
			return (0);
		}
		textures->west_path = path;
	}
	else if (strcmp(identifier, "EA") == 0)
	{
		if (textures->east_path)
		{
			printf("Error: East texture already defined\n");
			free(path);
			return (0);
		}
		textures->east_path = path;
	}
	else
	{
		free(path);
		return (0);
	}

	return (1);
}

int	parse_texture_line(const char *line, t_textures *textures)
{
	char	identifier[3];
	char	*path;
	char	*trimmed;

	trimmed = trim_whitespace((char *)line);
	if (!trimmed)
		return (0);

	// Extraer identificador
	if (strlen(trimmed) < 3)
		return (0);

	identifier[0] = trimmed[0];
	identifier[1] = trimmed[1];
	identifier[2] = '\0';

	// Validar identificador
	if (strcmp(identifier, "NO") != 0 && strcmp(identifier, "SO") != 0
		&& strcmp(identifier, "WE") != 0 && strcmp(identifier, "EA") != 0)
	{
		printf("Error: Invalid texture identifier: %s\n", identifier);
		return (0);
	}

	// Extraer path
	path = extract_texture_path(line, identifier);
	if (!path)
	{
		printf("Error: No texture path found for %s\n", identifier);
		return (0);
	}

	// Validar path
	if (!validate_texture_path(path))
	{
		printf("Error: Invalid texture path: %s\n", path);
		free(path);
		return (0);
	}

	// Asignar textura
	return (set_texture(textures, identifier, path));
}
