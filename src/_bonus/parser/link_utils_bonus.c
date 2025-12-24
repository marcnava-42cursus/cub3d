/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   link_utils_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 00:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/12/04 00:00:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	is_link_identifier_bonus(const char *line)
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
