/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_grid.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htekdemi <htekdemi@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 18:45:19 by htekdemi          #+#    #+#             */
/*   Updated: 2026/01/09 18:45:20 by htekdemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Cub3D.h"

static char	**alloc_grid(int h)
{
	char	**g;

	g = malloc(sizeof(char *) * (h + 1));
	if (!g)
		return (NULL);
	g[h] = NULL;
	return (g);
}

static int	alloc_row(char **g, int i, int w)
{
	g[i] = malloc(w + 1);
	if (!g[i])
	{
		while (--i >= 0)
			free(g[i]);
		free(g);
		return (0);
	}
	ft_memset(g[i], ' ', w);
	g[i][w] = '\0';
	return (1);
}

char	**new_grid(int h, int w)
{
	char	**g;
	int		i;

	g = alloc_grid(h);
	if (!g)
		return (NULL);
	i = 0;
	while (i < h)
	{
		if (!alloc_row(g, i, w))
			return (NULL);
		i++;
	}
	return (g);
}
