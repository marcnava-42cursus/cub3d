/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   floor_path_tokens_bonus.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 03:45:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/08 03:30:53 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	free_path_tokens(char **tokens, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		free(tokens[i]);
		i++;
	}
	free(tokens);
}

static int	is_empty_or_dot_token_bonus(const char *token)
{
	if (token[0] == '\0')
		return (1);
	return (token[0] == '.' && token[1] == '\0');
}

static int	is_parent_token_bonus(const char *token)
{
	return (token[0] == '.' && token[1] == '.' && token[2] == '\0');
}

static void	handle_parent_token_bonus(char **tokens, int *top, int i,
		bool abs_path)
{
	if (*top > 0 && ft_strcmp(tokens[*top - 1], "..") != 0)
	{
		free(tokens[--(*top)]);
		free(tokens[i]);
	}
	else if (!abs_path)
		tokens[(*top)++] = tokens[i];
	else
		free(tokens[i]);
}

int	collect_normalized_tokens_bonus(char **tokens, bool abs_path)
{
	int	i;
	int	top;

	top = 0;
	i = 0;
	while (tokens[i])
	{
		if (is_empty_or_dot_token_bonus(tokens[i]))
			free(tokens[i]);
		else if (is_parent_token_bonus(tokens[i]))
			handle_parent_token_bonus(tokens, &top, i, abs_path);
		else
			tokens[top++] = tokens[i];
		i++;
	}
	return (top);
}
