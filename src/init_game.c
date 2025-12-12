#include "../Cub3D.h"

static int	init_mlx(t_game *game)
{
	game->mlx = mlx_init();
	if (!game->mlx)
		return (0);
	game->win = mlx_new_window(game->mlx, WINDOW_WIDTH, 
			WINDOW_HEIGHT, "Cub3D");
	if (!game->win)
		return (0);
	return (1);
}

static int	init_screen(t_game *game)
{
	game->screen = malloc(sizeof(t_img));
	if (!game->screen)
		return (0);
	game->screen->img = mlx_new_image(game->mlx, WINDOW_WIDTH, WINDOW_HEIGHT);
	if (!game->screen->img)
		return (0);
	game->screen->addr = mlx_get_data_addr(game->screen->img,
			&game->screen->bits_per_pixel,
			&game->screen->line_length,
			&game->screen->endian);
	game->screen->width = WINDOW_WIDTH;
	game->screen->height = WINDOW_HEIGHT;
	return (1);
}

static int	init_structs(t_game *game)
{
	game->ray = malloc(sizeof(t_ray));
	if (!game->ray)
		return (0);
	ft_bzero(game->ray, sizeof(t_ray));
	game->keys = malloc(sizeof(t_keys));
	if (!game->keys)
		return (0);
	ft_bzero(game->keys, sizeof(t_keys));
	return (1);
}

int	init_game(t_game *game)
{
	if (!init_mlx(game))
		return (0);
	if (!init_screen(game))
		return (0);
	if (!init_structs(game))
		return (0);
	if (!load_textures(game))
		return (0);
	game->running = 1;
	return (1);
}
