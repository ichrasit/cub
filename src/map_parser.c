#include "../Cub3D.h"

/*
	parse map fonksiyonu oyun verileri için hafızada yer ayırır
	read config fonksiyonu ile duvar ve renk bilgilerini okur
	contiguous map block fonksiyonu ile harita da sızıntı var mı kontrol eder
	fill grid fonksiyonu haritanın boyutu ölçülür ve hafızaya kaydedilir.
	get dim fonksiyonu haritanın genişlik ve yükseklik değerlerini alır.
	validate and init map harita doğru mu ve player durumunu kontrol eder.
*/
static int	alloc_structs(t_game *g)
{
	g->map = malloc(sizeof(t_map));
	if (!g->map)
		return (0);
	ft_bzero(g->map, sizeof(t_map));
	g->tex = malloc(sizeof(t_tex));
	if (!g->tex)
		return (0);
	ft_bzero(g->tex, sizeof(t_tex));
	g->plr = malloc(sizeof(t_plr));
	if (!g->plr)
		return (0);
	ft_bzero(g->plr, sizeof(t_plr));
	return (1);
}

int	parse_map(t_game *g, char *f)
{
	if (!alloc_structs(g))
		return (0);
	if (!read_config(g, f))
		return (0);
	if (!contiguous_map_block(f))
		return (ft_putendl_fd("Error\nMap block is not contiguous", 2), 0);
	if (!get_dim(f, &g->map->h, &g->map->w))
		return (0);
	g->map->grid = new_grid(g->map->h, g->map->w);
	if (!g->map->grid)
		return (0);
	if (!fill_grid(f, g->map->grid, g->map->h))
		return (0);
	return (validate_and_init_map(g));
}