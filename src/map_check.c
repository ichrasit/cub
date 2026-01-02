#include "../Cub3D.h"

static int	is_ok(char **g, int y, int x, t_map *m)
{
	if (y < 0 || y >= m->h || x < 0 || x >= m->w)
		return (0);
	if (g[y][x] == ' ' || g[y][x] == '\0')
		return (0);
	return (1);
}

int	check_walls(char **g, int h, int w)
{
	int i;
	int j;
	t_map m;

	m.h = h;
	m.w = w;
	i = 0;
	while (i < h)
	{
		j = 0;
		while (j < w && g[i][j])
		{
			if (ft_strchr("0NSEW", g[i][j]))
			{
				if (i == 0 || i == h - 1)
					return (0);
				if (!is_ok(g, i - 1, j, &m) || !is_ok(g, i + 1, j, &m))
					return (0);
				if (!is_ok(g, i, j - 1, &m) || !is_ok(g, i, j + 1, &m))
					return (0);
			}
			j++;
		}
		i++;
	}
	return (1);
}

/*
	oyuncunun başlangıç yönünü ayarlar
	bunu dx, dy, cx, cy değerlerini ayarlayarak yapar
	yönü N,S,E,W olabilir
	-1 ve 1 değerleri yön vektörünü belirler
	cx ve cy ise kamera düzlemini belirler
*/
static void	set_dir(t_plr *p, char d)
{
	if (d == 'N')
	{
		p->dy = -1;
		p->cx = 0.66;
	}
	else if (d == 'S')
	{
		p->dy = 1;
		p->cx = -0.66;
	}
	else if (d == 'E')
	{
		p->dx = 1;
		p->cy = 0.66;
	}
	else if (d == 'W')
	{
		p->dx = -1;
		p->cy = -0.66;
	}
}

/*
	Bu fonksiyon harita ızgarasında oyuncunun başlangıç pozisyonunu bulur
	j x ekseni 
	i y ekseni
	harita ızgarasında oyuncunun başlangıç pozisyonunu bulur
	oyuncunun px ve py pozisyonlarını ayarlar
	dir yani bakış yönünü ayarlar
	nsew karakterini 0 ile değiştirir çünkü haritanın bir parçası değil
	Kısaca;
	"Haritada oyuncu var mı?
	Varsa: Ortasına koy, Yönünü ayarla, Raycasting'i kur, Haritadan sil, Çık"
*/
int	set_player(t_game *g)
{
	int i;
	int j;

	i = 0;
	while (i < g->map->h)
	{
		j = 0;
		while (g->map->grid[i][j])
		{
			if (ft_strchr("NSEW", g->map->grid[i][j]))
			{
				g->plr->px = (double)j + 0.5;
				g->plr->py = (double)i + 0.5;
				g->plr->dir = g->map->grid[i][j];
				set_dir(g->plr, g->map->grid[i][j]);
				g->map->grid[i][j] = '0';
				return (1);
			}
			j++;
		}
		i++;
	}
	return (0);
}

/*
	harita belleğe alındı ancak hala oynamaya uygun mu bilinmiyor
	bu fonksiyon haritanın geçerli olup olmadığını kontrol eder
	geçerli karakterler mi var
	haritada boşluklar var mı
	harita duvarlarla çevrili mi
	harita tek parça mı
	oyuncu başlangıç pozisyonu ayarlanır
	eğer tüm kontroller geçilirse harita geçerli olarak işaretlenir
*/
int	validate_and_init_map(t_game *g)
{
	if (!valid_chars(g->map))
		return (0);
	if (!no_empty_gaps(g->map))
		return (0);
	if (!check_walls(g->map->grid, g->map->h, g->map->w))
		return (0);
	if (!check_flood(g->map))
		return (0);
	if (!set_player(g))
		return (0);
	g->map->ok = 1;
	return (1);
}

/*
	Bu fonksiyon map butunlugu saglandiktan sonra yapilan islemdir. Map parse edildikten sonra buraya gelir ki artik map oynamaya hazir hale gelsin.
	Ilk once valid_chars fonksiyonuna gider. Bu fonksiyon mapte istenilen karakterler var mi bunlari kontrol eder, Eger yoksa 0 doner.
	Daha sonrasinda ise no_empty_gaps calisir. Bu fonksiyon map icerisinde herhangi bir yerde bosluk var mi harita butunlugu saglaniyor mu bunlari kontrol ediyor.
	Bundan sonra ise check_walls fonksiyonu calisir. Bu fonksiyon oyuncunun baslangic yonunu ayarlar. NSEW bu yonlerden biridir bu yon.
	Bu islemden sonra ise check_flood fonksiyonu calisir. Bu fonksiyon flood_fillin baslangicidir.
	En sonda ise set_player calisir. Bu fonksiyon flood_fillden sonra harita heap'e alindi fakat hala kesin olarak oynanabilinir mi emin degiliz bunlari kontrol edip oyuncuyu oyuna hazirlar.
	SOnrasinda ise map artik oynanmaya hazirdir ve flag 1 yapilir fonksiyon 1 OK dondurur.
*/
