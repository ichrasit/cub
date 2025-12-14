#!/bin/bash

# ==============================================================================
#                 CUB3D ULTIMATE DEBUGGER & STRESS TESTER v3.0
# ==============================================================================
# Özellikler:
# - Otomatik Harita Üretimi (Python)
# - Detaylı Hata Raporlama (error_report.txt)
# - Valgrind Entegrasyonu
# - Segfault Yakalama
# ==============================================================================

# --- AYARLAR ---
PROG="./cub3D"
OUT_DIR="test_artifacts"
REPORT_FILE="error_report.txt"
VALGRIND_LOG="valgrind.log"
NUM_RANDOM_TESTS=500
TIMEOUT_DUR="0.5s"

# --- RENKLER ---
R="\033[31m" # Kırmızı
G="\033[32m" # Yeşil
Y="\033[33m" # Sarı
B="\033[34m" # Mavi
M="\033[35m" # Magenta
C="\033[36m" # Cyan
X="\033[0m"  # Reset

# --- SAYAÇLAR ---
TOTAL=0
PASS=0
FAIL=0
SEGFAULTS=0
LEAKS=0

# --- EKRAN TEMİZLİĞİ ---
clear
rm -rf $OUT_DIR
mkdir -p $OUT_DIR
# Rapor dosyasını sıfırla ve başlık at
echo "======================================================================" > $REPORT_FILE
echo "                  CUB3D HATA ANALİZ RAPORU - $(date)" >> $REPORT_FILE
echo "======================================================================" >> $REPORT_FILE
echo "" >> $REPORT_FILE

echo -e "${C}"
echo "   ___________________________________________________________   "
echo "  |                                                           |  "
echo "  |   C U B 3 D   P R O   T E S T E R   -   D E B U G G E R   |  "
echo "  |___________________________________________________________|  "
echo "                                                                 "
echo -e "${X}"

# ==============================================================================
# [FIX] GEÇERLİ TEXTURE OLUŞTURMA
# ==============================================================================
cat << 'EOF_XPM' > $OUT_DIR/valid.xpm
/* XPM */
static char *dummy[] = {
"1 1 1 1",
"  c #FF0000",
" "
};
EOF_XPM

# ==============================================================================
# [PYTHON] HARİTA OLUŞTURUCU
# ==============================================================================
cat << 'EOF_PY' > $OUT_DIR/generator.py
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
EOF_PY

# ==============================================================================
# RAPORLAMA FONKSİYONU
# ==============================================================================
log_failure() {
    local NAME=$1
    local MAP=$2
    local OUT=$3
    local EXP=$4
    local REASON=$5
    
    echo "----------------------------------------------------------------------" >> $REPORT_FILE
    echo "TEST ID    : $TOTAL" >> $REPORT_FILE
    echo "SENARYO    : $NAME" >> $REPORT_FILE
    echo "DURUM      : BAŞARISIZ ($REASON)" >> $REPORT_FILE
    echo "BEKLENEN   : $EXP" >> $REPORT_FILE
    echo "" >> $REPORT_FILE
    echo "--- PROGRAM ÇIKTISI (STDOUT/STDERR) ---" >> $REPORT_FILE
    echo "$OUT" >> $REPORT_FILE
    echo "" >> $REPORT_FILE
    
    if [ -f "$MAP" ]; then
        echo "--- KULLANILAN HARİTA (İlk 20 Satır) ---" >> $REPORT_FILE
        head -n 20 "$MAP" >> $REPORT_FILE
        echo "[...]" >> $REPORT_FILE
    fi
    
    if [ "$REASON" == "MEMORY LEAK" ] && [ -f "$VALGRIND_LOG" ]; then
        echo "" >> $REPORT_FILE
        echo "--- VALGRIND RAPORU (ÖZET) ---" >> $REPORT_FILE
        grep -A 5 "definitely lost" "$VALGRIND_LOG" >> $REPORT_FILE
        grep -A 5 "Invalid read" "$VALGRIND_LOG" >> $REPORT_FILE
    fi
    echo "" >> $REPORT_FILE
}

# ==============================================================================
# TEST MOTORU
# ==============================================================================
run_test() {
    local DESC=$1
    local MAP_FILE=$2
    local MODE=$3 # 0: Başarı Bekle, 1: Hata Bekle
    local CHECK_MEM=$4

    ((TOTAL++))
    printf "%-50s" "[TEST $TOTAL] $DESC"

    # Programı Çalıştır
    if [ "$CHECK_MEM" -eq 1 ]; then
        OUTPUT=$(timeout 3s valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file=$VALGRIND_LOG $PROG $MAP_FILE 2>&1)
        RET=$?
    else
        OUTPUT=$(timeout $TIMEOUT_DUR $PROG $MAP_FILE 2>&1)
        RET=$?
    fi

    # Analiz
    HAS_ERROR=$(echo "$OUTPUT" | grep -i "Error" | wc -l)
    if [ "$RET" -eq 124 ]; then RET=0; fi # Timeout = Success (Loop çalışıyor)

    # 1. SEGFAULT
    if [ "$RET" -eq 139 ]; then
        echo -e "${R}[SEGFAULT]${X}"
        log_failure "$DESC" "$MAP_FILE" "$OUTPUT" "Programın çökmemesi" "SEGMENTATION FAULT"
        ((SEGFAULTS++))
        ((FAIL++))
        return
    fi

    # 2. SENARYO ANALİZİ
    if [ "$MODE" -eq 1 ]; then
        # Hata Bekleniyor (Invalid)
        if [ "$HAS_ERROR" -gt 0 ] || [ "$RET" -ne 0 ]; then
            echo -e "${G}[OK]${X}"
            ((PASS++))
        else
            echo -e "${R}[FAIL]${X}"
            log_failure "$DESC" "$MAP_FILE" "$OUTPUT" "Error mesajı ve çıkış" "Program hatayı yakalamadı, çalışmaya devam etti"
            ((FAIL++))
        fi
    else
        # Başarı Bekleniyor (Valid)
        if [ "$RET" -eq 0 ] && [ "$HAS_ERROR" -eq 0 ]; then
            if [ "$CHECK_MEM" -eq 1 ]; then
                LEAKS_FOUND=$(grep "definitely lost:" $VALGRIND_LOG | grep -v "0 bytes in 0 blocks" | wc -l)
                if [ "$LEAKS_FOUND" -gt 0 ]; then
                    echo -e "${Y}[LEAK]${X}"
                    log_failure "$DESC" "$MAP_FILE" "$OUTPUT" "0 Byte Kayıp" "MEMORY LEAK"
                    ((LEAKS++))
                else
                    echo -e "${G}[OK]${X}"
                    ((PASS++))
                fi
            else
                echo -e "${G}[OK]${X}"
                ((PASS++))
            fi
        else
            echo -e "${R}[FAIL]${X}"
            log_failure "$DESC" "$MAP_FILE" "$OUTPUT" "Oyunun açılması" "Program hata verdi veya kapandı"
            ((FAIL++))
        fi
    fi
}

# ==============================================================================
# [BÖLÜM 1] MANUEL EDGE CASELER
# ==============================================================================
echo -e "\n${B}--- [PHASE 1] PARSING KONTROLLERİ ---${X}"

# 1. Çift Virgül
echo -e "NO $OUT_DIR/valid.xpm\nSO $OUT_DIR/valid.xpm\nWE $OUT_DIR/valid.xpm\nEA $OUT_DIR/valid.xpm\nF 0,,0,0\nC 0,0,0\n111\n1N1\n111" > $OUT_DIR/comma.cub
run_test "Renklerde çift virgül" "$OUT_DIR/comma.cub" 1 0

# 2. Tab Karakteri
echo -e "NO $OUT_DIR/valid.xpm\nSO $OUT_DIR/valid.xpm\nWE $OUT_DIR/valid.xpm\nEA $OUT_DIR/valid.xpm\nF 0,0,0\nC 0,0,0\n111\n1\tN1\n111" > $OUT_DIR/tab.cub
run_test "Harita içinde TAB karakteri" "$OUT_DIR/tab.cub" 1 0

# 3. Eksik RGB
echo -e "NO $OUT_DIR/valid.xpm\nSO $OUT_DIR/valid.xpm\nWE $OUT_DIR/valid.xpm\nEA $OUT_DIR/valid.xpm\nF 0,0\nC 0,0,0\n111\n1N1\n111" > $OUT_DIR/missing_rgb.cub
run_test "Eksik RGB Değeri" "$OUT_DIR/missing_rgb.cub" 1 0

# 4. RGB > 255
echo -e "NO $OUT_DIR/valid.xpm\nSO $OUT_DIR/valid.xpm\nWE $OUT_DIR/valid.xpm\nEA $OUT_DIR/valid.xpm\nF 300,0,0\nC 0,0,0\n111\n1N1\n111" > $OUT_DIR/overflow.cub
run_test "RGB Değeri 255 Üstü" "$OUT_DIR/overflow.cub" 1 0

# 5. Haritada Boş Satır
echo -e "NO $OUT_DIR/valid.xpm\nSO $OUT_DIR/valid.xpm\nWE $OUT_DIR/valid.xpm\nEA $OUT_DIR/valid.xpm\nF 0,0,0\nC 0,0,0\n111\n1N1\n\n111" > $OUT_DIR/empty_line.cub
run_test "Harita Ortasında Boş Satır" "$OUT_DIR/empty_line.cub" 1 0

# ==============================================================================
# [BÖLÜM 2] OTOMATİK FUZZING
# ==============================================================================
echo -e "\n${B}--- [PHASE 2] RASTGELE SENARYOLAR ($NUM_RANDOM_TESTS Adet) ---${X}"

for ((i=1; i<=$NUM_RANDOM_TESTS; i++)); do
    MAP_PATH="$OUT_DIR/random_$i.cub"
    R=$((RANDOM % 10))
    
    if [ $R -lt 3 ]; then
        # VALID MAP
        python3 $OUT_DIR/generator.py valid $OUT_DIR > $MAP_PATH
        if (( i % 50 == 0 )); then
            run_test "Random Valid Map #$i (Memory Check)" "$MAP_PATH" 0 1
        else
            run_test "Random Valid Map #$i" "$MAP_PATH" 0 0
        fi
    elif [ $R -lt 8 ]; then
        # INVALID MAP
        python3 $OUT_DIR/generator.py invalid $OUT_DIR > $MAP_PATH
        run_test "Random Invalid Map #$i" "$MAP_PATH" 1 0
    else
        # CHAOS (Bozuk Config)
        python3 $OUT_DIR/generator.py chaos $OUT_DIR > $MAP_PATH
        run_test "Chaos Config Map #$i" "$MAP_PATH" 1 0
    fi
done

# ==============================================================================
# [BÖLÜM 3] STRESS TEST
# ==============================================================================
echo -e "\n${B}--- [PHASE 3] PERFORMANS TESTİ ---${X}"
python3 $OUT_DIR/generator.py huge $OUT_DIR > $OUT_DIR/huge.cub
run_test "100x100 Devasa Harita" "$OUT_DIR/huge.cub" 0 0

# ==============================================================================
# BİTİŞ
# ==============================================================================
echo -e "\n======================================================"
echo -e "TOPLAM: $TOTAL | BAŞARILI: $PASS | BAŞARISIZ: $FAIL"
if [ "$FAIL" -gt 0 ]; then
    echo -e "${R}HATALAR BULUNDU! Detaylar için '$REPORT_FILE' dosyasına bak.${X}"
else
    echo -e "${G}TEBRİKLER! Kodun mükemmel çalışıyor.${X}"
fi
echo -e "======================================================"