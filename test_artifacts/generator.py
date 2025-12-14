import random
import sys

def gen_map(mode):
    width, height = 20, 20
    if mode == "huge": width, height = 100, 100
    
    # Duvar çerçevesi
    grid = [['1' for _ in range(width)] for _ in range(height)]
    
    valid = (mode == "valid" or mode == "huge")
    
    # İçerik doldur
    for y in range(1, height-1):
        for x in range(1, width-1):
            if valid:
                grid[y][x] = '0' if random.random() > 0.2 else '1'
            else:
                r = random.random()
                if r < 0.1: grid[y][x] = ' '  # Delik
                elif r < 0.2: grid[y][x] = 'X' # Yasaklı karakter
                elif r < 0.25: grid[y][x] = '\t' # Tab (Yasaklı)
                else: grid[y][x] = '0'

    # Oyuncu Yerleştirme (Güvenli Alan)
    # Valid haritalarda oyuncu duvara bitişik olmasın diye merkeze yakın koyuyoruz
    # Invalid haritalarda rastgele koyuyoruz
    if valid:
        px = random.randint(2, width-3)
        py = random.randint(2, height-3)
        # Oyuncunun etrafını boşalt (Sıkışma önleme)
        for dy in [-1, 0, 1]:
            for dx in [-1, 0, 1]:
                grid[py+dy][px+dx] = '0'
    else:
        px, py = random.randint(1, width-2), random.randint(1, height-2)
    
    grid[py][px] = 'N'

    # Texture Yolları
    print(f"NO {sys.argv[2]}/valid.xpm")
    print(f"SO {sys.argv[2]}/valid.xpm")
    print(f"WE {sys.argv[2]}/valid.xpm")
    print(f"EA {sys.argv[2]}/valid.xpm")
    
    # Renkler (Chaos modunda bozuk renkler olabilir)
    if mode == "chaos":
        if random.random() > 0.5:
            print(f"F {random.randint(0,255)},,0,0") # Çift virgül hatası
        else:
            print(f"F {random.randint(0,300)},0,0") # 255+ hatası
    else:
        print(f"F {random.randint(0,255)},{random.randint(0,255)},{random.randint(0,255)}")
        
    print(f"C {random.randint(0,255)},{random.randint(0,255)},{random.randint(0,255)}")
    print("")
    
    for row in grid:
        print("".join(row))

if __name__ == "__main__":
    try:
        gen_map(sys.argv[1])
    except:
        pass
