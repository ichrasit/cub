cub

aslında arka planda solong gibi 2d bir harita var
ancak biz matematik kullanarak sanki oyuncunun gözünden görüyormuş
gibi fake 3d bir görüntü oluşturuyoruz. 

main:

if (argc != 2)
    uzantı kontrolu
if (check_file_extension(argv[1]))
    dosya .cub ile bitiyormu
ft_bzero(....) game yapısını sıfırla garbage kalmasın

if(parse_map(...)) harita ve konfigurasyon okuma (solongdan karmaşık)

if(init_game(..)) Oyunun görsel kısmını başlatma (mlx)


mlx_loop_hook(....) oyunun sürekli çalışan fonksiyonu
mlx_hook(....) tuşa basınca
mlx_hook(...) tuşu bırakınca
mlx_hook(...) x'e basınca kapama

mlx_loop(..) pencere loop


1: burada neler oluyor? 

Program önce harita dosyasını okuyup hafızaya atıyor, sonra pencereyi açıyor ve mlx_loop() ile sonsuz bir döngüye girip senin tuşlara basmanı veya oyunun bir sonraki frame çizmesini bekliyor


2: Harita Okuma src/map_parser.c & src/config_parser.c: 

so_long'da sadece harita vardı (11111/1OP1) gibi. Burada ise .cub dosyasının içinde önce ayarlar sonrasında ise harita var.

Dosya içeriği şöyledir:
1. texture Yolları: NO./path/north.xpm (kuzey duvarı resmi vb.)
2. Renkler: F 220, 100, 0 zemin rengi, C 225 30 0 tavan rengi
3. Harita: 11100N11 Duvarlar ve oyuncu

* parse_config: Dosyanın başındaki NO, SO, WE, EA, F, C satırlarını okur ve game->texture ve game->map içine kaydeder.

* read_map_lines: Haritanın kendisini 1 ve 0'ları okur ve game->map->grid adında 2 boyutlu bir diziye (char **) kaydeder. Tıpkı so_long'daki gibi

* validate_and_init_map (map_check.c):
    * Harita Kapalı mı?
    * Geçersiz Karakter var mı?
    * Oyuncu (N, S, E, W) nerede? Oyuncunun başlangıç yönüne göre
    (init_player_direction) bakış açısı ayarlanır.

3: Başlatma: src/init_game.c: 

Harita hatasız okunduktan sonra grafik motoru başlar.

* init_mlx: mlx_init() ve mlx_new_window() fonksiyonlarını çağırır ve pencere açılır.
* init_screen: so_long'da resimleri direkt pencereye basıyorduk. CUB'de ise pixel pixel çizim yapacağımıziçin her seferinde pencereye basmak aşırı yavaş olur.
    * Bunun yerine mlx_new_image ile ekran boyutunda boş bir resim (buffer) oluştururuz. 
    * Tüm çizimi bu resmin üzerine yaparız.
    * İşimiz bitince tek seferde (mlx_put_image_to_window) ile bu resmi ekrana basarız. Bu sayede oyun akıcı (fps yüksek) olur.


* load_textures: Harita dosyasında belirtilen xpm dosyalarını (duvar resimlerini) mlx ile yükler.


5. Hareket Sistemi: src/hooks.c ve src/movement.c

so_long'da tuşa basınca key_hook ile anında hareket ettiriyorduk. Cub'de akıcı bir hareket için farklı bir yöntem izlenir:
 
* hooks.c: Tuşa basıldığında (key_press) ilgili değişkeni 1 yapar (örneğin game->keys->w = 1) tuş bırakıldığında (key_release) 0 yapar.

* movement.c -> process_movement : Oyun döngüsünde sürekli bu tuş durumlarını kontrol eder.

    * Eğer w 1 ise move_forward çalışır.
    * move_forward: Oyuncunun koordinatlarını (pos_x, pos_y) baktığı yöne dir_x ve dir_y doğru birazcık arttırır.
    * check_collision: "Gideceğim yer duvar mı?" diye kontrol eder. Duvar değilse yürür.
    * rotate_left/right : Oyuncunun bakış yönü vektörünü matematiksel olarak döndürür (sinüs ve kosinüs ile).


Özetle işleyiş şu şekilde :
1. Program açılır, .cub dosyasını okur, haritayı hafızaya (1, ve 0 matrisi olarak) atar.
2. Grafik arayüzü başlatır ve duvar resimlerini yükler.

3. Sonsuz döngü başlar:
    * Klavye durumuna bak -> Oyuncuyu hareket ettirir.
    * Tavanı ve zemini boya.
    * Ekranın her dikey çizgisi için oyuncudan bir ışın fırlat.
    * Işın duvara çarpınca mesafeyi ölç.
    * Mesafeye göre duvarın yüksekliğini hesapla (Yakınsa büyük, uzaksa küçük).
    * Hesaplanan yüksekliğe göre duvarın resmini (texture) o dikey çizgiye yapıştır.
    * Hazırlanan resmi ekrana bas.




Fonskiyonların yol haritası ve detaylı açıklama:

check_file_extension: İlk güvenlik önlemi. Kullanıcı ./cub3d harita.txt yazarsa reddeder. İlla .cub olmalı

ft_bzero: t_game yapısının içini yamamen sıfırlar

parse_map: (detay aşağıda) Dosyayı okur, Haritayı ve ayarları hafızaya yükler.

init_game: (detay aşağıda) Pencereyi açar, resimleri yükler.

mlx_loop_hook: Bu çok kritik. Oyunun kalp atışıdır. Bilgisayarın işlemcisi ne kadar hızlıysa o kadar sık çağırılan loop_hook fonksiyonunu (kodda src/render.c) kaydeder. Oyunun sürekli ekrana çizilmesini bu sağlar.

mlx_hook: Klavye ve fare olaylarını dinler.
* 2 KeyPress tuşa basınca key_press fonksiyonunu çağırır.
* 3 KeyReleaase: Tuşu bırakınca key_release fonksiyonunu çağırır.
* 17 DestroyNotify: Penceredeki kırmızı çarpıya basınca close_game'i çağırır.

mlx_loop: Programı sonsuz döngüye sokar. Artık program kapanmaz, olayları (tuş basma, ekran yenileme) bekler.


Hazırlık Aşaması: parse_map(harita okuma): 

src/map_parser.c dosyasındadır. Amacı .cub dosyasını anlamlı verilere dönüştürmektir.

1. parse_config(src/config_parser.c): Dosyanın başındaki NO, SO, F, C gibi satırları okur.
    * Neden var?: Oyunun hangi duvar resimlerini ve hangi zemin renklerini kullanacağını bilmesi lazım.

2. count_map_lines: Dosyayı baştan sona okuyup haritanın kaç satır (height) ve en geniş yerinin kaç sütün (width) olduğunu sayar
    * Neden var?: C dilinde dinamik dizi açmak için önce ne kadar yer lazım olduğunu bilmemiz gerekir.

3. allocate_map_grid: Harita için hafızada yer ayırır

4. read_map_lines: Dosyayı tekrar ve haritadaki 1, 0, N gibi karakterleri az önce açtığı hafızaya doldurur.

5. validate_and_init_map: (detay aşağıda) Haritanın kurallara uygun olup olmadığını denetler.



Güvenlik Kontrolü: validate_and_init_map (Map kontrölü):

src/map_check.c dosyasındadır. Harita oynanabilir mi diye bakar.

1. validate_map_chars: Haritada sadece 0, 1, N, S, E, W ve boşluk karakterleri mi var? Başka bir şey varsa hata verir. Ayrıca oyuncu sayısının tam olarak 1 olup olmadığını kontrol eder.

2. check_wall_closed: Haritanın etrafı duvarlarla 1 kapalımı?
    * Neden var?: Eğer harita açık olursa, Raycast ışınları sonsuza kadar gider ve program çöker (segfault). Oyuncu harita dışına çıkamasın diye bu kontrol şarttır.

3. flood_fill_check (src/flood_fill.c): Bu çok zekice bir algoritmadırç Oyuncunun olduğu yerden sanki su dökülmüş gibi her yere yayılır.
    * Neden Var?: Eğer bu "su" haritanın dışına sızabiliyorsa veya space'e ulaşıyorsa, harita delik demektir. Hata verir.

4. set_player_position: Haritayı tarar, N, S, E, veya W harfini bulur.
    * Oyununcunun pos_x ve pos_y değerlerini kaydeder.
    * Oyuncunun baktığı yöne dir_x, dir_y göre kamera açısını ayarlar.
    * Haritadaki o harfi silip yerine 0 yazar ki oyuncu kendi üstüne basıp duvara çarpmış olmasın.


Oyun Döngüsü: loop_hook & render_frame:

Oyun açıldıktan sonra saniyede 60-144 kez (FPS) çalışan kısımdır.

1. process_movement(src/movement.c):

    * O an hangi tuşlara basılı (game->keys->w vb.) diye bakar.
    * Eğer W basılıysa move_forward çağrılır.
    * move_forward: Oyuncunun yeni koordinatlarını hesaplar.
    * check_collision: "Gideceğim yeni koordinatta duvar "1" var mı?" diye bakar. Varsa hareket etmez, yoksa pos_x ve pos_y güncellenir.
    * rotate_left/right: Oyuncunun bakış açısını (vektörünü) matematiksel olarak döndürülür.

2. render_frame (src/render.c)
    * draw_ceiling_floor: Önce ekranı temizler. Üst yarıyı tavan rengine, alt yarıyı zemin rengine boyar.
    * raycasting : Burası sihrin olduğu yer.
    * Her dikey sütun 0'dan 1280'e kadar bir ışın atar.
    * Duvarın uzaklığını bulur.
    * Uzaklığa göre duvarın ne kadar yüksek çizileceğini hesaplar.
    * draw_textured_wall: O duvarın üzerine, init_game'de yüklediğimiz resimden doğru parçayı alıp yapıştırır.

    * mlx_put_image_to_window: Hazırlanan o "screen" resmini ekrana basar. Biz de oyunu görürüz.

