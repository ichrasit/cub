#include "../Cub3D.h"
#include <stdio.h>


int	chk_ext(char *s)
{
	int len;

	if (!s)
		return (0);
	len = ft_strlen(s);
	if (len < 5)
		return (0);
	return (ft_strncmp(s + len - 4, ".cub", 4) == 0);
}

static void	free_map(t_game *g)
{
	int i;

	if (g->map && g->map->grid)
	{
		i = 0;
		while (i < g->map->h)
			free(g->map->grid[i++]);
		free(g->map->grid);
	}
	if (g->map)
		free(g->map);
	if (g->plr)
		free(g->plr);
	if (g->ray)
		free(g->ray);
	if (g->key)
		free(g->key);
}

void	clean_exit(t_game *g, char *m, int c)
{
	if (m)
		ft_putendl_fd(m, 2);
	free_map(g);
	free_tex(g);
	if (g->tex)
		free(g->tex);
	if (g->scr)
	{
		if (g->scr->ptr)
			mlx_destroy_image(g->mlx, g->scr->ptr);
		free(g->scr);
	}
	if (g->win)
		mlx_destroy_window(g->mlx, g->win);
	if (g->mlx)
	{
		mlx_destroy_display(g->mlx);
		free(g->mlx);
	}
	exit(c);
}
int	end_game(t_game *g)
{
	clean_exit(g, NULL, 0);
	return (0);
}

int	main(int ac, char **av)
{
	t_game g;

	if (ac != 2)
		return (printf("Error\nUsage: ./cub3D maps/map.cub\n"), 1);
	if (!chk_ext(av[1]))
		return (printf("Error\nInvalid extension\n"), 1);
	ft_bzero(&g, sizeof(t_game));
	if (!parse_map(&g, av[1]))
		clean_exit(&g, "Error\nMap parsing failed", 1);
	if (!init_game(&g))
		clean_exit(&g, "Error\nInitialization failed", 1);
	mlx_loop_hook(g.mlx, render, &g);
	mlx_hook(g.win, 2, 1L << 0, k_down, &g);
	mlx_hook(g.win, 3, 1L << 1, k_up, &g);
	mlx_hook(g.win, 17, 0, end_game, &g);
	mlx_loop(g.mlx);
	return (0);
}
