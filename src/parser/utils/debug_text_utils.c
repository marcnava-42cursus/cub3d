/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_text_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 20:41:57 by marcnava          #+#    #+#             */
/*   Updated: 2025/11/04 20:42:20 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "structs.h"

static void	print_path(char *orientation, char *path)
{
	if (path)
		printf("\t%s Path: %s\n", orientation, path);
	else
		printf("\t%s Path: NULL\n", orientation);
}

static void	print_load(char *orientation, xpm_t *texture)
{
	if (texture)
		printf("\t%s: Loaded successfully\n", orientation);
	else
		printf("\t%s: Not loaded\n", orientation);
}

void	print_textures(const t_cub_data *data)
{
	printf("Textures:\n");
	printf("  Texture Paths:\n");
	print_path("North", data->textures.north_path);
	print_path("South", data->textures.south_path);
	print_path("East", data->textures.east_path);
	print_path("West", data->textures.west_path);
	printf("  Loaded Textures:\n");
	print_load("North", data->textures.north);
	print_load("South", data->textures.south);
	print_load("East", data->textures.east);
	print_load("West", data->textures.west);
}
