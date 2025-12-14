#include "../Cub3D.h"

void	my_px(t_img *img, int x, int y, int c)
{
	char *dst;
	int off;

	if (x < 0 || x >= WIN_W || y < 0 || y >= WIN_H)
		return ;
	off = (y * img->len + x * (img->bpp / 8));
	dst = img->addr + off;
	*(unsigned int *)dst = c;
}

static int	get_col(t_img *t, int x, int y)
{
	char *src;
	int off;

	if (x < 0 || x >= t->w || y < 0 || y >= t->h)
		return (0);
	off = (y * t->len + x * (t->bpp / 8));
	src = t->addr + off;
	return (*(unsigned int *)src);
}

static void	bg(t_game *g)
{
	int x;
	int y;
	int c;
	int f;

	c = g->map->c_col;
	f = g->map->f_col;
	y = 0;
	while (y < WIN_H)
	{
		x = 0;
		while (x < WIN_W)
		{
			if (y < WIN_H / 2)
				my_px(g->scr, x, y, c);
			else
				my_px(g->scr, x, y, f);
			x++;
		}
		y++;
	}
}

static void	draw(t_game *g, int x, t_img *t)
{
	int y;
	int ty;
	int c;
	double step;
	double pos;

	step = 1.0 * t->h / g->ray->lh;
	pos = (g->ray->ds - WIN_H / 2 + g->ray->lh / 2) * step;
	y = g->ray->ds;
	while (y < g->ray->de)
	{
		ty = (int)pos & (t->h - 1);
		pos += step;
		c = get_col(t, g->ray->tx, ty);
		if (g->ray->side == 1)
			c = (c >> 1) & 8355711;
		my_px(g->scr, x, y, c);
		y++;
	}
}

int	render(t_game *g)
{
	int x;
	t_img *t;

	inputs(g);
	bg(g);
	x = 0;
	while (x < WIN_W)
	{
		set_ray(g, x);
		set_step(g);
		run_dda(g);
		calc_wall(g);
		t = get_texture(g);
		calc_tx(g, t);
		draw(g, x, t);
		x++;
	}
	mlx_put_image_to_window(g->mlx, g->win, g->scr->ptr, 0, 0);
	return (0);
}
