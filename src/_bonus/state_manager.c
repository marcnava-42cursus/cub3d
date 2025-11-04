/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state_manager.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 17:36:12 by marcnava          #+#    #+#             */
/*   Updated: 2025/11/03 18:36:10 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"

typedef struct s_state
{
	char key;
	char state;
	struct s_state next;
} t_state;

// char change_state(t_game *game, )
