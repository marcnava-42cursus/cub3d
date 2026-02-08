/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   floor_path_normalize_bonus.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 03:45:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/08 03:30:53 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static char	*alloc_normalized_path_bonus(size_t size, bool abs_path)
{
	char	*normalized;

	normalized = malloc(size);
	if (!normalized)
		return (NULL);
	normalized[0] = '\0';
	if (abs_path)
		ft_strlcpy(normalized, "/", size);
	return (normalized);
}

static void	ensure_non_empty_path_bonus(char *normalized, bool abs_path,
		size_t size)
{
	if (normalized[0] != '\0')
		return ;
	if (abs_path)
		ft_strlcpy(normalized, "/", size);
	else
		ft_strlcpy(normalized, ".", size);
}

static void	append_tokens_to_path_bonus(char *normalized, size_t size,
		char **tokens, int top)
{
	int	i;

	i = 0;
	while (i < top)
	{
		if (normalized[0] != '\0'
			&& normalized[ft_strlen(normalized) - 1] != '/')
			ft_strlcat(normalized, "/", size);
		ft_strlcat(normalized, tokens[i], size);
		i++;
	}
}

char	*normalize_path_bonus(const char *path)
{
	char	**tokens;
	char	*normalized;
	size_t	size;
	int		top;
	bool	abs_path;

	if (!path)
		return (NULL);
	tokens = ft_split(path, '/');
	if (!tokens)
		return (NULL);
	abs_path = (path[0] == '/');
	top = collect_normalized_tokens_bonus(tokens, abs_path);
	size = ft_strlen(path) + 3;
	normalized = alloc_normalized_path_bonus(size, abs_path);
	if (!normalized)
		return (free_path_tokens(tokens, top), NULL);
	append_tokens_to_path_bonus(normalized, size, tokens, top);
	ensure_non_empty_path_bonus(normalized, abs_path, size);
	return (free_path_tokens(tokens, top), normalized);
}
