#include "../Cub3D.h"
#include <stdio.h>

static char	**allocate_map_grid(int height, int width)
{
	char	**grid;
	int		i;

	grid = malloc(sizeof(char *) * (height + 1));
	if (!grid)
		return (NULL);
	i = 0;
	while (i < height)
	{
		grid[i] = malloc(sizeof(char) * (width + 1));
		if (!grid[i])
		{
			while (--i >= 0)
				free(grid[i]);
			free(grid);
			return (NULL);
		}
		ft_memset(grid[i], ' ', width);
		grid[i][width] = '\0';
		i++;
	}
	grid[height] = NULL;
	return (grid);
}

static int	is_map_line(char *line)
{
	int	i;

	if (!line)
		return (0);
	i = 0;
	while (line[i] == ' ' || line[i] == '\t')
		i++;
	printf("%s\n", line);
	if (line[i] == '1' || line[i] == '0')
		return (1);
	return (0);
}

static int	count_map_lines(char *filename, int *height, int *width)
{
	int		fd;
	char	*line;
	int		len;
	int t;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (0);
	*height = 0;
	*width = 0;
	t = 0;
	line = get_next_line(fd);
	while (line)
	{
		if (is_map_line(line))
		{
			t = 1;
			len = ft_strlen(line);
			if (len > 0 && line[len - 1] == '\n')
				len--;
			if (len > *width)
				*width = len;
			(*height)++;
		}
		if(t == 1 && (line[0] == '\0' && line[1] == '\n'))
			return (free(line), close(fd), 0);
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	return (1);
}

static int	read_map_lines(char *filename, char **grid, int height)
{
	int		fd;
	char	*line;
	int		i;
	int		j;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (0);
	i = 0;
	line = get_next_line(fd);
	while (line && i < height)
	{
		if (is_map_line(line))
		{
			j = 0;
			while (line[j] && line[j] != '\n')
			{
				grid[i][j] = line[j];
				j++;
			}
			i++;
		}
		free(line);
		line = get_next_line(fd);
	}
	if (line)
		free(line);
	close(fd);
	return (1);
}

int	parse_map(t_game *game, char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (0);
	close(fd);
	game->map = malloc(sizeof(t_map));
	if (!game->map)
		return (0);
	ft_bzero(game->map, sizeof(t_map));
	game->texture = malloc(sizeof(t_texture));
	if (!game->texture)
		return (0);
	ft_bzero(game->texture, sizeof(t_texture));
	game->player = malloc(sizeof(t_player));
	if (!game->player)
		return (0);
	ft_bzero(game->player, sizeof(t_player));
	if (!parse_config(game, filename))
		return (0);
	if (!count_map_lines(filename, &game->map->height, &game->map->width))
		return (0);
	if (game->map->height == 0 || game->map->width == 0)
		return (0);
	game->map->grid = allocate_map_grid(game->map->height, game->map->width);
	if (!game->map->grid)
		return (0);
	if (!read_map_lines(filename, game->map->grid, game->map->height))
		return (0);
	if (!validate_and_init_map(game))
		return (0);
	return (1);
}
