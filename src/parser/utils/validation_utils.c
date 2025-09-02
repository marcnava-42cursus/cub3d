/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 02:10:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/02 02:10:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/parser.h"

int	validate_file_extension(const char *filename)
{
	int	len;

	if (!filename)
		return (0);
	len = strlen(filename);
	if (len < 4)
		return (0);
	return (strcmp(filename + len - 4, ".cub") == 0);
}

int	validate_texture_path(const char *path)
{
	FILE	*file;

	if (!path || strlen(path) == 0)
		return (0);
	file = fopen(path, "r");
	if (!file)
		return (0);
	fclose(file);
	return (1);
}

