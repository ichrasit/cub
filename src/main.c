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

static void	setup_hooks(t_game *g)
{
	mlx_loop_hook(g->mlx, render, g);
	mlx_hook(g->win, 2, 1L << 0, k_down, g);
	mlx_hook(g->win, 3, 1L << 1, k_up, g);
	mlx_hook(g->win, 17, 0, end_game, g);
}

static int	start_game(char *map_file)
{
	t_game g;

	ft_bzero(&g, sizeof(t_game));
	if (!parse_map(&g, map_file))
		clean_exit(&g, "Error\nMap parsing failed", 1);
	if (!init_game(&g))
		clean_exit(&g, "Error\nInitialization failed", 1);
	setup_hooks(&g);
	mlx_loop(g.mlx);
	return (0);
}

/*
	bu fonksiyon oyunu baslatir yani baslatmaya hazirlar yavas yavas.
	parse map fonksiyonuna mainden gelen av1'i yollar.
	parse map fonksiyonu mapde bir sorun var mi? 
	Map kurallara uygun mu yer yon bilgisi var mi bu gibi durumlari kontrol eder
	buradan gelen deger olumsuz ise hata verir fakat parse mapde bir allocate yapildigi icin clean_exit fonksiyonu ile cikis yapiyorum.
	init_game fonksiyonuma gonderiyorum burada bu fonksiyon struct yapimi initialize eder.
	Fakat burada da bir hata durumu olursa yine clean_exit atarim. Cunku allocate yaptik.
	Daha sonrasinda ise setup_hooks calisir.
	Burada raycasting ve klavye durumlari event hook icerisine alinir. Tipki bir activity monitor gibi. Bir loop degildir fakat oyun acik oldugu muddetce klavyeyi dinler.
	Buradan sonra ise oyunun surekli acik kalmasi icin mlx_loop calisir.

*/
int	main(int ac, char **av)
{
	if (ac != 2)
		return (printf("Error\nUsage: ./cub3D maps/map.cub\n"), 1);
	if (!chk_ext(av[1]))
		return (printf("Error\nInvalid extension\n"), 1);
	return (start_game(av[1]));
}

/*
	eger arguman sayim iki degilse hata verir
	dosya uzantisi kontrol ettigim fonksiyona argumani mi yollarim.
	gelen deger yoksa hata verir.
	sonrasinda her sey yolundadir ve start game fonksiyonuma gider.
*/