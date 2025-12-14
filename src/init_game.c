#include "../Cub3D.h"

static int	start_mlx(t_game *g)
{
	g->mlx = mlx_init();
	if (!g->mlx)
		return (0);
	g->win = mlx_new_window(g->mlx, WIN_W, WIN_H, "Cub3D");
	if (!g->win)
		return (0);
	return (1);
}

static int	make_screen(t_game *g)
{
	g->scr = malloc(sizeof(t_img));
	if (!g->scr)
		return (0);
	g->scr->ptr = mlx_new_image(g->mlx, WIN_W, WIN_H);
	if (!g->scr->ptr)
		return (0);
	g->scr->addr = mlx_get_data_addr(g->scr->ptr, &g->scr->bpp, &g->scr->len,
			&g->scr->end);
	g->scr->w = WIN_W;
	g->scr->h = WIN_H;
	return (1);
}

static int	alloc_mem(t_game *g)
{
	g->ray = malloc(sizeof(t_ray));
	if (!g->ray)
		return (0);
	ft_bzero(g->ray, sizeof(t_ray));
	g->key = malloc(sizeof(t_keys));
	if (!g->key)
		return (0);
	ft_bzero(g->key, sizeof(t_keys));
	return (1);
}

int	init_game(t_game *g)
{
	if (!start_mlx(g))
		return (0);
	if (!make_screen(g))
		return (0);
	if (!alloc_mem(g))
		return (0);
	if (!load_tex(g))
		return (0);
	return (1);
}
