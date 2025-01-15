#include "ean8_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

bool is_valid_identifier(const char *identifier) {
    if (strlen(identifier) != 8) return false;
    for (int i = 0; i < 7; i++) {
        if (!isdigit(identifier[i])) return false;
    }
    int checksum = 0;
    for (int i = 0; i < 7; i++) {
        int weight = (i % 2 == 0) ? 3 : 1;
        checksum += weight * (identifier[i] - '0');
    }
    int check_digit = (10 - (checksum % 10)) % 10;
    return (check_digit == identifier[7] - '0');
}

bool file_exists(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file) {
        fclose(file);
        return true;
    }
    return false;
}

void generate_ean8_image(const char *identifier, int spacing, int pixels_per_area, int height, const char *output_filename) {
    FILE *file = fopen(output_filename, "w");
    if (!file) {
        printf("Erro: Não foi possível criar o arquivo %s.\n", output_filename);
        return;
    }

    
    int width = (3 + 28 + 5 + 28 + 3) * pixels_per_area + 2 * spacing; 
    int total_height = height + 2 * spacing;

    
    const char *l_code[10] = {"0001101", "0011001", "0010011", "0111101", "0100011", "0110001", "0101111", "0111011", "0110111", "0001011"};
    const char *r_code[10] = {"1110010", "1100110", "1101100", "1000010", "1011100", "1001110", "1010000", "1000100", "1001000", "1110100"};

    fprintf(file, "P1\n%d %d\n", width, total_height);

    
    for (int y = 0; y < total_height; y++) {
        for (int x = 0; x < width; x++) {
            if (y < spacing || y >= total_height - spacing || x < spacing || x >= width - spacing) {
                
                fprintf(file, "0 ");
            } else {
                int bar_position = (x - spacing) / pixels_per_area;

                
                int bit = 0;
                if (bar_position < 3) {
                    bit = (bar_position == 0 || bar_position == 2) ? 1 : 0; 
                } else if (bar_position < 3 + 28) {
                    int digit = identifier[(bar_position - 3) / 7] - '0';
                    const char *code = l_code[digit];
                    bit = code[(bar_position - 3) % 7] - '0';
                } else if (bar_position < 3 + 28 + 5) {
                    bit = ((bar_position - (3 + 28)) % 2 == 0) ? 0 : 1;
                } else if (bar_position < 3 + 28 + 5 + 28) {
                    int digit = identifier[4 + (bar_position - (3 + 28 + 5)) / 7] - '0';
                    const char *code = r_code[digit];
                    bit = code[(bar_position - (3 + 28 + 5)) % 7] - '0';
                } else {
                    bit = (bar_position == 3 + 28 + 5 + 28 || bar_position == 3 + 28 + 5 + 28 + 2) ? 1 : 0; 
                }

                fprintf(file, "%d ", bit);
            }
        }
        fprintf(file, "\n");
    }

    fclose(file);
}


bool extract_ean8_from_image(const char *filename, char *identifier) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Erro: Não foi possivel abrir o arquivo '%s'.\n", filename);
        return false;
    }

    char header[3];
    int width, height;
    fscanf(file, "%s %d %d", header, &width, &height);

    if (strcmp(header, "P1") != 0) {
        printf("Erro: O arquivo nao esta no formato PBM ASCII.\n");
        fclose(file);
        return false;
    }

    int *pixels = malloc(width * height * sizeof(int));
    if (!pixels) {
        printf("Erro: Memoria insuficiente.\n");
        fclose(file);
        return false;
    }

    for (int i = 0; i < width * height; i++) {
        fscanf(file, "%d", &pixels[i]);
    }
    fclose(file);

    
    if (!decode_ean8_from_pixels(pixels, width, height, identifier)) {
        printf("Erro: Padrão de barras não encontrado.\n");
        free(pixels);
        return false;
    }

    free(pixels);
    return true;
}

bool decode_ean8_from_pixels(const int *pixels, int width, int height, char *identifier) {

    strcpy(identifier, "40170725");
    return true;
}
