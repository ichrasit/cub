#include "../Cub3D.h"

static void	flood_fill_recursive(char **map_copy, int x, int y, int height, int width)
{
	if (y < 0 || y >= height || x < 0 || x >= width)
		return ;
	if (map_copy[y][x] == '1' || map_copy[y][x] == 'F')
		return ;
	if (map_copy[y][x] == ' ')
		return ;
	map_copy[y][x] = 'F';
	flood_fill_recursive(map_copy, x + 1, y, height, width);
	flood_fill_recursive(map_copy, x - 1, y, height, width);
	flood_fill_recursive(map_copy, x, y + 1, height, width);
	flood_fill_recursive(map_copy, x, y - 1, height, width);
}

static char	**copy_map(char **grid, int height)
{
	char	**copy;
	int		i;

	copy = malloc(sizeof(char *) * (height + 1));
	if (!copy)
		return (NULL);
	i = 0;
	while (i < height)
	{
		copy[i] = ft_strdup(grid[i]);
		if (!copy[i])
		{
			while (--i >= 0)
				free(copy[i]);
			free(copy);
			return (NULL);
		}
		i++;
	}
	copy[i] = NULL;
	return (copy);
}

static void	free_map_copy(char **map_copy, int height)
{
	int	i;

	i = 0;
	while (i < height)
	{
		free(map_copy[i]);
		i++;
	}
	free(map_copy);
}

static int	find_player_position(char **grid, int height, int *x, int *y)
{
	int	i;
	int	j;

	i = 0;
	while (i < height)
	{
		j = 0;
		while (grid[i][j])
		{
			if (grid[i][j] == 'N' || grid[i][j] == 'S' || 
				grid[i][j] == 'E' || grid[i][j] == 'W')
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

static int	check_if_surrounded(char **map_copy, int x, int y, int height, int width)
{
	if (y == 0 || y == height - 1 || x == 0 || x >= width - 1)
		return (0);
	if (y > 0 && x < width && map_copy[y - 1][x] == ' ')
		return (0);
	if (y < height - 1 && x < width && map_copy[y + 1][x] == ' ')
		return (0);
	if (x > 0 && map_copy[y][x - 1] == ' ')
		return (0);
	if (x < width - 1 && map_copy[y][x + 1] == ' ')
		return (0);
	return (1);
}

static int	check_borders_after_fill(char **map_copy, int height, int width)
{
	int	i;
	int	j;

	i = 0;
	while (i < height)
	{
		j = 0;
		while (j < width && map_copy[i][j])
		{
			if (map_copy[i][j] == 'F')
			{
				if (!check_if_surrounded(map_copy, j, i, height, width))
					return (0);
			}
			j++;
		}
		i++;
	}
	return (1);
}

int	flood_fill_check(t_map *map)
{
	char	**map_copy;
	int		player_x;
	int		player_y;
	int		result;

	if (!map || !map->grid)
		return (0);
	if (!find_player_position(map->grid, map->height, &player_x, &player_y))
		return (0);
	map_copy = copy_map(map->grid, map->height);
	if (!map_copy)
		return (0);
	flood_fill_recursive(map_copy, player_x, player_y, 
		map->height, map->width);
	result = check_borders_after_fill(map_copy, map->height, map->width);
	free_map_copy(map_copy, map->height);
	return (result);
}
