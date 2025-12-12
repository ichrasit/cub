# CUB3D - Detaylı Proje Dokümantasyonu

## 📋 İçindekiler
1. [Proje Genel Bakış](#proje-genel-bakış)
2. [Mimari ve Yapı](#mimari-ve-yapı)
3. [Veri Yapıları](#veri-yapıları)
4. [MLX Kütüphanesi](#mlx-kütüphanesi)
5. [Raycasting Algoritması](#raycasting-algoritması)
6. [Map Parsing ve Validation](#map-parsing-ve-validation)
7. [Texture Management](#texture-management)
8. [Hareket Sistemi](#hareket-sistemi)
9. [Fonksiyon Referansı](#fonksiyon-referansı)

---

## Proje Genel Bakış

CUB3D, Wolfenstein 3D'den esinlenerek geliştirilmiş bir 3D first-person shooter (FPS) görünüm motoru projesidir. Raycasting tekniği kullanarak 2D haritadan 3D perspektif görünümü oluşturur.

### Temel Özellikler
- ✅ Gerçek zamanlı 3D raycasting motoru
- ✅ Texture mapping (duvar dokuları)
- ✅ WASD ve yön tuşları ile hareket
- ✅ Çarpışma tespiti
- ✅ .cub formatında özel map dosyaları
- ✅ Özelleştirilebilir zemin ve tavan renkleri

### Teknik Detaylar
- **Grafik Kütüphanesi**: MiniLibX (X11 tabanlı)
- **Rendering Tekniği**: Raycasting
- **Resolüsyon**: 1280x720 piksel
- **Texture Boyutu**: 64x64 piksel
- **Programlama Dili**: C

---

## Mimari ve Yapı

### Proje Dizin Yapısı
```
CUB3D/
├── src/               # Kaynak kod dosyaları
│   ├── main.c         # Ana program giriş noktası
│   ├── init_game.c    # Oyun başlatma fonksiyonları
│   ├── render.c       # Render ve raycasting
│   ├── movement.c     # Hareket kontrolleri
│   ├── hooks.c        # Klavye event handlers
│   ├── map_parser.c   # Map dosyası okuma
│   ├── config_parser.c # Konfigürasyon parsing
│   ├── map_validation.c # Map validasyon
│   ├── map_check.c    # Duvar kontrolleri
│   ├── flood_fill.c   # Flood fill algoritması
│   ├── texture_loader.c # Texture yükleme
│   └── get_next_line.c # Dosya okuma utility
├── assets/            # Texture dosyaları (.xpm)
├── maps/              # Map dosyaları (.cub)
├── libft/             # Yardımcı fonksiyon kütüphanesi
├── minilibx-linux/    # MLX grafik kütüphanesi
├── Cub3D.h           # Ana header dosyası
└── Makefile          # Derleme konfigürasyonu
```

### Derleme Akışı
1. **libft** derlenır (yardımcı fonksiyonlar)
2. **minilibx** derlenir (grafik kütüphanesi)
3. Kaynak dosyalar object dosyalarına derlenir
4. Tüm object dosyaları birleştirilerek executable oluşturulur

---

## Veri Yapıları

### t_game - Ana Oyun Yapısı
```c
typedef struct s_game
{
    void        *mlx;        // MLX bağlantı pointer'ı
    void        *win;        // Pencere pointer'ı
    t_img       *screen;     // Ekran buffer'ı
    t_map       *map;        // Harita verisi
    t_player    *player;     // Oyuncu bilgisi
    t_texture   *texture;    // Texture'lar
    t_ray       *ray;        // Raycasting verisi
    t_keys      *keys;       // Klavye durumu
    int         running;     // Oyun durumu
}   t_game;
```

**Açıklama**: Tüm oyun bileşenlerini bir araya getiren merkezi yapı. MLX bağlantısı, pencere, tüm oyun nesneleri ve durumları burada tutulur.

### t_player - Oyuncu Yapısı
```c
typedef struct s_player
{
    double  pos_x;      // X koordinatı (map üzerinde)
    double  pos_y;      // Y koordinatı (map üzerinde)
    double  dir_x;      // Bakış yönü X bileşeni
    double  dir_y;      // Bakış yönü Y bileşeni
    double  plane_x;    // Kamera düzlemi X bileşeni
    double  plane_y;    // Kamera düzlemi Y bileşeni
    char    start_dir;  // Başlangıç yönü (N/S/E/W)
}   t_player;
```

**Önemli Noktalar**:
- `pos_x`, `pos_y`: Oyuncunun harita üzerindeki gerçek pozisyonu (double precision)
- `dir_x`, `dir_y`: Normalize edilmiş yön vektörü (bakış yönü)
- `plane_x`, `plane_y`: Kamera düzlemi - FOV (Field of View) için kritik
- Plane vektörü dir vektörüne dik olmalıdır
- Plane uzunluğu FOV'u belirler (genelde 0.66 = ~66° FOV)

### t_ray - Raycasting Yapısı
```c
typedef struct s_ray
{
    double  camera_x;       // Kamera düzleminde X pozisyonu [-1, 1]
    double  ray_dir_x;      // Ray yön vektörü X
    double  ray_dir_y;      // Ray yön vektörü Y
    int     map_x;          // Mevcut map X koordinatı
    int     map_y;          // Mevcut map Y koordinatı
    double  side_dist_x;    // Bir sonraki X kenarına mesafe
    double  side_dist_y;    // Bir sonraki Y kenarına mesafe
    double  delta_dist_x;   // X ekseni boyunca 1 grid mesafesi
    double  delta_dist_y;   // Y ekseni boyunca 1 grid mesafesi
    double  perp_wall_dist; // Duvara perpendicular mesafe
    int     step_x;         // X yönünde adım (+1 veya -1)
    int     step_y;         // Y yönünde adım (+1 veya -1)
    int     hit;            // Duvar çarpma flag'i
    int     side;           // 0=X tarafı, 1=Y tarafı
    int     line_height;    // Ekranda çizilecek çizgi yüksekliği
    int     draw_start;     // Çizginin başlangıç Y koordinatı
    int     draw_end;       // Çizginin bitiş Y koordinatı
    int     tex_num;        // Texture numarası
    double  wall_x;         // Duvarda tam çarpma noktası
    int     tex_x;          // Texture X koordinatı
    int     tex_y;          // Texture Y koordinatı
    double  step;           // Texture step değeri
    double  tex_pos;        // Texture pozisyonu
}   t_ray;
```

**DDA (Digital Differential Analysis) için Kritik Değişkenler**:
- `delta_dist_x/y`: Ray'in bir grid hücresini geçmesi için gereken mesafe
- `side_dist_x/y`: Ray'in başlangıcından bir sonraki grid kenarına mesafe
- `step_x/y`: Grid üzerinde hareket yönü

### t_map - Harita Yapısı
```c
typedef struct s_map
{
    char    **grid;         // 2D harita array'i
    int     width;          // Harita genişliği
    int     height;         // Harita yüksekliği
    int     floor_color;    // Zemin rengi (RGB)
    int     ceiling_color;  // Tavan rengi (RGB)
    int     is_valid;       // Validasyon flag'i
}   t_map;
```

**Grid Karakterleri**:
- `'0'`: Boş alan (gezilebilir)
- `'1'`: Duvar
- `'N'/'S'/'E'/'W'`: Oyuncu başlangıç pozisyonu ve yönü
- `' '`: Boşluk (map dışı)

### t_texture - Texture Yapısı
```c
typedef struct s_texture
{
    t_img   *north;         // Kuzey duvarı texture'ı
    t_img   *south;         // Güney duvarı texture'ı
    t_img   *east;          // Doğu duvarı texture'ı
    t_img   *west;          // Batı duvarı texture'ı
    char    *north_path;    // Kuzey texture yolu
    char    *south_path;    // Güney texture yolu
    char    *east_path;     // Doğu texture yolu
    char    *west_path;     // Batı texture yolu
}   t_texture;
```

### t_img - Image Buffer Yapısı
```c
typedef struct s_img
{
    void    *img;               // MLX image pointer'ı
    char    *addr;              // Pixel buffer adresi
    int     bits_per_pixel;     // Pixel başına bit sayısı
    int     line_length;        // Satır byte uzunluğu
    int     endian;             // Endianness
    int     width;              // Genişlik (piksel)
    int     height;             // Yükseklik (piksel)
}   t_img;
```

**Memory Layout**: MLX image'ları doğrudan bellek erişimi için `addr` pointer'ı sağlar. Bu, piksel manipülasyonunu hızlandırır.

---

## MLX Kütüphanesi

### MLX Nedir?
MiniLibX (MLX), 42 School tarafından geliştirilen basit bir grafik kütüphanesidir. X11 (Linux) veya Cocoa (MacOS) üzerine inşa edilmiştir ve temel grafik operasyonları sağlar.

### Temel MLX Fonksiyonları

#### 1. mlx_init()
```c
void *mlx_init(void);
```
**Açıklama**: MLX bağlantısını başlatır ve grafik sunucuya bağlanır.
**Dönüş**: MLX instance pointer'ı (NULL ise hata)
**Kullanım**:
```c
game->mlx = mlx_init();
if (!game->mlx)
    return (0);  // Hata
```

#### 2. mlx_new_window()
```c
void *mlx_new_window(void *mlx_ptr, int size_x, int size_y, char *title);
```
**Açıklama**: Yeni bir pencere oluşturur.
**Parametreler**:
- `mlx_ptr`: MLX instance
- `size_x`: Pencere genişliği
- `size_y`: Pencere yüksekliği
- `title`: Pencere başlığı

**Kullanım**:
```c
game->win = mlx_new_window(game->mlx, 1280, 720, "Cub3D");
```

#### 3. mlx_new_image()
```c
void *mlx_new_image(void *mlx_ptr, int width, int height);
```
**Açıklama**: Yeni bir image buffer oluşturur. Bu buffer'a piksel yazılabilir.
**Önemli**: Ekrana yazmadan önce buffer'a yazıp sonra buffer'ı ekrana kopyalamak (double buffering) flicker'ı önler.

#### 4. mlx_get_data_addr()
```c
char *mlx_get_data_addr(void *img_ptr, int *bits_per_pixel, 
                        int *line_length, int *endian);
```
**Açıklama**: Image buffer'ının bellek adresini döndürür. Bu adres üzerinden doğrudan piksel manipülasyonu yapılabilir.

**Piksel Yazma Formülü**:
```c
int offset = y * line_length + x * (bits_per_pixel / 8);
*(unsigned int *)(addr + offset) = color;
```

#### 5. mlx_xpm_file_to_image()
```c
void *mlx_xpm_file_to_image(void *mlx_ptr, char *filename, 
                            int *width, int *height);
```
**Açıklama**: XPM formatındaki texture dosyasını yükler ve image buffer'ı döndürür.
**XPM Format**: Basit ASCII tabanlı image formatı, kolay parse edilir.

#### 6. mlx_put_image_to_window()
```c
int mlx_put_image_to_window(void *mlx_ptr, void *win_ptr, 
                            void *img_ptr, int x, int y);
```
**Açıklama**: Image buffer'ını pencereye kopyalar. Render loop'unun son adımı.

#### 7. mlx_loop_hook()
```c
int mlx_loop_hook(void *mlx_ptr, int (*funct_ptr)(), void *param);
```
**Açıklama**: Her frame'de çağrılacak fonksiyonu register eder. Game loop için kritik.

**Kullanım**:
```c
mlx_loop_hook(game->mlx, loop_hook, game);
```

#### 8. mlx_hook()
```c
int mlx_hook(void *win_ptr, int x_event, int x_mask, 
             int (*funct)(), void *param);
```
**Açıklama**: Event handler register eder.

**Önemli Event'ler**:
- `2`: KeyPress (tuşa basma)
- `3`: KeyRelease (tuşu bırakma)
- `17`: DestroyNotify (pencere kapatma)

**Kullanım**:
```c
mlx_hook(game->win, 2, 1L << 0, key_press, game);      // KeyPress
mlx_hook(game->win, 3, 1L << 1, key_release, game);    // KeyRelease
mlx_hook(game->win, 17, 0, close_game, game);          // Close window
```

#### 9. mlx_loop()
```c
int mlx_loop(void *mlx_ptr);
```
**Açıklama**: Ana event loop'u başlatır. Bu fonksiyon çağrıldıktan sonra program event-driven modda çalışır.

### MLX Pixel Manipulation

#### Renk Formatı
MLX, TRGB (Transparency-Red-Green-Blue) formatı kullanır:
```
0xTTRRGGBB
```
- `TT`: Transparency (genelde 0x00)
- `RR`: Red (0-255)
- `GG`: Green (0-255)
- `BB`: Blue (0-255)

**Renk Oluşturma**:
```c
int color = (r << 16) | (g << 8) | b;
```

#### Hızlı Pixel Yazma
```c
void mlx_put_pixel(t_img *img, int x, int y, int color)
{
    char *dst;
    
    if (x < 0 || x >= img->width || y < 0 || y >= img->height)
        return;
    dst = img->addr + (y * img->line_length + x * (img->bits_per_pixel / 8));
    *(unsigned int *)dst = color;
}
```

**Neden Hızlı?**
- MLX'in `mlx_pixel_put()` fonksiyonu her seferinde ekrana yazıyor (yavaş)
- Buffer'a yazmak ve sonra `mlx_put_image_to_window()` çağırmak çok daha hızlı

---

## Raycasting Algoritması

### Raycasting Nedir?

Raycasting, 2D haritadan 3D perspektif görünümü oluşturmak için kullanılan bir rendering tekniğidir. Temel prensip:

1. Oyuncunun her piksel kolonu için bir ışın (ray) gönder
2. Bu ışın ilk hangi duvara çarpıyor bul
3. Mesafeye göre duvarı çiz (uzak = küçük, yakın = büyük)
4. Duvara texture uygula

### Raycasting vs Raytracing

**Raycasting (Bizim kullandığımız)**:
- Sadece yatay düzlemde çalışır
- Yansıma, gölge yok
- Çok hızlı (gerçek zamanlı)
- 2.5D (pseudo-3D)

**Raytracing**:
- 3D uzayda çalışır
- Yansıma, kırılma, gölge var
- Yavaş (film render için)
- Gerçek 3D

### Algoritma Adımları

#### Adım 1: Ray Direction Hesaplama

Her ekran kolonu için ray yönü hesaplanır:

```c
void cast_ray(t_game *game, int x)
{
    t_ray *ray = game->ray;
    
    // Kamera düzleminde X pozisyonu [-1, 1] aralığında
    ray->camera_x = 2 * x / (double)WINDOW_WIDTH - 1;
    
    // Ray yönü = bakış yönü + (kamera düzlemi * camera_x)
    ray->ray_dir_x = game->player->dir_x + game->player->plane_x * ray->camera_x;
    ray->ray_dir_y = game->player->dir_y + game->player->plane_y * ray->camera_x;
}
```

**Matematiksel Açıklama**:
- Sol kenar (x=0): `camera_x = -1` → Ray sola bakar
- Orta (x=WIDTH/2): `camera_x = 0` → Ray düz bakar
- Sağ kenar (x=WIDTH): `camera_x = 1` → Ray sağa bakar

#### Adım 2: DDA Initialization

DDA (Digital Differential Analysis) için başlangıç değerleri:

```c
// Grid üzerinde başlangıç pozisyonu
ray->map_x = (int)game->player->pos_x;
ray->map_y = (int)game->player->pos_y;

// Delta distance: 1 grid birim geçmek için gereken ray mesafesi
ray->delta_dist_x = fabs(1 / ray->ray_dir_x);
ray->delta_dist_y = fabs(1 / ray->ray_dir_y);
```

**Delta Distance Açıklaması**:
- Ray eğer X eksenine paralel ise, Y'de 1 birim ilerlemek için sonsuz mesafe gerekir
- Ray eğer 45° açıda ise, her eksende 1 birim = √2 mesafe
- `delta_dist` bu oranı hesaplar

#### Adım 3: Step ve Side Distance

Ray hangi yöne ilerleyecek ve ilk grid kenarına ne kadar uzaklıkta?

```c
if (ray->ray_dir_x < 0)
{
    ray->step_x = -1;  // Sola git
    ray->side_dist_x = (game->player->pos_x - ray->map_x) * ray->delta_dist_x;
}
else
{
    ray->step_x = 1;   // Sağa git
    ray->side_dist_x = (ray->map_x + 1.0 - game->player->pos_x) * ray->delta_dist_x;
}
// Y için aynı
```

**Side Distance**: Ray'in mevcut pozisyonundan bir sonraki grid kenarına olan mesafe.

#### Adım 4: DDA Algorithm (Duvar Bulma)

Grid üzerinde ilerle, duvar bulana kadar:

```c
void perform_dda(t_game *game)
{
    t_ray *ray = game->ray;
    
    ray->hit = 0;
    while (ray->hit == 0)
    {
        // En yakın grid kenarına ilerle
        if (ray->side_dist_x < ray->side_dist_y)
        {
            ray->side_dist_x += ray->delta_dist_x;
            ray->map_x += ray->step_x;
            ray->side = 0;  // X tarafına çarptı
        }
        else
        {
            ray->side_dist_y += ray->delta_dist_y;
            ray->map_y += ray->step_y;
            ray->side = 1;  // Y tarafına çarptı
        }
        
        // Duvar kontrolü
        if (game->map->grid[ray->map_y][ray->map_x] == '1')
            ray->hit = 1;
    }
}
```

**Neden Bu Kadar Hızlı?**
- Her adımda sadece bir eksende ilerleme yapılır
- Floating point karşılaştırma yerine integer grid kullanılır
- Pythagorean theorem gereksiz (delta distance önceden hesaplanmış)

#### Adım 5: Perpendicular Wall Distance

Balıkgözü etkisini (fisheye) önlemek için perpendicular mesafe kullanılır:

```c
if (ray->side == 0)
    ray->perp_wall_dist = (ray->map_x - game->player->pos_x + 
        (1 - ray->step_x) / 2) / ray->ray_dir_x;
else
    ray->perp_wall_dist = (ray->map_y - game->player->pos_y + 
        (1 - ray->step_y) / 2) / ray->ray_dir_y;
```

**Neden Perpendicular?**
- Gerçek ray mesafesi kullanılırsa, ekranın kenarlarında duvarlar yamulur (fisheye)
- Perpendicular mesafe, duvara dik mesafedir
- Bu, düz bir kamera düzlemi illüzyonu yaratır

#### Adım 6: Duvar Yüksekliği Hesaplama

```c
ray->line_height = (int)(WINDOW_HEIGHT / ray->perp_wall_dist);

// Ekranda çizilecek aralık
ray->draw_start = -ray->line_height / 2 + WINDOW_HEIGHT / 2;
if (ray->draw_start < 0)
    ray->draw_start = 0;

ray->draw_end = ray->line_height / 2 + WINDOW_HEIGHT / 2;
if (ray->draw_end >= WINDOW_HEIGHT)
    ray->draw_end = WINDOW_HEIGHT - 1;
```

**Matematiksel İlişki**:
- Mesafe arttıkça, duvar yüksekliği azalır (ters orantı)
- `WINDOW_HEIGHT / distance` formülü perspektif projeksiyonu sağlar

#### Adım 7: Texture Mapping

Duvarda tam olarak nereye çarptık?

```c
// Duvar üzerinde çarpma noktası (0.0 - 1.0 arası)
if (ray->side == 0)
    ray->wall_x = game->player->pos_y + ray->perp_wall_dist * ray->ray_dir_y;
else
    ray->wall_x = game->player->pos_x + ray->perp_wall_dist * ray->ray_dir_x;
ray->wall_x -= floor(ray->wall_x);  // Ondalık kısmı al

// Texture X koordinatı
ray->tex_x = (int)(ray->wall_x * (double)TEX_WIDTH);

// Texture Y için step değeri
ray->step = 1.0 * TEX_HEIGHT / ray->line_height;
ray->tex_pos = (ray->draw_start - WINDOW_HEIGHT / 2 + 
    ray->line_height / 2) * ray->step;
```

**Texture Sampling**:
```c
for (int y = ray->draw_start; y < ray->draw_end; y++)
{
    ray->tex_y = (int)ray->tex_pos & (TEX_HEIGHT - 1);
    ray->tex_pos += ray->step;
    
    int color = get_pixel_color(texture, ray->tex_x, ray->tex_y);
    mlx_put_pixel(game->screen, x, y, color);
}
```

#### Adım 8: Shading (Gölgelendirme)

Y tarafındaki duvarlar (Kuzey/Güney) daha koyu gösterilir:

```c
if (ray->side == 1)
    color = (color >> 1) & 8355711;  // Rengi karart
```

**Bit Manipulation**:
- `>> 1`: Her renk bileşenini 2'ye böl
- `& 8355711`: En yüksek bitleri temizle (0x7F7F7F)

### Optimizasyon Teknikleri

1. **Pre-calculation**: Delta distances hesaplamaları cache'lenir
2. **Integer Grid**: Floating point yerine integer grid kullanımı
3. **Direct Memory Access**: MLX buffer'a doğrudan yazma
4. **Minimal Branching**: Loop içinde az dallanma

---

## Map Parsing ve Validation

### .cub Dosya Formatı

```
NO assets/NORT-WALL.xpm
SO assets/SOUTH-WALL.xpm
WE assets/WEST-WALL.xpm
EA assets/EAST-WALL.xpm
F 220,100,0
C 225,30,0

111111111111111
111111000111111
111110000011111
1111000N0001111
11111111111111
```

**Format Açıklaması**:
- İlk 6 satır: Konfigürasyon (texture yolları ve renkler)
- Boş satırdan sonra: Map grid
- `NO/SO/WE/EA`: Yön texture'ları
- `F`: Floor (zemin) rengi (R,G,B)
- `C`: Ceiling (tavan) rengi (R,G,B)

### Parsing Süreci

#### 1. Config Parsing

```c
int parse_config(t_game *game, char *filename)
{
    int fd = open(filename, O_RDONLY);
    char *line;
    int count = 0;
    
    while ((line = get_next_line(fd)) && count < 6)
    {
        if (line[0] == 'N' && line[1] == 'O')
            parse_texture_line(line, &game->texture->north_path);
        else if (line[0] == 'F')
            parse_color(line, &game->map->floor_color);
        // ... diğer konfigürasyonlar
        
        count++;
        free(line);
    }
    close(fd);
    return (count == 6);  // Tüm configs bulundu mu?
}
```

#### 2. Color Parsing

```c
int parse_color(char *line, int *color)
{
    int r = atoi(line + 2);           // "F 220,100,0" -> 220
    // ',' karakterini bul
    int g = atoi(strchr(line, ',') + 1);  // -> 100
    int b = atoi(strrchr(line, ',') + 1); // -> 0
    
    // Validasyon
    if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255)
        return (0);
    
    *color = (r << 16) | (g << 8) | b;  // TRGB formatına çevir
    return (1);
}
```

#### 3. Map Grid Parsing

**İki Geçişli Algoritma**:

**Geçiş 1: Boyutları Bul**
```c
int count_map_lines(char *filename, int *height, int *width)
{
    int fd = open(filename, O_RDONLY);
    char *line;
    
    *height = 0;
    *width = 0;
    
    while ((line = get_next_line(fd)))
    {
        if (is_map_line(line))  // '1' veya '0' ile başlıyor mu?
        {
            int len = strlen(line);
            if (len > *width)
                *width = len;
            (*height)++;
        }
        free(line);
    }
    close(fd);
    return (1);
}
```

**Geçiş 2: Grid'i Doldur**
```c
int read_map_lines(char *filename, char **grid, int height)
{
    int fd = open(filename, O_RDONLY);
    char *line;
    int i = 0;
    
    while ((line = get_next_line(fd)) && i < height)
    {
        if (is_map_line(line))
        {
            int j = 0;
            while (line[j] && line[j] != '\n')
            {
                grid[i][j] = line[j];
                j++;
            }
            i++;
        }
        free(line);
    }
    close(fd);
    return (1);
}
```

### Validation Süreci

#### 1. Karakter Validasyonu

```c
int validate_map_chars(t_map *map)
{
    int player_count = 0;
    
    for (int i = 0; i < map->height; i++)
    {
        for (int j = 0; j < map->width; j++)
        {
            char c = map->grid[i][j];
            
            // Sadece geçerli karakterler
            if (c != '0' && c != '1' && c != 'N' && c != 'S' && 
                c != 'E' && c != 'W' && c != ' ')
                return (0);
            
            // Oyuncu sayısını say
            if (c == 'N' || c == 'S' || c == 'E' || c == 'W')
                player_count++;
        }
    }
    
    return (player_count == 1);  // Tam olarak 1 oyuncu olmalı
}
```

#### 2. Duvar Kapalılık Kontrolü

Her boş alan ('0') etrafında duvar olmalı:

```c
int check_wall_closed(char **grid, int height, int width)
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (grid[i][j] == '0' || is_player(grid[i][j]))
            {
                // İlk/son satır kontrolü
                if (i == 0 || i == height - 1)
                    return (0);
                
                // Komşu hücre kontrolü
                if (grid[i-1][j] == ' ' || grid[i+1][j] == ' ' ||
                    grid[i][j-1] == ' ' || grid[i][j+1] == ' ')
                    return (0);
            }
        }
    }
    return (1);
}
```

#### 3. Flood Fill Algoritması

Map'in tam olarak kapalı olduğunu garanti etmek için flood fill:

```c
int flood_fill_check(t_map *map)
{
    // Map kopyası oluştur
    char **map_copy = copy_map(map->grid, map->height);
    
    // Oyuncu pozisyonunu bul
    int start_x, start_y;
    find_player_position(map->grid, map->height, &start_x, &start_y);
    
    // Recursive flood fill
    flood_fill_recursive(map_copy, start_x, start_y, map->height, map->width);
    
    // Doldurulan alanın sınırlarını kontrol et
    int result = check_borders_after_fill(map_copy, map->height, map->width);
    
    // Temizle
    free_map_copy(map_copy, map->height);
    
    return (result);
}
```

**Recursive Flood Fill**:
```c
void flood_fill_recursive(char **map, int x, int y, int h, int w)
{
    // Sınır kontrolü
    if (y < 0 || y >= h || x < 0 || x >= w)
        return;
    
    // Duvar veya zaten doldurulmuş
    if (map[y][x] == '1' || map[y][x] == 'F')
        return;
    
    // Boşluk = map sınırına çıkıldı (geçersiz!)
    if (map[y][x] == ' ')
        return;
    
    // Doldur
    map[y][x] = 'F';
    
    // 4 yöne devam et
    flood_fill_recursive(map, x + 1, y, h, w);
    flood_fill_recursive(map, x - 1, y, h, w);
    flood_fill_recursive(map, x, y + 1, h, w);
    flood_fill_recursive(map, x, y - 1, h, w);
}
```

**Neden Flood Fill?**
- Duvar kontrolü bazı edge case'leri kaçırabilir
- Flood fill, oyuncunun erişebileceği her hücreyi işaretler
- Eğer işaretli hücre map dışına/boşluğa komşu ise = geçersiz map

---

## Texture Management

### Texture Loading

```c
t_img *load_texture(void *mlx, char *path)
{
    t_img *texture = malloc(sizeof(t_img));
    
    // XPM dosyasını yükle
    texture->img = mlx_xpm_file_to_image(mlx, path, 
                                        &texture->width, 
                                        &texture->height);
    if (!texture->img)
    {
        free(texture);
        return (NULL);
    }
    
    // Pixel buffer adresini al
    texture->addr = mlx_get_data_addr(texture->img,
                                     &texture->bits_per_pixel,
                                     &texture->line_length,
                                     &texture->endian);
    return (texture);
}
```

### Texture Seçimi (Runtime)

Ray'in hangi duvara çarptığına göre texture seçilir:

```c
t_img *get_wall_texture(t_game *game)
{
    t_ray *ray = game->ray;
    
    if (ray->side == 0)  // X tarafı (Doğu/Batı)
    {
        if (ray->step_x > 0)
            return (game->texture->east);   // Doğu
        else
            return (game->texture->west);   // Batı
    }
    else  // Y tarafı (Kuzey/Güney)
    {
        if (ray->step_y > 0)
            return (game->texture->south);  // Güney
        else
            return (game->texture->north);  // Kuzey
    }
}
```

### Pixel Okuma (Texture'dan)

```c
int get_pixel_color(t_img *img, int x, int y)
{
    char *dst;
    
    // Sınır kontrolü
    if (x < 0 || x >= img->width || y < 0 || y >= img->height)
        return (0);
    
    // Pixel adresini hesapla
    dst = img->addr + (y * img->line_length + 
                       x * (img->bits_per_pixel / 8));
    
    return (*(unsigned int *)dst);
}
```

---

## Hareket Sistemi

### Movement Physics

#### İleri/Geri Hareket

```c
void move_forward(t_game *game)
{
    double new_x = game->player->pos_x + game->player->dir_x * MOVE_SPEED;
    double new_y = game->player->pos_y + game->player->dir_y * MOVE_SPEED;
    
    // X ve Y eksenlerini ayrı kontrol et (smooth wall sliding)
    if (!check_collision(game, new_x, game->player->pos_y))
        game->player->pos_x = new_x;
    
    if (!check_collision(game, game->player->pos_x, new_y))
        game->player->pos_y = new_y;
}
```

**Neden Ayrı Eksen Kontrolü?**
- Diagonal harekette duvara çarpınca kayma efekti
- Örnek: Kuzeye giderken doğu duvarına çarparsanız, kuzey hareketi devam eder

#### Strafe (Sağa/Sola Kayma)

```c
void move_right(t_game *game)
{
    // Plane vektörü dir vektörüne diktir, bu yüzden yan hareket
    double new_x = game->player->pos_x + game->player->plane_x * MOVE_SPEED;
    double new_y = game->player->pos_y + game->player->plane_y * MOVE_SPEED;
    
    if (!check_collision(game, new_x, game->player->pos_y))
        game->player->pos_x = new_x;
    
    if (!check_collision(game, game->player->pos_x, new_y))
        game->player->pos_y = new_y;
}
```

#### Rotasyon (Dönme)

Rotation matrix kullanılır:

```
[cos(θ)  -sin(θ)] [x]   [x']
[sin(θ)   cos(θ)] [y] = [y']
```

```c
void rotate_right(t_game *game)
{
    double old_dir_x = game->player->dir_x;
    
    // Direction vector'ü döndür
    game->player->dir_x = game->player->dir_x * cos(ROT_SPEED) - 
                          game->player->dir_y * sin(ROT_SPEED);
    game->player->dir_y = old_dir_x * sin(ROT_SPEED) + 
                          game->player->dir_y * cos(ROT_SPEED);
    
    // Plane vector'ü de döndür (FOV ile birlikte dönmeli)
    double old_plane_x = game->player->plane_x;
    game->player->plane_x = game->player->plane_x * cos(ROT_SPEED) - 
                            game->player->plane_y * sin(ROT_SPEED);
    game->player->plane_y = old_plane_x * sin(ROT_SPEED) + 
                            game->player->plane_y * cos(ROT_SPEED);
}
```

**ROT_SPEED**: Radyan cinsinden dönüş hızı (örn: 0.03 ≈ 1.7°)

### Collision Detection

```c
int check_collision(t_game *game, double new_x, double new_y)
{
    int map_x = (int)new_x;
    int map_y = (int)new_y;
    
    // Map sınırları
    if (map_x < 0 || map_x >= game->map->width || 
        map_y < 0 || map_y >= game->map->height)
        return (1);  // Çarpışma var
    
    // Duvar kontrolü
    if (game->map->grid[map_y][map_x] == '1')
        return (1);  // Çarpışma var
    
    return (0);  // Çarpışma yok
}
```

**Basit ama Etkili**: Double precision pozisyon, ama collision için integer grid kullanımı.

---

## Fonksiyon Referansı

### Main Flow

#### main()
```c
int main(int argc, char **argv)
```
**Açıklama**: Program giriş noktası
**Akış**:
1. Argüman kontrolü (2 olmalı)
2. `.cub` uzantı kontrolü
3. Map parsing
4. Oyun başlatma
5. Event hook'ları register etme
6. MLX loop başlatma

#### init_game()
```c
int init_game(t_game *game)
```
**Açıklama**: Oyun bileşenlerini başlatır
**Yapar**:
- MLX bağlantısı ve pencere oluşturma
- Screen buffer oluşturma
- Struct'ları allocate etme
- Texture'ları yükleme

### Rendering

#### loop_hook()
```c
int loop_hook(t_game *game)
```
**Açıklama**: Her frame çağrılır (game loop)
**Yapar**:
1. Hareket işleme (`process_movement`)
2. Frame render etme (`render_frame`)

#### render_frame()
```c
int render_frame(t_game *game)
```
**Açıklama**: Tek frame render eder
**Akış**:
1. Tavan/zemin renkleri doldur
2. Her ekran kolonu için raycasting yap
3. Buffer'ı ekrana kopyala

#### raycasting()
```c
static void raycasting(t_game *game)
```
**Açıklama**: Tüm ekran için raycasting loop
**İçerik**:
```c
for (int x = 0; x < WINDOW_WIDTH; x++)
{
    cast_ray(game, x);       // Ray yönü hesapla
    perform_dda(game);       // Duvar bul
    draw_textured_wall(game, x);  // Texture'lı duvar çiz
}
```

#### cast_ray()
```c
static void cast_ray(t_game *game, int x)
```
**Açıklama**: Tek ray için başlangıç hesaplamaları
**Hesaplar**:
- Camera X pozisyonu
- Ray yönü
- Delta distances
- Initial side distances

#### perform_dda()
```c
static void perform_dda(t_game *game)
```
**Açıklama**: DDA algoritması ile duvar bulma
**Algoritma**:
1. Side distance'ları karşılaştır
2. Daha küçük olana doğru adım at
3. Duvar bulunana kadar tekrarla

### Movement

#### process_movement()
```c
void process_movement(t_game *game)
```
**Açıklama**: Tüm aktif tuşları işler
**Kontrol Eder**:
- W/S: İleri/geri
- A/D: Sol/sağ strafe
- Yön tuşları: Dönme

#### move_forward() / move_backward()
```c
void move_forward(t_game *game)
void move_backward(t_game *game)
```
**Açıklama**: Bakış yönünde hareket
**Formül**: `new_pos = pos + dir * MOVE_SPEED`

#### move_left() / move_right()
```c
void move_left(t_game *game)
void move_right(t_game *game)
```
**Açıklama**: Yan hareket (strafe)
**Formül**: `new_pos = pos ± plane * MOVE_SPEED`

#### rotate_left() / rotate_right()
```c
void rotate_left(t_game *game)
void rotate_right(t_game *game)
```
**Açıklama**: Kamera rotasyonu
**Method**: 2D rotation matrix

#### check_collision()
```c
static int check_collision(t_game *game, double new_x, double new_y)
```
**Açıklama**: Yeni pozisyonda çarpışma kontrolü
**Döner**: 1 = çarpışma var, 0 = yol açık

### Event Handling

#### key_press()
```c
int key_press(int keycode, t_game *game)
```
**Açıklama**: Tuşa basma event'i
**Yapar**: İlgili tuş flag'ini 1 yapar

#### key_release()
```c
int key_release(int keycode, t_game *game)
```
**Açıklama**: Tuş bırakma event'i
**Yapar**: İlgili tuş flag'ini 0 yapar

#### close_game()
```c
int close_game(t_game *game)
```
**Açıklama**: Pencere kapatma/ESC event'i
**Yapar**:
1. Tüm belleği temizle
2. MLX kaynakları free et
3. Program sonlandır

### Map Operations

#### parse_map()
```c
int parse_map(t_game *game, char *filename)
```
**Açıklama**: Ana map parsing fonksiyonu
**Akış**:
1. Config parse
2. Map boyutları hesapla
3. Grid allocate et
4. Grid doldur
5. Validasyon yap

#### parse_config()
```c
int parse_config(t_game *game, char *filename)
```
**Açıklama**: Texture ve renk konfigürasyonu parse
**Okur**: NO, SO, WE, EA, F, C satırları

#### validate_and_init_map()
```c
int validate_and_init_map(t_game *game)
```
**Açıklama**: Tüm validasyon kontrolleri
**Kontroller**:
1. Karakter validasyonu
2. Duvar kapalılık
3. Flood fill
4. Oyuncu pozisyonu

#### flood_fill_check()
```c
int flood_fill_check(t_map *map)
```
**Açıklama**: Map'in tam kapalı olduğunu garanti eder
**Method**: Recursive flood fill

### Texture Operations

#### load_textures()
```c
int load_textures(t_game *game)
```
**Açıklama**: Tüm texture'ları yükler
**Yükler**: 4 yön texture'ı (N, S, E, W)

#### get_wall_texture()
```c
static t_img *get_wall_texture(t_game *game)
```
**Açıklama**: Ray'in çarptığı duvara göre texture seçer
**Seçim**: Ray side ve step yönüne göre

### Utility Functions

#### mlx_put_pixel()
```c
void mlx_put_pixel(t_img *img, int x, int y, int color)
```
**Açıklama**: Image buffer'a piksel yazar
**Hızlı**: Doğrudan bellek erişimi

#### get_pixel_color()
```c
int get_pixel_color(t_img *img, int x, int y)
```
**Açıklama**: Texture'dan renk okur
**Döner**: TRGB formatında renk

#### get_next_line()
```c
char *get_next_line(int fd)
```
**Açıklama**: File descriptor'dan satır okur
**Özellik**: Static buffer ile birden fazla dosya destekler

#### free_game()
```c
void free_game(t_game *game)
```
**Açıklama**: Tüm allocate edilmiş belleği temizler
**Temizler**:
- Map grid
- Texture'lar
- MLX kaynakları
- Tüm struct'lar

---

## Performans ve Optimizasyon

### Frame Rate Optimizasyonu

**Mevcut Yöntemler**:
1. **Direct Memory Access**: `mlx_pixel_put()` yerine buffer yazma
2. **Pre-calculation**: Delta distances tek sefer hesaplanır
3. **Integer Grid**: Floating point yerine integer karşılaştırma
4. **Minimal Function Calls**: Loop içinde az fonksiyon çağrısı

**Potansiyel İyileştirmeler**:
- Multi-threading (her kolonu ayrı thread)
- SIMD instructions (AVX2)
- Texture caching
- Dirty rectangle rendering (sadece değişen alanlar)

### Bellek Kullanımı

**Heap Allocations**:
- Map grid: `width * height * sizeof(char)`
- Texture'lar: `4 * 64 * 64 * 4 bytes = 64 KB`
- Screen buffer: `1280 * 720 * 4 bytes ≈ 3.6 MB`

**Stack Usage**:
- Ray struct: ~200 bytes
- Player struct: ~50 bytes

**Toplam**: ~4 MB (çok verimli!)

---

## Hata Yönetimi

### Yaygın Hatalar ve Çözümleri

#### 1. Segmentation Fault
**Sebep**: NULL pointer dereference
**Çözüm**: Her malloc sonrası NULL kontrolü
```c
game->map = malloc(sizeof(t_map));
if (!game->map)
    return (error_exit("Memory allocation failed"));
```

#### 2. Fisheye Effect
**Sebep**: Euclidean distance kullanımı
**Çözüm**: Perpendicular distance hesaplama
```c
perp_wall_dist = (map_x - pos_x + (1 - step_x) / 2) / ray_dir_x;
```

#### 3. Texture Smearing
**Sebep**: Yanlış texture step hesabı
**Çözüm**: Doğru step ve tex_pos hesaplama
```c
step = 1.0 * TEX_HEIGHT / line_height;
tex_pos = (draw_start - WINDOW_HEIGHT / 2 + line_height / 2) * step;
```

#### 4. Map Validation Fails
**Sebep**: Windows line endings (`\r\n`)
**Çözüm**: `dos2unix` ile dosya dönüşümü
```bash
dos2unix maps/*.cub
```

---

## Matematiksel Referans

### Vektör İşlemleri

**Normalizasyon**:
```
v_normalized = v / |v|
|v| = sqrt(v_x² + v_y²)
```

**Dot Product**:
```
a · b = a_x * b_x + a_y * b_y
```

**Perpendicular Vector**:
```
v_perp = (-v_y, v_x)  // Saat yönü tersine 90°
```

### Rotation Matrix

2D rotasyon (saat yönü tersine):
```
[x']   [cos(θ)  -sin(θ)] [x]
[y'] = [sin(θ)   cos(θ)] [y]
```

### Projeksiyon Formülü

Perspektif projeksiyonu:
```
screen_height = (real_height * screen_distance) / object_distance
```

Bizim durumumuzda:
```
line_height = WINDOW_HEIGHT / perp_wall_dist
```

---

## Kaynaklar ve Referanslar

### Öğrenme Kaynakları

1. **Lode's Computer Graphics Tutorial**
   - https://lodev.org/cgtutor/raycasting.html
   - En kapsamlı raycasting rehberi

2. **Permadi's Ray-Casting Tutorial**
   - https://permadi.com/1996/05/ray-casting-tutorial-table-of-contents/
   - Detaylı matematiksel açıklamalar

3. **42 Docs - MiniLibX**
   - MLX fonksiyonları ve kullanım örnekleri

### İlgili Algoritmalar

- **Bresenham's Line Algorithm**: Çizgi çizimi
- **DDA (Digital Differential Analysis)**: Grid traversal
- **Flood Fill**: Map validasyon
- **Rotation Matrix**: Kamera rotasyonu

### Tarihsel Bağlam

- **Wolfenstein 3D (1992)**: İlk raycasting oyunu
- **DOOM (1993)**: Binary space partitioning (BSP)
- **Quake (1996)**: Gerçek 3D rendering

---

## Sonuç

CUB3D projesi, bilgisayar grafiklerinin temellerini öğrenmek için mükemmel bir örnektir. Raycasting algoritması, basitliği ve performansıyla modern oyun motorlarının kökenini temsil eder.

### Öğrenilen Temel Kavramlar

1. **Raycasting**: 2D'den 3D illüzyonu yaratma
2. **Texture Mapping**: Gerçekçi yüzeyler oluşturma
3. **Event Handling**: Real-time kullanıcı etkileşimi
4. **Optimizasyon**: Performans odaklı programlama
5. **Bellek Yönetimi**: C dilinde güvenli memory handling

### Gelecek Geliştirmeler

Projeyi geliştirmek için fikirler:
- **Sprites**: Düşman ve obje ekleme
- **Multiple Height Levels**: Merdivenler ve platformlar
- **Lighting**: Dinamik ışık ve gölge
- **Minimap**: 2D harita overlay
- **Skybox**: Daha immersive ortam

---

**Not**: Bu dokümantasyon, CUB3D projesinin tüm teknik detaylarını kapsar. Her bölüm, ilgili kavramları derinlemesine açıklar ve kod örnekleriyle destekler.

**Hazırlayan**: AI Assistant
**Tarih**: 26 Kasım 2025
**Versiyon**: 1.0
