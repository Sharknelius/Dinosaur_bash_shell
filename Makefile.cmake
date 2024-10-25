CC = gcc
CFLAGS = -Wall -Wextra -std=c11

OBJ = my-shell.o
EXEC = my-shell
SRC = my-shell.c

$(EXEC): $(OBJ)
    $(CC) $(CFLAGS) -o $(EXEC) $(OBJ)

$(OBJ): $(SRC)
    $(CC) $(CFLAGS) -c $(SRC)

install:
    @echo "Installing $(EXEC)"
    sudo cp $(EXEC) /bin/

uninstall:
    @echo "Uninstalling $(EXEC)"
    sudo rm /bin/$(EXEC)

clean:
    rm -f $(EXEC) *.o