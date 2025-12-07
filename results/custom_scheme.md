# Documentación del Esquema de Codificación Propio

> **Estudiante**: [Tu nombre aquí]
> **Cédula**: [Tu cédula aquí]

---

## Información del Esquema

**Nombre del esquema**: [Nombre creativo para tu esquema]

---

## Mi Restricción Asignada

Mi cédula termina en: **[X]**

Mi restricción es: **[Copia tu restricción de la tabla del README]**

---

## Descripción del Algoritmo

### Idea general

[Describe en 2-3 párrafos la idea detrás de tu esquema]

### Regla de codificación

[Explica cómo se codifica cada bit o grupo de bits]

Ejemplo:
- `0` → [secuencia de salida]
- `1` → [secuencia de salida]

O si es por bloques:
| Entrada | Salida |
|---------|--------|
| 00 | |
| 01 | |
| 10 | |
| 11 | |

### Regla de decodificación

[Explica cómo se decodifica la señal recibida]

---

## Demostración de Propiedades

### Cumplimiento de la restricción asignada

**Mi restricción**: "[Tu restricción según tu cédula]"

**Demostración**:
[Demuestra formalmente o con ejemplos que tu esquema cumple la restricción]

[Incluye al menos 3 ejemplos de bitstreams codificados mostrando que se cumple]

### Análisis de eficiencia

- Bits de entrada: N
- Bits de salida: M
- **Eficiencia**: N/M × 100 = [X%]

### Autosincronización (opcional pero valorado)

[¿Es autosincronizante? Explica por qué sí o no]

---

## Ejemplo Completo

### Entrada
```
Bitstream: 11010010
```

### Proceso de codificación
```
[Muestra paso a paso cómo se codifica]
```

### Salida codificada
```
[Resultado]
```

### Diagrama de señal
```
Tiempo:    0   1   2   3   4   5   6   7   ...
Entrada:   1   1   0   1   0   0   1   0
Salida:    [diagrama ASCII de tu señal]
```

---

## Comparación con Otros Esquemas

| Propiedad | Mi Esquema | NRZ | Manchester | 4B/5B |
|-----------|------------|-----|------------|-------|
| Eficiencia | | 100% | 50% | 80% |
| Autosincronizante | | No | Sí | Parcial |
| Max. símbolos iguales consecutivos | | ∞ | 2 | 3 |
| Complejidad de implementación | | Baja | Media | Alta |

---

## Implementación

### Firma de funciones

```c
char *encode_custom(const char *bitstream);
char *decode_custom(const char *encoded);
```

### Notas de implementación

[Cualquier detalle importante sobre tu implementación]

---

## Conclusiones

[Reflexión sobre las ventajas y desventajas de tu esquema]

[¿En qué escenarios sería útil tu esquema?]

