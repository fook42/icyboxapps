#
#

CC      = /opt/amiga/bin/m68k-amigaos-gcc
STRIP   = /opt/amiga/bin/m68k-amigaos-strip
CFLAGS  = -noixemul -nostartfiles -nostdlib -flto -s -g0 -Os -m68020-60 -Wall -Wno-pointer-sign
TARGET  = I2CFreeMemDisplay
LDFLAGS = -noixemul -nostartfiles -nostdlib -flto

OBJDIR = objdir
SRC = $(wildcard *.c)
objects := $(patsubst %.c,%.o,$(wildcard *.c))
# OBJS = $(addprefix $(OBJDIR)/,$(dir $(SOURCE)), $(notdir $(SOURCE:.c=.o)))

clean:
	@rm -f $(TARGET) || true
	@rm -f $(objects) || true

all : clean $(objects)
	@$(CC) $(CFLAGS) -o $(TARGET) $(objects) $(LDFLAGS)
	@rm -f $(objects)

$(TARGET): $(SRC)
	echo "============="
	echo "building the target $@"
	echo "============="
	@$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
	@$(STRIP) $@ --strip-all
	@echo -- Link finished --

%.o : %.c
	$(CC) $(CFLAGS) -c $<
