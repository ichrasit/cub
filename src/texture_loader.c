#include "../Cub3D.h"

void	calc_tx(t_game *g, t_img *t)
{
	t_plr *p;
	t_ray *r;
	double wx;

	p = g->plr;
	r = g->ray;
	if (r->side == 0)
		wx = p->py + r->pwd * r->rdy;
	else
		wx = p->px + r->pwd * r->rdx;
	wx -= floor(wx);
	r->tx = (int)(wx * (double)t->w);
	if (r->side == 0 && r->rdx > 0)
		r->tx = t->w - r->tx - 1;
	if (r->side == 1 && r->rdy < 0)
		r->tx = t->w - r->tx - 1;
}

static t_img	*img_load(void *mlx, char *p)
{
	t_img *t;

	if (!p)
		return (NULL);
	t = malloc(sizeof(t_img));
	if (!t)
		return (NULL);
	t->ptr = mlx_xpm_file_to_image(mlx, p, &t->w, &t->h);
	if (!t->ptr)
	{
		free(t);
		return (NULL);
	}
	t->addr = mlx_get_data_addr(t->ptr, &t->bpp, &t->len, &t->end);
	return (t);
}

int	load_tex(t_game *g)
{
	t_tex *t;

	t = g->tex;
	if (!t->n_path || !t->s_path || !t->e_path || !t->w_path)
		return (0);
	t->n = img_load(g->mlx, t->n_path);
	if (!t->n)
		return (0);
	t->s = img_load(g->mlx, t->s_path);
	if (!t->s)
		return (0);
	t->e = img_load(g->mlx, t->e_path);
	if (!t->e)
		return (0);
	t->w = img_load(g->mlx, t->w_path);
	if (!t->w)
		return (0);
	return (1);
}

static void	free_img(t_game *g, t_img *img)
{
	if (img)
	{
		if (img->ptr)
			mlx_destroy_image(g->mlx, img->ptr);
		free(img);
	}
}

void	free_tex(t_game *g)
{
	t_tex *t;

	t = g->tex;
	if (!t)
		return ;
	if (t->n_path)
		free(t->n_path);
	if (t->s_path)
		free(t->s_path);
	if (t->e_path)
		free(t->e_path);
	if (t->w_path)
		free(t->w_path);
	free_img(g, t->n);
	free_img(g, t->s);
	free_img(g, t->e);
	free_img(g, t->w);
}
