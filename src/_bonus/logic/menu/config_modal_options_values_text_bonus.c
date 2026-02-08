/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_modal_options_values_text_bonus.c           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 05:40:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/08 05:40:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config_bonus.h"
#include "structs.h"
#include "logic_bonus.h"

static void	write_int_value(char *buffer, size_t buffer_size, int value)
{
	char	*num;

	if (!buffer || buffer_size == 0)
		return ;
	buffer[0] = '\0';
	num = ft_itoa(value);
	if (!num)
		return ;
	ft_strlcpy(buffer, num, buffer_size);
	free(num);
}

static bool	write_fps_limit_text(char *buffer, size_t size, int raw)
{
	int	value;

	if (raw < 0)
		raw = 0;
	if (raw > 5)
		raw = 5;
	value = config_fps_limit_value(raw);
	if (value < 0)
		ft_strlcpy(buffer, "UNLIMITED", size);
	else
		write_int_value(buffer, size, value);
	return (true);
}

static bool	write_slider_special_text(int slider, int raw, char *buffer,
			size_t buffer_size)
{
	static const char	*labels[CONFIG_QUALITY_COUNT] = {
		"LOW", "MEDIUM", "HIGH", "ULTRA"
	};

	if (slider == CONFIG_SLIDER_FPS_LIMIT)
		return (write_fps_limit_text(buffer, buffer_size, raw));
	if (slider == CONFIG_SLIDER_QUALITY)
	{
		if (raw < 0)
			raw = 0;
		if (raw >= CONFIG_QUALITY_COUNT)
			raw = CONFIG_QUALITY_COUNT - 1;
		ft_strlcpy(buffer, labels[raw], buffer_size);
		return (true);
	}
	return (false);
}

void	config_option_slider_text(t_game *game, int index, char *buffer,
			size_t buffer_size)
{
	int	slider;
	int	raw;

	if (!buffer || buffer_size == 0)
		return ;
	buffer[0] = '\0';
	if (index < CONFIG_MODAL_TOGGLE_COUNT || index >= CONFIG_MODAL_OPTION_COUNT)
		return ;
	slider = index - CONFIG_MODAL_TOGGLE_COUNT;
	raw = config_option_slider_raw(game, slider);
	if (write_slider_special_text(slider, raw, buffer, buffer_size))
		return ;
	write_int_value(buffer, buffer_size, raw);
}
