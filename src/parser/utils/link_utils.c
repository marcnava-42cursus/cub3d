/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   link_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 00:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/08 15:23:46 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "structs.h"

int	is_link_identifier(const char *line)
{
	char	*trimmed;

	trimmed = trim_whitespace((char *)line);
	if (!trimmed || *trimmed == '\0')
		return (0);
	if (ft_strncmp(trimmed, "UP", 2) == 0
		&& (trimmed[2] == '\0' || trimmed[2] == ' '))
		return (1);
	if (ft_strncmp(trimmed, "DOWN", 4) == 0
		&& (trimmed[4] == '\0' || trimmed[4] == ' '))
		return (1);
	return (0);
}

int	parse_link_line(const char *line, t_cub_data *data)
{
	char	*trimmed;

	(void)data;
	trimmed = trim_whitespace((char *)line);
	if (!trimmed)
		return (0);
	printf("Error\nLink identifiers (UP/DOWN) are not allowed in mandatory\n");
	return (0);
}
