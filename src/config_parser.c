#include "../Cub3D.h"

static int	is_texture_line(char *line, char **path, char *id)
{
	int	i;
	int	j;

	i = 0;
	while (line[i] == ' ' || line[i] == '\t')
		i++;
	if (ft_strncmp(line + i, id, ft_strlen(id)) != 0)
		return (0);
	i += ft_strlen(id);
	while (line[i] == ' ' || line[i] == '\t')
		i++;
	j = i;
	while (line[j] && line[j] != '\n' && line[j] != ' ' && line[j] != '\t')
		j++;
	*path = malloc(j - i + 1);
	if (!*path)
		return (0);
	j = 0;
	while (line[i] && line[i] != '\n' && line[i] != ' ' && line[i] != '\t')
		(*path)[j++] = line[i++];
	(*path)[j] = '\0';
	return (1);
}

static int	parse_color(char *line, int *color)
{
	int	i;
	int	r;
	int	g;
	int	b;

	i = 0;
	while (line[i] == ' ' || line[i] == '\t')
		i++;
	i++;
	while (line[i] == ' ' || line[i] == '\t')
		i++;
	r = ft_atoi(line + i);
	while (line[i] && line[i] != ',')
		i++;
	i++;
	g = ft_atoi(line + i);
	while (line[i] && line[i] != ',')
		i++;
	i++;
	b = ft_atoi(line + i);
	if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255)
		return (0);
	*color = (r << 16) | (g << 8) | b;
	return (1);
}

static int	parse_config_line(char *line, t_game *game)
{
	int	i;

	i = 0;
	while (line[i] == ' ' || line[i] == '\t')
		i++;
	if (line[i] == 'N' && line[i + 1] == 'O' && line[i + 2] == ' ')
		return (is_texture_line(line, &game->texture->north_path, "NO"));
	else if (line[i] == 'S' && line[i + 1] == 'O' && line[i + 2] == ' ')
		return (is_texture_line(line, &game->texture->south_path, "SO"));
	else if (line[i] == 'W' && line[i + 1] == 'E' && line[i + 2] == ' ')
		return (is_texture_line(line, &game->texture->west_path, "WE"));
	else if (line[i] == 'E' && line[i + 1] == 'A' && line[i + 2] == ' ')
		return (is_texture_line(line, &game->texture->east_path, "EA"));
	else if (line[i] == 'F' && line[i + 1] == ' ')
		return (parse_color(line, &game->map->floor_color));
	else if (line[i] == 'C' && line[i + 1] == ' ')
		return (parse_color(line, &game->map->ceiling_color));
	return (1);
}

int	parse_config(t_game *game, char *filename)
{
	int		fd;
	char	*line;
	int count;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (0);
	line = get_next_line(fd);
	count = 0;
	while (line)
	{
		if (line[0] != '\n' && line[0] != ' ' && line[0] != '\t' && line[0] != '1')
		{
			if (!parse_config_line(line, game))
			{
				free(line);
				close(fd);
				return (0);
			}
			else
				count++;
		}
		else if (line[0] == '0')
			break;
		printf("%s\n", line);
		free(line);
		line = get_next_line(fd);
	}
	if (line)
		free(line);
	close(fd);
	return 1;
}
