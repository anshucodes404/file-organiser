CC ?= cc
CFLAGS ?= -Wall -Wextra -O2
PREFIX ?= /usr/local
BINDIR ?= $(PREFIX)/bin

TARGET = org
SRC = org.c

.PHONY: all clean install uninstall

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^

install: $(TARGET)
	install -d $(BINDIR)
	install -m 0755 $(TARGET) $(BINDIR)/$(TARGET)

uninstall:
	rm -f $(BINDIR)/$(TARGET)

clean:
	rm -f $(TARGET)
