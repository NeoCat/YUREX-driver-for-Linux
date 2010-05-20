BIN = async-read-sample timing-game

all: ${BIN}

clean:
	rm -f ${BIN}

%: %.c
	gcc -Os -g -o $@ $@.c
