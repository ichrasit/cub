/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flood_fill_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muhaoz <muhaoz@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 18:56:32 by htekdemi          #+#    #+#             */
/*   Updated: 2026/01/10 00:43:10 by muhaoz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Cub3D.h"

char	**dup_arr(t_map *map)
{
	char	**dup;
	int		i;

	dup = malloc(sizeof(char *) * (map->h + 1));
	if (!dup)
		return (NULL);
	i = 0;
	while (i < map->h)
	{
		dup[i] = ft_strdup(map->grid[i]);
		if (!dup[i])
		{
			while (--i >= 0)
				free(dup[i]);
			free(dup);
			return (NULL);
		}
		i++;
	}
	dup[i] = NULL;
	return (dup);
}

void	free_arr(char **a, int h)
{
	int	i;

	i = 0;
	while (i < h)
	{
		free(a[i]);
		i++;
	}
	free(a);
}

int	find_pos(t_map *map, int *x, int *y)
{
	int	i;
	int	j;

	i = 0;
	while (i < map->h)
	{
		j = 0;
		while (map->grid[i][j])
		{
			if (ft_strchr("NSEW", map->grid[i][j]))
			{
				*x = j;
				*y = i;
				return (1);
			}
			j++;
		}
		i++;
	}
	return (0);
}

static int	chk_void(char **m, int x, int y, t_map *mp)
{
	if (y == 0 || y == mp->h - 1 || x == 0 || x >= mp->w - 1)
		return (0);
	if (m[y - 1][x] == ' ' || m[y + 1][x] == ' ' || m[y][x - 1] == ' '
		|| m[y][x + 1] == ' ')
		return (0);
	return (1);
}

int	scan_filled(char **m, t_map *mp)
{
	int	i;
	int	j;

	i = 0;
	while (i < mp->h)
	{
		j = 0;
		while (j < mp->w && m[i][j])
		{
			if (m[i][j] == 'F')
			{
				if (!chk_void(m, j, i, mp))
					return (0);
			}
			j++;
		}
		i++;
	}
	return (1);
}
