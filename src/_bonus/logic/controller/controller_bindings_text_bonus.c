/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controller_bindings_text_bonus.c                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 04:35:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/08 04:35:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config_bonus.h"
#include "structs.h"
#include "logic_bonus.h"

static void	write_prefixed_int(char *buffer, size_t size,
		const char *prefix, int value)
{
	char	*num;

	ft_strlcpy(buffer, prefix, size);
	num = ft_itoa(value);
	if (!num)
		return ;
	ft_strlcat(buffer, num, size);
	free(num);
}

static void	append_axis_sign(char *buffer, size_t size, int dir)
{
	if (dir < 0)
		ft_strlcat(buffer, "-", size);
	else
		ft_strlcat(buffer, "+", size);
}

static const char	*controller_bind_text(const t_controller_bind *bind,
		char *buffer, size_t size)
{
	if (!buffer || size == 0)
		return ("");
	buffer[0] = '\0';
	if (!bind || bind->type == CONTROLLER_BIND_NONE)
	{
		ft_strlcpy(buffer, "-", size);
		return (buffer);
	}
	if (bind->type == CONTROLLER_BIND_BUTTON)
	{
		write_prefixed_int(buffer, size, "B", bind->id);
		return (buffer);
	}
	if (bind->type == CONTROLLER_BIND_AXIS)
	{
		write_prefixed_int(buffer, size, "A", bind->id);
		append_axis_sign(buffer, size, bind->dir);
		return (buffer);
	}
	ft_strlcpy(buffer, "-", size);
	return (buffer);
}

void	controller_refresh_texts(t_game *game)
{
	int		i;
	char	buf[CONFIG_MODAL_KEY_LABEL_LEN];

	if (!game)
		return ;
	i = 0;
	while (i < CONFIG_MODAL_CONTROL_COUNT)
	{
		controller_bind_text(&game->controller.binds[i], buf, sizeof(buf));
		ft_strlcpy(game->menu.controls_controller_text[i], buf,
			CONFIG_MODAL_KEY_LABEL_LEN);
		i++;
	}
}
