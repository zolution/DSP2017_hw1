.PHONY: all clean

CFLAGS+=
LDFLAGS+=-lm     # link to math library

TARGET=test train acc_calculate

all: $(TARGET)
# type make/make all to compile test_hmm

clean:
	$(RM) $(TARGET)   # type make clean to remove the compiled file
