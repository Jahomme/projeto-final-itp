
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "ean8_utils.h"

int main(int argc, char *argv[]) {
    
    if (argc < 2) {
        printf("Erro: Forneça um identificador EAN-8.\n");
        return 1;
    }

    char *identifier = argv[1];
    int spacing = (argc > 2) ? atoi(argv[2]) : 4;
    int pixels_per_area = (argc > 3) ? atoi(argv[3]) : 3;
    int height = (argc > 4) ? atoi(argv[4]) : 50;
    char *output_filename = (argc > 5) ? argv[5] : "ean8_output.pbm";

     if (strlen(identifier) < 8) {
        printf("Erro: O identificador possui menos que 8 dígitos. Por favor, insira um identificador válido.\n");
        return 1;
    }

    if (!is_valid_identifier(identifier)) {
        printf("Erro: Identificador invalido.\n");
        return 1;
    }

    if (file_exists(output_filename)) {
        char choice;
        printf("Arquivo '%s' ja existe. Sobrescrever? (s/n): ", output_filename);
        scanf(" %c", &choice);
        if (choice != 's' && choice != 'S') {
            printf("Erro: Arquivo existente. Execucao encerrada.\n");
            return 1;
        }
    }

    generate_ean8_image(identifier, spacing, pixels_per_area, height, output_filename);
    printf("Imagem PBM gerada com sucesso em '%s'.\n", output_filename);

    return 0;
}
