#include "../Cub3D.h"

static int	is_valid_char(char c)
{
	return (c == '0' || c == '1' || c == 'N' || c == 'S' || 
			c == 'E' || c == 'W' || c == ' ' || c == '\t' || 
			c == '\n' || c == '\r');
}

static int	count_players(char **grid, int height)
{
	int	i;
	int	j;
	int	count;

	count = 0;
	i = 0;
	while (i < height)
	{
		j = 0;
		while (grid[i][j])
		{
			if (grid[i][j] == 'N' || grid[i][j] == 'S' || 
				grid[i][j] == 'E' || grid[i][j] == 'W')
				count++;
			j++;
		}
		i++;
	}
	return (count);
}

int	validate_map_chars(t_map *map)
{
	int	i;
	int	j;
	int	player_count;

	if (!map || !map->grid)
		return (0);
	i = 0;
	while (i < map->height)
	{
		j = 0;
		while (map->grid[i][j])
		{
			if (!is_valid_char(map->grid[i][j]))
				return (0);
			j++;
		}
		i++;
	}
	player_count = count_players(map->grid, map->height);
	if (player_count != 1)
		return (0);
	return (1);
}
