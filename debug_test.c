#include "Cub3D.h"
#include <stdio.h>

int main() {
    t_game g;
    ft_bzero(&g, sizeof(t_game));
    
    if (parse_map(&g, "test_artifacts/random_2.cub")) {
        printf("SUCCESS\n");
    } else {
        printf("FAIL\n");
    }
    return 0;
}
