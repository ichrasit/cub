#ifndef CUB3D_H
# define CUB3D_H

# include <math.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include "libft/libft.h"
# include "minilibx-linux/mlx.h"
#include <stdio.h>

# define WINDOW_WIDTH 1280
# define WINDOW_HEIGHT 720
# define TEX_WIDTH 64
# define TEX_HEIGHT 64
# define MOVE_SPEED 0.06
# define ROT_SPEED 0.04

# define KEY_W 119
# define KEY_A 97
# define KEY_S 115
# define KEY_D 100
# define KEY_LEFT 65361
# define KEY_RIGHT 65363
# define KEY_ESC 65307

# define ERR_MALLOC "Bellek ayırma hatası"
# define ERR_FILE "Dosya açma hatası"
# define ERR_MAP "Geçersiz harita"
# define ERR_TEXTURE "Texture yükleme hatası"

typedef struct s_color
{
	int	r;
	int	g;
	int	b;
	int	trgb;
}	t_color;

typedef struct s_img
{
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
	int		width;
	int		height;
}	t_img;

typedef struct s_texture
{
	t_img	*north;
	t_img	*south;
	t_img	*east;
	t_img	*west;
	char	*north_path;
	char	*south_path;
	char	*east_path;
	char	*west_path;
}	t_texture;

typedef struct s_player
{
	double	pos_x;
	double	pos_y;
	double	dir_x;
	double	dir_y;
	double	plane_x;
	double	plane_y;
	char	start_dir;
}	t_player;

typedef struct s_ray
{
	double	camera_x;
	double	ray_dir_x;
	double	ray_dir_y;
	int		map_x;
	int		map_y;
	double	side_dist_x;
	double	side_dist_y;
	double	delta_dist_x;
	double	delta_dist_y;
	double	perp_wall_dist;
	int		step_x;
	int		step_y;
	int		hit;
	int		side;
	int		line_height;
	int		draw_start;
	int		draw_end;
	int		tex_num;
	double	wall_x;
	int		tex_x;
	int		tex_y;
	double	step;
	double	tex_pos;
}	t_ray;

// Harita yapısı
typedef struct s_map
{
	char	**grid;
	int		width;
	int		height;
	int		floor_color;
	int		ceiling_color;
	int		is_valid;
}	t_map;

// Klavye kontrol yapısı
typedef struct s_keys
{
	int	w;
	int	a;
	int	s;
	int	d;
	int	left;
	int	right;
}	t_keys;

// Ana oyun yapısı
typedef struct s_game
{
	void		*mlx;
	void		*win;
	t_img		*screen;
	t_map		*map;
	t_player	*player;
	t_texture	*texture;
	t_ray		*ray;
	t_keys		*keys;
	int			running;
}	t_game;

// Ana fonksiyonlar
int		init_game(t_game *game);
int		parse_map(t_game *game, char *file);
int		render_frame(t_game *game);

// Movement fonksiyonları
void	move_forward(t_game *game);
void	move_backward(t_game *game);
void	move_left(t_game *game);
void	move_right(t_game *game);
void	rotate_left(t_game *game);
void	rotate_right(t_game *game);
void	process_movement(t_game *game);

// Map parsing ve validation
int		validate_map_chars(t_map *map);
int		validate_and_init_map(t_game *game);
int		flood_fill_check(t_map *map);
int		parse_config(t_game *game, char *filename);
char	*get_next_line(int fd);

// Texture loading
int		load_textures(t_game *game);
void	free_textures(t_game *game);

// Yardımcı fonksiyonlar
void	free_game(t_game *game);
int		error_exit(char *msg);
void	mlx_put_pixel(t_img *img, int x, int y, int color);
int		get_pixel_color(t_img *img, int x, int y);

// Hook fonksiyonları
int		key_press(int keycode, t_game *game);
int		key_release(int keycode, t_game *game);
int		close_game(t_game *game);
int		loop_hook(t_game *game);

#endif
