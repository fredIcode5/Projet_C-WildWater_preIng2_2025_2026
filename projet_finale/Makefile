# Variables
CC = gcc
CFLAGS = -Wall -Wextra -Werror -Iinclude
LEAKS = leaks_program
HISTO = histo_program
SRC = source

#par défaut
all: $(LEAKS) $(HISTO)

# Création de l'exécutable leaks
$(LEAKS): $(SRC)/leaks.c
	$(CC) $(CFLAGS) $< -o $@
	
# Création de l'exécutable histo
$(HISTO): $(SRC)/histo.c
	$(CC) $(CFLAGS) $< -o $@

# Nettoyage
.PHONY: all clean
clean:
	rm -f $(LEAKS) $(HISTO) *.o
