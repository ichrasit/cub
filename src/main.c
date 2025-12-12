#include "../Cub3D.h"
#include <stdio.h>

int	check_file_extension(char *filename)
{
	int	len;

	if (!filename)
		return (0);
	len = ft_strlen(filename);
	if (len < 5)
		return (0);
	if (ft_strncmp(filename + len - 4, ".cub", 4) != 0)
		return (0);
	return (1);
}

int	error_exit(char *msg)
{
	if (msg)
	{
		write(2, "Error\n", 6);
		write(2, msg, ft_strlen(msg));
		write(2, "\n", 1);
	}
	return (1);
}

void	free_game(t_game *game)
{
	int	i;

	if (!game)
		return ;
	if (game->map && game->map->grid)
	{
		i = 0;
		while (i < game->map->height)
		{
			if (game->map->grid[i])
				free(game->map->grid[i]);
			i++;
		}
		free(game->map->grid);
	}
	if (game->map)
		free(game->map);
	if (game->player)
		free(game->player);
	free_textures(game);
	if (game->texture)
		free(game->texture);
	if (game->ray)
		free(game->ray);
	if (game->keys)
		free(game->keys);
	if (game->screen && game->screen->img)
		mlx_destroy_image(game->mlx, game->screen->img);
	if (game->screen)
		free(game->screen);
	if (game->win)
		mlx_destroy_window(game->mlx, game->win);
	if (game->mlx)
	{
		mlx_destroy_display(game->mlx);
		free(game->mlx);
	}
}

int	main(int argc, char **argv)
{
	t_game	game;

	if (argc != 2)
		return (error_exit("Kullanım: ./cub3D <harita.cub>"));
	if (!check_file_extension(argv[1]))
		return (error_exit("Geçersiz dosya uzantısı. .cub dosyası gerekli"));
	ft_bzero(&game, sizeof(t_game));
	if (!parse_map(&game, argv[1]))
	{
		free_game(&game);
		return (error_exit(ERR_MAP));
	}
	if (!init_game(&game))
	{
		free_game(&game);
		return (error_exit("Oyun başlatma hatası"));
	}
	mlx_loop_hook(game.mlx, loop_hook, &game);
	mlx_hook(game.win, 2, 1L << 0, key_press, &game);
	mlx_hook(game.win, 3, 1L << 1, key_release, &game);
	mlx_hook(game.win, 17, 0, close_game, &game);
	mlx_loop(game.mlx);
	free_game(&game);
	return (0);
}
