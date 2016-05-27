# Flags de gcc. Se usan automáticamente para compilar *.c → *.o
CFLAGS := -g -std=c99 -Wall -Werror -Wformat=2 -MMD
CFLAGS += -Wshadow -Wpointer-arith -Wunreachable-code
CFLAGS += -Wconversion -Wno-sign-conversion -Wbad-function-cast

# Si se ejecuta `make` a secas, se compilarán tac, tail y strutil.
all: uniq 

# Se puede escribir `make abb`, `make tail` o `make strutil.o` para compilar

# una parte del TP solamente.
uniq: uniq.o hash.o 
join: join.o
multi-merge: multi-merge.o heap.o
mediana: mediana.o heap.o testing.o pruebas_mediana.o main abb.o
abb: abb.o testing.o pruebas_abb_alumnos.o main_abb.o	
# Si tac o tail usan un TDA, se debe especificar bien en las lineas anteriores,
# bien aquí. Ejemplos:

# tac: pila.o
# tail: lista.o
# tail: cola.o

clean::
	rm -f tac tail *.o *.d

include deps.mk
-include $(SRCS:%.c=%.d)

.PHONY: all clean
