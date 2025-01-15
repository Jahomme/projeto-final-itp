#ifndef EAN8_UTILS_H
#define EAN8_UTILS_H

#include <stdbool.h>

bool is_valid_identifier(const char *identifier);
bool file_exists(const char *filename);
void generate_ean8_image(const char *identifier, int spacing, int pixels_per_area, int height, const char *output_filename);
bool extract_ean8_from_image(const char *filename, char *identifier);
bool decode_ean8_from_pixels(const int *pixels, int width, int height, char *identifier);


#endif 