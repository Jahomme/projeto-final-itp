#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "ean8_utils.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Erro: Forneça o nome do arquivo PBM.\n");
        return 1;
    }

    char *input_filename = argv[1];
    if (!file_exists(input_filename)) {
        printf("Erro: O arquivo '%s' nao existe.\n", input_filename);
        return 1;
    }

    char identifier[9]; 
    if (!extract_ean8_from_image(input_filename, identifier)) {
        printf("Erro: Nao foi possivel extrair o identificador do arquivo '%s'.\n", input_filename);
        return 1;
    }

    printf("Identificador EAN-8 extraido: %s\n", identifier);
    return 0;
}
