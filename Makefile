PROJ_NAME = gameboy

CC = clang
LD = clang

SRC = main.c \
	cpu/z80.c \
	cpu/mem.c \
	interface/bios.c

BUILDDIR = build
OBJDIR = $(BUILDDIR)/obj

OBJ = $(addprefix $(OBJDIR)/,$(notdir $(SRC:.c=.o)))

SRCPATH = $(sort $(dir $(SRC)))
VPATH = $(SRCPATH)

CFLAGS = -c -g3

LDFLAGS = -g3
all: makedir $(OBJ) $(PROJ_NAME)

$(OBJDIR)/%.o: %.c Makefile
	@echo Compiling $<...
	@$(CC) $< $(CFLAGS) -o $@ -I.

$(PROJ_NAME): $(OBJ) 
	@echo Linking $(PROJ_NAME)...
	@$(LD) $(OBJ) $(LDFLAGS) -o $(BUILDDIR)/$(PROJ_NAME)

clean:
	rm -rf $(BUILDDIR)

makedir:
	@mkdir -p $(BUILDDIR)
	@mkdir -p $(OBJDIR)
