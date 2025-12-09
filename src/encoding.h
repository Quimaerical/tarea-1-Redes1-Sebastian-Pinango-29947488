#ifndef ENCODING_H
#define ENCODING_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

// Prototipos de codificación/decodificación
char *encode_nrz(const char *bitstream);
char *decode_nrz(const char *encoded);

char *encode_nrzi(const char *bitstream);
char *decode_nrzi(const char *encoded);

char *encode_manchester(const char *bitstream);
char *decode_manchester(const char *encoded);

char *encode_4b5b(const char *bitstream);
char *decode_4b5b(const char *encoded);

// Prototipos del esquema propio (Custom)
char *encode_custom(const char *bitstream);
char *decode_custom(const char *encoded);

// Herramientas de visualización y ruido
void plot_signal(const char *encoded, const char *filename, const char *label);
void add_noise(char *bitstream, double ber);
void add_burst_noise(char *bitstream, double prob_burst);
char *my_strdup(const char *s);

#endif