/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   value_extraction.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 01:55:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/11/03 19:07:15 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "structs.h"

static char	*find_values_start(char *trimmed)
{
	char	*values_start;

	values_start = trimmed + 1;
	while (*values_start == ' ' || *values_start == '\t')
		values_start++;
	return (values_start);
}

static char	*find_values_end(char *values_start)
{
	char	*values_end;

	values_end = values_start + ft_strlen(values_start) - 1;
	while (values_end > values_start && (*values_end == ' '
			|| *values_end == '\t' || *values_end == '\n'))
		values_end--;
	return (values_end);
}

static char	*copy_values(char *values_start, char *values_end)
{
	char	*result;
	int		len;
	int		i;

	len = values_end - values_start + 1;
	result = malloc(len + 1);
	if (!result)
		return (NULL);
	i = 0;
	while (i < len)
	{
		result[i] = values_start[i];
		i++;
	}
	result[len] = '\0';
	return (result);
}

char	*extract_color_values(const char *line)
{
	char	*trimmed;
	char	*values_start;
	char	*values_end;

	trimmed = trim_whitespace((char *)line);
	if (!trimmed)
		return (NULL);
	values_start = find_values_start(trimmed);
	if (*values_start == '\0')
		return (NULL);
	values_end = find_values_end(values_start);
	return (copy_values(values_start, values_end));
}
