# Tarea 1 – Capa Física: Sistemas de Codificación (NRZ, NRZI, Manchester, 4B/5B)

## Introducción

En esta práctica trabajaremos con **sistemas de codificación de línea**, que forman parte de la **Capa Física** del modelo OSI. Tu objetivo será **implementar, simular y analizar** diferentes esquemas de codificación digital utilizando el lenguaje **C**.

Los esquemas que estudiaremos son:

- NRZ (Non-Return to Zero)
- NRZI (Non-Return to Zero Inverted)
- Manchester
- 4B/5B

Además, implementarás una pequeña simulación de canal con errores y realizarás un
análisis sobre el comportamiento de cada esquema.

## Objetivos de Aprendizaje

1. Comprender cómo funcionan los esquemas de codificación de línea.
2. Implementar funciones modulares en C para codificar y decodificar bitstreams.
3. Simular condiciones reales de transmisión (ruido) y analizar su impacto.

## Estructura del Repositorio

Tu repositorio de GitHub Classroom deberá tener la siguiente estructura:

```
/
├── src/
│   ├── encoding.c
│   ├── encoding.h
│   ├── test_encoding.c
│   └── utils.c (opcional para funciones auxiliares)
├── data/
│   └── input_bits.txt  (bitstream de entrada de ejemplo)
├── results/
│   ├── signals.txt      (visualizaciones ASCII)
│   └── analysis.md      (análisis y reflexiones)
└── README.md            (este archivo)
```

## Instrucciones

### 1. Implementación de Codificadores/Decodificadores

Debes implementar las siguientes funciones en `encoding.c` y declarar sus prototipos
en `encoding.h`:

```c
char *encode_nrz(const char *bitstream);
char *decode_nrz(const char *encoded);

char *encode_nrzi(const char *bitstream);
char *decode_nrzi(const char *encoded);

char *encode_manchester(const char *bitstream);
char *decode_manchester(const char *encoded);

char *encode_4b5b(const char *bitstream);
char *decode_4b5b(const char *encoded);
```
Requisitos:

- Cada función debe devolver una cadena dinámica (malloc) con el resultado.
- El programa principal debe liberar toda la memoria utilizada.
- El código debe compilar sin warnings usando gcc -Wall -Werror.

### 2. Visualización de Señales

Implementa una función que genere un diagrama de tiempo ASCII para las señales codificadas:

```
void plot_signal(const char *encoded, const char *filename);
```

Ejemplo de salida (guardada en results/signals.txt):

```
NRZI encoding of 110010
Tiempo: 0   1   2   3   4   5
Señal:  ____----______----
```

Puedes ser creativo en el modo de visualización, pero debe ser legible y mostrar claramente las transiciones.

### 3. Simulación de Ruido

Implementa una función que simule ruido en el canal de transmisión, introduciendo errores de bit con una probabilidad configurable:

```c
void add_noise(char *bitstream, double ber);
```

#### Tu BER Personal

Tu valor de BER se determina por el **último dígito de tu cédula de identidad**:

```
BER = 0.0X   donde X = último dígito de tu cédula
Si X = 0, entonces usa X = 1
```

**Ejemplos:**
| Cédula termina en | BER a usar |
|-------------------|------------|
| 1 | 0.01 (1%) |
| 2 | 0.02 (2%) |
| 3 | 0.03 (3%) |
| ... | ... |
| 9 | 0.09 (9%) |
| 0 | 0.01 (1%) |

> ⚠️ **IMPORTANTE**: Debes usar TU BER personal en todos los análisis y simulaciones.
> Tu programa debe mostrar tu cédula y BER al ejecutarse.

### 4. Análisis Cualitativo

Crea un archivo `results/analysis.md` donde respondas:

#### Parte A: Preguntas Conceptuales
1. ¿Qué sistemas de codificación son autosincronizantes y por qué?
2. ¿Cuál es más eficiente en ancho de banda? Justifica con cálculos.
3. ¿Qué sucede ante errores aleatorios de transmisión?
4. ¿Cuál recomendarías para un enlace real de baja tasa de error y por qué?

#### Parte B: Análisis Cuantitativo (OBLIGATORIO)

Debes realizar experimentos y reportar datos reales de tu implementación:

5. **Overhead de codificación**: Para un mensaje de 1000 bits útiles, ¿cuántos bits
   totales se transmiten con cada esquema? Presenta una tabla comparativa.

6. **Análisis estadístico de errores**:
   - Ejecuta **N simulaciones** (ver tu configuración personal) con tu BER asignado
   - Para cada esquema, reporta: media de errores, desviación estándar, mínimo, máximo
   - Presenta los resultados en una tabla y/o gráfico

7. **Curva BER vs Tasa de Error Efectiva**:
   - Varía el BER de 0.001 a 0.1 en incrementos logarítmicos
   - Grafica cómo cada esquema se comporta ante diferentes niveles de ruido
   - ¿A partir de qué BER el esquema Manchester supera a NRZ? ¿Por qué?

8. **Análisis de ráfagas de errores**:
   - Implementa una función que simule errores en ráfaga (varios bits consecutivos)
   - ¿Qué esquema es más resistente a ráfagas? Demuéstralo experimentalmente.

### 5. Diseño de Esquema Propio (15% de la nota)

Debes diseñar e implementar tu **propio esquema de codificación** que sea **diferente**
a NRZ, NRZI, Manchester y 4B/5B.

#### Tu restricción personal (basada en tu cédula):

| Último dígito | Restricción que debe cumplir tu esquema |
|---------------|----------------------------------------|
| 1 | Debe tener al menos una transición cada 4 bits |
| 2 | No puede tener más de 3 niveles iguales consecutivos |
| 3 | Debe ser decodificable con un retraso máximo de 2 bits |
| 4 | Debe ser balanceado (igual cantidad de 1s y 0s en promedio) |
| 5 | Debe permitir detección de errores simples sin bits adicionales |
| 6 | Cada símbolo codificado debe empezar con transición |
| 7 | Eficiencia mínima del 60% |
| 8 | Debe funcionar correctamente con inversión de polaridad |
| 9 | Debe tener al menos una transición cada 3 bits |
| 0 | No puede tener más de 2 niveles iguales consecutivos |

#### Requisitos del esquema propio:

1. **Implementación completa**:
   ```c
   char *encode_custom(const char *bitstream);
   char *decode_custom(const char *encoded);
   ```

2. **Documentación formal** en `results/custom_scheme.md`:
   - Nombre del esquema (sé creativo)
   - Descripción del algoritmo de codificación
   - Tabla o regla de mapeo
   - **Demostración de que cumple TU restricción asignada**
   - Análisis de eficiencia (bits transmitidos / bits útiles)
   - Comparación con Manchester y NRZ

3. **Pruebas**: Debe pasar las pruebas de encode/decode igual que los otros esquemas.

4. **Visualización**: Incluir diagrama ASCII de tu esquema en `results/signals.txt`.

#### Ejemplo de documentación esperada:

```markdown
# Esquema: Mi-Code (Ejemplo)

## Mi restricción (cédula termina en 9)
"Debe tener al menos una transición cada 3 bits"

## Regla de codificación
- '0' → "01" (bajo, luego alto)
- '1' → "10" (alto, luego bajo)

## Eficiencia
50% (2 bits transmitidos por cada bit útil)

## Cumplimiento de restricción
✓ Cumple: siempre hay una transición cada 2 bits (mejor que el requisito de 3).
```

## Entregables

- Código fuente correctamente compilable y documentado (debe pasar pruebas unitarias)
- Archivo `results/signals.txt` con diagramas de salida de todos los esquemas (incluyendo el propio)
- Archivo `results/analysis.md` con todas las respuestas (Parte A y B)
- Archivo `results/custom_scheme.md` con documentación del esquema propio
- El programa debe mostrar tu cédula y BER personal al ejecutarse

## Evaluación

| Criterio | Peso |
|----------|------|
| Codificación / Decodificación correcta (NRZ, NRZI, Manchester, 4B/5B) | 20% |
| Uso correcto de BER personal (basado en cédula) | 10% |
| Simulación de canal con ruido | 10% |
| Visualización y organización del código | 10% |
| Análisis cualitativo (Parte A) | 10% |
| **Análisis cuantitativo con datos experimentales (Parte B)** | **15%** |
| **Diseño e implementación de esquema propio** | **15%** |
| Buenas prácticas de programación (modularidad, memoria, comentarios) | 10% |

> ⚠️ **IMPORTANTE**: Debes usar TU BER personal. Usar el BER de otro estudiante
> o copiar código resultará en **nota 0**.

## Ejecución Ejemplo

Compilación:

```
gcc -Wall -Werror -o test src/*.c
```

Ejecución:

```
./test data/input_bits.txt 0.02
```

Salida esperada (fragmento):

```
Esquema: Manchester
BER = 0.02
Bitstream original: 110010
Recibido: 110000
Errores: 1
```

## Cómo usar el Makefile

| Acción | Comando | Descripción |
|--------|---------|-------------|
| Compilar todo | `make` | Genera el ejecutable de pruebas |
| Ejecutar pruebas automáticas | `make test` | Ejecuta test_encoding.c |
| Ejecutar manualmente | `make run args="data/input_bits.txt 0.02"` | Corre el programa con archivo de bits y BER |
| Limpiar proyecto | `make clean` | Elimina binarios, resultados y archivos temporales |

