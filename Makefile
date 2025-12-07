##############################################
# Proyecto: Codificación de Línea (Capa Física)
# Profesor: Jesus Santander
# Curso: Redes de Computadores
##############################################

# Compilador y banderas
CC = gcc
CFLAGS = -Wall -Werror -std=c11
SRC_DIR = src
RESULTS_DIR = results
BIN_DIR = bin

# Archivos fuente
SRCS = $(SRC_DIR)/encoding.c
TEST_SRC = $(SRC_DIR)/test_encoding.c

# Ejecutables
TEST_BIN = $(BIN_DIR)/test

##############################################
# Regla por defecto
##############################################
all: setup $(TEST_BIN)

##############################################
# Compilación
##############################################

$(TEST_BIN): $(SRCS) $(TEST_SRC)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $(TEST_BIN) $(SRCS) $(TEST_SRC)

##############################################
# Pruebas automáticas
##############################################
test: $(TEST_BIN)
	@echo "=== Ejecutando pruebas automatizadas ==="
	@./$(TEST_BIN)
	@echo "✅ Todas las pruebas completadas."

##############################################
# Ejecución manual (como entrega del alumno)
##############################################
run: $(TEST_BIN)
ifeq ($(args),)
	@echo "Uso: make run args=\"data/input_bits.txt 0.02\""
else
	@mkdir -p $(RESULTS_DIR)
	./$(TEST_BIN) $(args)
endif

##############################################
# Limpieza
##############################################
clean:
	@echo "🧹 Limpiando archivos generados..."
	rm -rf $(BIN_DIR) $(RESULTS_DIR) *.o
	@echo "Listo."

##############################################
# 🗂️ Inicialización de estructura
##############################################
setup:
	@mkdir -p $(SRC_DIR) $(BIN_DIR) $(RESULTS_DIR) data
	@touch $(RESULTS_DIR)/signals.txt