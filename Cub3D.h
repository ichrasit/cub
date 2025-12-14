#ifndef CUB3D_H
# define CUB3D_H

# include <math.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <stdio.h>
# include "libft/libft.h"
# include "minilibx-linux/mlx.h"

# define WIN_W 1280
# define WIN_H 720
# define MOV_SPD 0.06
# define ROT_SPD 0.04

# define K_W 119
# define K_A 97
# define K_S 115
# define K_D 100
# define K_LF 65361
# define K_RT 65363
# define K_ESC 65307

typedef struct s_img
{
	void	*ptr;
	char	*addr;
	int		bpp;
	int		len;
	int		end;
	int		w;
	int		h;
}	t_img;

typedef struct s_tex
{
	t_img	*n;
	t_img	*s;
	t_img	*e;
	t_img	*w;
	char	*n_path;
	char	*s_path;
	char	*e_path;
	char	*w_path;
}	t_tex;

typedef struct s_plr
{
	double	px;
	double	py;
	double	dx;
	double	dy;
	double	cx;
	double	cy;
	char	dir;
}	t_plr;

typedef struct s_ray
{
	double	cam;
	double	rdx;
	double	rdy;
	int		mx;
	int		my;
	double	sdx;
	double	sdy;
	double	ddx;
	double	ddy;
	double	pwd;
	int		stx;
	int		sty;
	int		hit;
	int		side;
	int		lh;
	int		ds;
	int		de;
	int		tx;
}	t_ray;

typedef struct s_map
{
	char	**grid;
	int		w;
	int		h;
	int		f_col;
	int		c_col;
	int		ok;
}	t_map;

typedef struct s_keys
{
	int	w;
	int	a;
	int	s;
	int	d;
	int	l;
	int	r;
}	t_keys;

typedef struct s_game
{
	void	*mlx;
	void	*win;
	t_img	*scr;
	t_map	*map;
	t_plr	*plr;
	t_tex	*tex;
	t_ray	*ray;
	t_keys	*key;
}	t_game;

// Main & Init
int		init_game(t_game *g);
int		end_game(t_game *g);
void	clean_exit(t_game *g, char *m, int c);

// Parsing
int		parse_map(t_game *g, char *f);
int		read_config(t_game *g, char *f);
char	*get_next_line(int fd);
void	clear_gnl_fd(int fd);
int		contiguous_map_block(char *f);
int		is_map_line(char *s);
int		is_map(char *s);
int		get_dim(char *f, int *h, int *w);
int		fill_grid(char *f, char **g, int h);
char	**new_grid(int h, int w);

// Validation
int		valid_chars(t_map *m);
int		no_empty_gaps(t_map *m);
int		check_walls(char **gr, int h, int w);
int		set_player(t_game *g);
int		validate_and_init_map(t_game *g);
int		check_flood(t_map *m);

// Utils
char	**dup_arr(t_map *m);
void	free_arr(char **a, int h);
int		find_pos(t_map *m, int *x, int *y);

// Texture
int		load_tex(t_game *g);
void	free_tex(t_game *g);
void	calc_tx(t_game *g, t_img *t);

// Render & Raycast
int		render(t_game *g);
void	set_ray(t_game *g, int x);
void	set_step(t_game *g);
void	run_dda(t_game *g);
void	calc_wall(t_game *g);
t_img	*get_texture(t_game *g);

// Inputs & Movement
int		k_down(int k, t_game *g);
int		k_up(int k, t_game *g);
void	inputs(t_game *g);
void	strafe(t_game *g, double d);
void	rotate(t_game *g, double d);
int		is_wall(t_game *g, double x, double y);

#endif