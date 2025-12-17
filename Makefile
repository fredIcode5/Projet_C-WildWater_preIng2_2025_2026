# Variables
CC = gcc
CFLAGS = -Wall -Wextra -std=c11
LDFLAGS = -lm
EXEC = leaks_program
SRC = leaks.c 

# Règle par défaut (compilation)
all: $(EXEC)

# Création de l'exécutable
$(EXEC): $(SRC)
	$(CC) $(CFLAGS) -o $(EXEC) $(SRC) $(LDFLAGS)

# Nettoyage
.PHONY: clean
clean:
	rm -f $(EXEC) *.o