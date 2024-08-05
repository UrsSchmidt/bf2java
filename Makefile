TITLE=bf2java
TARGET_EXEC=$(TITLE)

# build target

SRC=src

CC=gcc
CFLAGS=-std=c18 -Wall -Wpedantic
LDLIBS=

$(TARGET_EXEC): $(SRC)/$(TITLE).c
	$(CC) $(CFLAGS) -o $@ $(SRC)/$(TITLE).c $(LDLIBS)

# install/uninstall targets

FILE_BIN=$(TARGET_EXEC)
FILE_MAN=$(TITLE).1
INSTALL_DIR_BIN=/usr/local/bin/
INSTALL_DIR_MAN=/usr/local/share/man/man1/
INSTALL_PATH_BIN=$(INSTALL_DIR_BIN)$(FILE_BIN)
INSTALL_PATH_MAN=$(INSTALL_DIR_MAN)$(FILE_MAN)

.PHONY: install
install: $(FILE_BIN) $(FILE_MAN)
ifneq ($(wildcard $(INSTALL_DIR_BIN).),)
	cp $(FILE_BIN) $(INSTALL_PATH_BIN)
else
	@echo directory not found $(INSTALL_DIR_BIN)
endif
ifneq ($(wildcard $(INSTALL_DIR_MAN).),)
	cp $(FILE_MAN) $(INSTALL_PATH_MAN)
	mandb -q
else
	@echo directory not found $(INSTALL_DIR_MAN)
endif

.PHONY: uninstall
uninstall:
ifneq ($(wildcard $(INSTALL_PATH_BIN)),)
	rm -f $(INSTALL_PATH_BIN)
endif
ifneq ($(wildcard $(INSTALL_PATH_MAN)),)
	rm -f $(INSTALL_PATH_MAN)
	mandb -q
endif

# clean target

.PHONY: clean
clean:
	rm -f $(TARGET_EXEC) *.class *.jasmin
