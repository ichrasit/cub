#include "../Cub3D.h"

static t_img	*load_texture(void *mlx, char *path)
{
	t_img	*texture;

	if (!path)
		return (NULL);
	texture = malloc(sizeof(t_img));
	if (!texture)
		return (NULL);
	texture->img = mlx_xpm_file_to_image(mlx, path, 
			&texture->width, &texture->height);
	if (!texture->img)
	{
		free(texture);
		return (NULL);
	}
	texture->addr = mlx_get_data_addr(texture->img, 
			&texture->bits_per_pixel,
			&texture->line_length, 
			&texture->endian);
	return (texture);
}

int	load_textures(t_game *game)
{
	if (!game->texture->north_path || !game->texture->south_path ||
		!game->texture->east_path || !game->texture->west_path)
		return (0);
	game->texture->north = load_texture(game->mlx, game->texture->north_path);
	if (!game->texture->north)
		return (0);
	game->texture->south = load_texture(game->mlx, game->texture->south_path);
	if (!game->texture->south)
		return (0);
	game->texture->east = load_texture(game->mlx, game->texture->east_path);
	if (!game->texture->east)
		return (0);
	game->texture->west = load_texture(game->mlx, game->texture->west_path);
	if (!game->texture->west)
		return (0);
	return (1);
}

void	free_textures(t_game *game)
{
	if (game->texture)
	{
		if (game->texture->north_path)
			free(game->texture->north_path);
		if (game->texture->south_path)
			free(game->texture->south_path);
		if (game->texture->east_path)
			free(game->texture->east_path);
		if (game->texture->west_path)
			free(game->texture->west_path);
		if (game->texture->north && game->texture->north->img)
		{
			mlx_destroy_image(game->mlx, game->texture->north->img);
			free(game->texture->north);
		}
		if (game->texture->south && game->texture->south->img)
		{
			mlx_destroy_image(game->mlx, game->texture->south->img);
			free(game->texture->south);
		}
		if (game->texture->east && game->texture->east->img)
		{
			mlx_destroy_image(game->mlx, game->texture->east->img);
			free(game->texture->east);
		}
		if (game->texture->west && game->texture->west->img)
		{
			mlx_destroy_image(game->mlx, game->texture->west->img);
			free(game->texture->west);
		}
	}
}
