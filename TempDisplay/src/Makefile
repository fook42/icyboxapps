#
#

CC      = /opt/amiga/bin/m68k-amigaos-gcc
STRIP   = /opt/amiga/bin/m68k-amigaos-strip
CFLAGS  = -noixemul -nostartfiles -flto -s -g0 -Os -m68020-60 -Wall -Wno-pointer-sign
TARGET  = I2CTempDisplay
LDFLAGS = -noixemul -nostartfiles -flto -s

OBJDIR = objdir
SRC = $(wildcard *.c)
objects := $(patsubst %.c,%.o,$(wildcard *.c))
# OBJS = $(addprefix $(OBJDIR)/,$(dir $(SOURCE)), $(notdir $(SOURCE:.c=.o)))

clean:
	@rm -f $(TARGET) || true
	@rm -f $(objects) || true

all : clean $(objects)
	@$(CC) $(CFLAGS) $(LDFLAGS) -o $(TARGET) $(objects)
	@rm -f $(objects)

$(TARGET): $(SRC)
	echo "============="
	echo "building the target $@"
	echo "============="
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
	@$(STRIP) $@ --strip-all
	@echo -- Link finished --

%.o : %.c
	$(CC) $(CFLAGS) -c $<
