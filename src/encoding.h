#ifndef ENCODING_H
#define ENCODING_H

/**
 * @file encoding.h
 * @brief Declaraciones de funciones para codificación de línea (Capa Física)
 * 
 * Esquemas implementados:
 * - NRZ (Non-Return to Zero)
 * - NRZI (Non-Return to Zero Inverted)
 * - Manchester
 * - 4B/5B
 */

// ============================================
// NRZ (Non-Return to Zero)
// ============================================

/**
 * @brief Codifica un bitstream usando NRZ
 * @param bitstream Cadena de bits ('0' y '1')
 * @return Cadena codificada (memoria dinámica, debe liberarse con free)
 */
char *encode_nrz(const char *bitstream);

/**
 * @brief Decodifica una señal NRZ
 * @param encoded Señal codificada
 * @return Bitstream original (memoria dinámica, debe liberarse con free)
 */
char *decode_nrz(const char *encoded);

// ============================================
// NRZI (Non-Return to Zero Inverted)
// ============================================

/**
 * @brief Codifica un bitstream usando NRZI
 * @param bitstream Cadena de bits ('0' y '1')
 * @return Cadena codificada (memoria dinámica, debe liberarse con free)
 */
char *encode_nrzi(const char *bitstream);

/**
 * @brief Decodifica una señal NRZI
 * @param encoded Señal codificada
 * @return Bitstream original (memoria dinámica, debe liberarse con free)
 */
char *decode_nrzi(const char *encoded);

// ============================================
// Manchester
// ============================================

/**
 * @brief Codifica un bitstream usando Manchester
 * @param bitstream Cadena de bits ('0' y '1')
 * @return Cadena codificada (memoria dinámica, debe liberarse con free)
 */
char *encode_manchester(const char *bitstream);

/**
 * @brief Decodifica una señal Manchester
 * @param encoded Señal codificada
 * @return Bitstream original (memoria dinámica, debe liberarse con free)
 */
char *decode_manchester(const char *encoded);

// ============================================
// 4B/5B
// ============================================

/**
 * @brief Codifica un bitstream usando 4B/5B
 * @param bitstream Cadena de bits (longitud debe ser múltiplo de 4)
 * @return Cadena codificada (memoria dinámica, debe liberarse con free)
 */
char *encode_4b5b(const char *bitstream);

/**
 * @brief Decodifica una señal 4B/5B
 * @param encoded Señal codificada (longitud debe ser múltiplo de 5)
 * @return Bitstream original (memoria dinámica, debe liberarse con free)
 */
char *decode_4b5b(const char *encoded);

// ============================================
// Visualización de señales
// ============================================

/**
 * @brief Genera un diagrama ASCII de la señal codificada
 * @param encoded Señal codificada
 * @param filename Archivo donde guardar el diagrama
 */
void plot_signal(const char *encoded, const char *filename);

// ============================================
// Simulación de ruido
// ============================================

/**
 * @brief Introduce errores aleatorios en el bitstream
 * @param bitstream Cadena de bits a modificar (se modifica in-place)
 * @param ber Bit Error Rate (probabilidad de error por bit, ej: 0.02 = 2%)
 */
void add_noise(char *bitstream, double ber);

#endif // ENCODING_H

