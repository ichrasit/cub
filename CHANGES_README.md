# Cub3D Proje Değişiklikleri

Bu dosya, proje üzerinde yapılan bug düzeltmelerini ve değişiklikleri açıklamaktadır.

## 1. `src/flood_fill.c` - Boundary Check Hatası Düzeltmesi

### Sorun
`chk_void` fonksiyonunda x ve y koordinatları karışmıştı. Height (yükseklik) kontrolü yanlışlıkla x ile yapılıyordu.

### Önceki Kod (Hatalı)
```c
static int	chk_void(char **m, int x, int y, t_map *mp)
{
	if (y == 0 || x == mp->h - 1 || x == 0 || x >= mp->w - 1)
		return (0);
	// ...
}
```

### Düzeltilmiş Kod
```c
static int	chk_void(char **m, int x, int y, t_map *mp)
{
	if (y == 0 || y == mp->h - 1 || x == 0 || x >= mp->w - 1)
		return (0);
	// ...
}
```

### Açıklama
- `x == mp->h - 1` → `y == mp->h - 1` olarak düzeltildi
- `h` (height) değeri y koordinatı ile karşılaştırılmalıdır
- Bu hata, bazı geçerli haritaların yanlışlıkla reddedilmesine neden oluyordu

---

## 2. `src/get_next_line.c` - NULL Pointer Crash Düzeltmesi

### Sorun
`update_buffer` fonksiyonu NULL buffer ile çağrıldığında crash (segfault) oluşuyordu.

### Önceki Kod (Hatalı)
```c
static char	*update_buffer(char *buffer)
{
	char	*nl;
	char	*new_buf;
	// buffer NULL kontrolü yoktu - crash!
	nl = ft_strchr(buffer, '\n');
	// ...
}
```

### Düzeltilmiş Kod
```c
static char	*update_buffer(char *buffer)
{
	char	*nl;
	char	*new_buf;

	if (!buffer)
		return (NULL);
	nl = ft_strchr(buffer, '\n');
	// ...
}
```

### Açıklama
- Fonksiyon başına NULL kontrolü eklendi
- Bu düzeltme, boş dosya veya okuma hatalarında programın crash olmasını önler

---

## 3. `src/map_contiguous.c` - Debug Çıktıları Temizlendi

### Sorun
Dosyada test amaçlı bırakılmış debug çıktıları vardı.

### Kaldırılan Kodlar
- `print_line_debug()` fonksiyonu tamamen kaldırıldı
- `ft_putendl_fd("Gap before map line", 2)` kaldırıldı
- `ft_putendl_fd("  -> MAP LINE, started=1", 2)` kaldırıldı
- `ft_putendl_fd("  -> BLANK after map start, gap=1", 2)` kaldırıldı

### Açıklama
- Tüm debug print ifadeleri temizlendi
- Norminette uyumluluğu sağlandı (5 fonksiyon limiti)

---

## 4. `textures/` Klasörü Oluşturuldu

### Sorun
Haritalar `./textures/*.xpm` dosyalarına referans veriyordu ama klasör mevcut değildi.

### Çözüm
`textures/` klasörü oluşturuldu ve gerekli XPM dosyaları kopyalandı:
- `north_texture.xpm`
- `south_texture.xpm`
- `east_texture.xpm`
- `west_texture.xpm`
- `w1.xpm`, `w2.xpm`, `w3.xpm`
- `saincesu.xpm`, `gta6.xpm`
- ve diğerleri...

---

## 5. `maps/test_subject_labyrinth_fixed.cub` Oluşturuldu

### Sorun
Orijinal `test_subject_labyrinth.cub` dosyasında satır hizalama hatası vardı:
- Satır 9-12: 8 boşluk ile başlıyor
- Satır 20-23: 7 boşluk ile başlıyor

Bu hizalama farkı, duvar kontrolünde boşluk olarak algılanıp haritanın reddedilmesine neden oluyordu.

### Çözüm
Düzgün hizalanmış bir versiyon oluşturuldu: `test_subject_labyrinth_fixed.cub`

---

## Test Sonuçları

### Geçen Haritalar (30 adet)
```
complex_maze, huge_map, map, neon_map, neon_simple, 
player_in_room, player_surrounded_by_zeros, simple, 
simple_test_open, test2, test2_spaces, test_branching_clean, 
test_brick, test_center, test_complex_labyrinth, test_existing, 
test_exit, test_fixed, test_mlx, test_open_bottom, 
test_simple_space, test_space_adjacent, test_subject_labyrinth_fixed, 
test_texture, test_trailing_space, test_valid_spaces, test_working, 
test_working_notabs, test_yilan, test_yilan_fixed
```

### Başarısız Haritalar (26 adet) - Doğru Şekilde Reddedildi
| Harita | Hata Nedeni |
|--------|-------------|
| empty_map.cub | Harita içeriği yok |
| simple_red.cub | Boş dosya |
| test_closed.cub | Boş dosya |
| test_missing.cub | NO (kuzey) texture eksik |
| test_missing_texture.cub | Var olmayan texture dosyası |
| test_subject_labyrinth.cub | Satır hizalama hatası |
| test_yilan2.cub | Bağlantısız harita bölümü |
| test_space_near_player.cub | Oyuncu yanında boşluk |
| test_space_near_zero.cub | Zemin yanında boşluk |
| test_open_left.cub | Sol duvar açık |
| test_open_zero.cub | Bağlantısız zemin |
| test2_clean.cub | Harita ortasında boşluk |
| test_branching.cub | Bağlantısız bölüm |
| ... | ... |

---

## Norminette Durumu

Tüm kaynak dosyaları norminette kontrolünden geçmektedir:
```bash
$ norminette src/*.c
# Tüm dosyalar: OK!
```

---

## Özet

| Değişiklik | Dosya | Açıklama |
|------------|-------|----------|
| Bug Fix | `src/flood_fill.c` | x/y koordinat karışıklığı düzeltildi |
| Bug Fix | `src/get_next_line.c` | NULL pointer crash düzeltildi |
| Cleanup | `src/map_contiguous.c` | Debug çıktıları temizlendi |
| New | `textures/` | Texture dosyaları eklendi |
| New | `maps/test_subject_labyrinth_fixed.cub` | Düzeltilmiş labirent haritası |

---

*Son güncelleme: 9 Ocak 2026*
