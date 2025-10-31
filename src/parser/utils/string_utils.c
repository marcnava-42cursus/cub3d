/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 02:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/10/31 04:45:41 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/parser.h"

static char	*find_start(char *str)
{
	char	*start;

	if (!str)
		return (NULL);
	start = str;
	while (*start == ' ' || *start == '\t' || *start == '\r')
		start++;
	if (*start == '\0')
		return (start);
	return (start);
}

char	*trim_whitespace(char *str)
{
	char	*start;
	char	*end;

	start = find_start(str);
	if (!start || *start == '\0')
		return (start);
	end = start + strlen(start) - 1;
	while (end > start && (*end == ' ' || *end == '\t' || *end == '\n' || *end == '\r'))
		end--;
	*(end + 1) = '\0';
	return (start);
}

int	is_empty_line(const char *line)
{
	int	i;

	if (!line)
		return (1);
	i = 0;
	while (line[i])
	{
		if (line[i] != ' ' && line[i] != '\t' && line[i] != '\n' && line[i] != '\r')
			return (0);
		i++;
	}
	return (1);
}
