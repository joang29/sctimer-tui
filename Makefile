CXX ?= g++

NAME = sctimer
BIN = /usr/local/bin/sctimer
SRC = timer/src/functions.h timer/src/interface.cpp timer/src/keys.cpp timer/src/main.cpp timer/src/settings.cpp timer/src/statics.cpp timer/src/timer.cpp

install:
	${CXX} ${SRC} -o ${NAME}
	install -Dm 775 ${NAME} ${BIN}

clean:
	rm ${BIN}

