#include "encoding.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Función auxiliar para comparar strings de bits
void test_equal(const char *test_name, const char *expected, const char *actual) {
    if (strcmp(expected, actual) != 0) {
        fprintf(stderr, "❌ %s falló.\nEsperado: %s\nObtenido: %s\n", test_name,
                expected, actual);
        exit(1);
    } else {
        printf("✅ %s pasó.\n", test_name);
    }
}

int main(void) {
    printf("=== Iniciando pruebas automáticas de codificación ===\n");

    // Casos de prueba base — ajusta o amplía según tu curso
    const char *bitstream = "110010";

    // NRZ
    char *enc_nrz = encode_nrz(bitstream);
    char *dec_nrz = decode_nrz(enc_nrz);
    test_equal("NRZ encode/decode", bitstream, dec_nrz);
    free(enc_nrz);
    free(dec_nrz);

    // NRZI
    char *enc_nrzi = encode_nrzi(bitstream);
    char *dec_nrzi = decode_nrzi(enc_nrzi);
    test_equal("NRZI encode/decode", bitstream, dec_nrzi);
    free(enc_nrzi);
    free(dec_nrzi);

    // Manchester
    char *enc_man = encode_manchester(bitstream);
    char *dec_man = decode_manchester(enc_man);
    test_equal("Manchester encode/decode", bitstream, dec_man);
    free(enc_man);
    free(dec_man);

    // 4B/5B
    const char *bitstream_4b = "101011110000"; // múltiplo de 4 bits
    char *enc_4b5b = encode_4b5b(bitstream_4b);
    char *dec_4b5b = decode_4b5b(enc_4b5b);
    test_equal("4B/5B encode/decode", bitstream_4b, dec_4b5b);
    free(enc_4b5b);
    free(dec_4b5b);

    printf("🎉 Todas las pruebas automáticas pasaron correctamente.\n");
    return 0;
}