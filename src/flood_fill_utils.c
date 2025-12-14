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
