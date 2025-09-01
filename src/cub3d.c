/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 01:33:16 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/01 11:25:33 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

int main(int argc, char **argv)
{
	if (argc != 2)
		return (printf("Error:\n\tUsage: %s <map_path>\n", argv[0]), 1);
	return 0;
}
