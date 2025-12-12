#include "../Cub3D.h"

static int	is_valid_neighbor(char **grid, int y, int x, int height, int width)
{
	if (y < 0 || y >= height || x < 0 || x >= width)
		return (0);
	if (grid[y][x] == ' ' || grid[y][x] == '\0')
		return (0);
	return (1);
}

static int	check_wall_closed(char **grid, int height, int width)
{
	int	i;
	int	j;

	i = 0;
	while (i < height)
	{
		j = 0;
		while (j < width && grid[i][j])
		{
			if (grid[i][j] == '0' || grid[i][j] == 'N' || 
				grid[i][j] == 'S' || grid[i][j] == 'E' || grid[i][j] == 'W')
			{
				if (i == 0 || i == height - 1)
					return (0);
				if (!is_valid_neighbor(grid, i - 1, j, height, width))
					return (0);
				if (!is_valid_neighbor(grid, i + 1, j, height, width))
					return (0);
				if (!is_valid_neighbor(grid, i, j - 1, height, width))
					return (0);
				if (!is_valid_neighbor(grid, i, j + 1, height, width))
					return (0);
			}
			j++;
		}
		i++;
	}
	return (1);
}

static void	init_player_direction(t_player *player, char dir)
{
	if (dir == 'N')
	{
		player->dir_x = 0;
		player->dir_y = -1;
		player->plane_x = 0.66;
		player->plane_y = 0;
	}
	else if (dir == 'S')
	{
		player->dir_x = 0;
		player->dir_y = 1;
		player->plane_x = -0.66;
		player->plane_y = 0;
	}
	else if (dir == 'E')
	{
		player->dir_x = 1;
		player->dir_y = 0;
		player->plane_x = 0;
		player->plane_y = 0.66;
	}
	else if (dir == 'W')
	{
		player->dir_x = -1;
		player->dir_y = 0;
		player->plane_x = 0;
		player->plane_y = -0.66;
	}
}

static int	set_player_position(t_game *game)
{
	int	i;
	int	j;

	i = 0;
	while (i < game->map->height)
	{
		j = 0;
		while (game->map->grid[i][j])
		{
			if (game->map->grid[i][j] == 'N' || game->map->grid[i][j] == 'S' || 
				game->map->grid[i][j] == 'E' || game->map->grid[i][j] == 'W')
			{
				game->player->pos_x = (double)j + 0.5;
				game->player->pos_y = (double)i + 0.5;
				game->player->start_dir = game->map->grid[i][j];
				init_player_direction(game->player, game->map->grid[i][j]);
				game->map->grid[i][j] = '0';
				return (1);
			}
			j++;
		}
		i++;
	}
	return (0);
}

int	validate_and_init_map(t_game *game)
{
	if (!validate_map_chars(game->map))
	{
		write(2, "Char validation failed\n", 23);
		return (0);
	}
	if (!check_wall_closed(game->map->grid, game->map->height, game->map->width))
	{
		write(2, "Wall check failed\n", 18);
		return (0);
	}
	if (!flood_fill_check(game->map))
	{
		write(2, "Flood fill failed\n", 18);
		return (0);
	}
	if (!set_player_position(game))
	{
		write(2, "Player position failed\n", 23);
		return (0);
	}
	game->map->is_valid = 1;
	return (1);
}
