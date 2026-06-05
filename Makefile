#
# Makefile -- build system
#

CFILES= main.c util.c cons.c lexer.c parser.c
OFILES= ${CFILES:.c=.o}
DFILES= ${CFILES:.c=.d}

CFLAGS:=   -Wall -Werror -pedantic -std=c99
LDFLAGS:=  -static
VERSION:=  0.1.0
CPPFLAGS:= -DIN_FERMILISP -DVERSION="\"${VERSION}\"" -MD

TARG:=    flisp
BINNAME?= ${TARG}
BINDIR:=  /bin/
PREFIX:=  /usr/local

.SUFFIXES: .c .o
.PHONY:    all clean install uninstall

.c.o:
	${CC} ${CFLAGS} ${CPPFLAGS} -c $< -o $@

${TARG}: ${OFILES}
	${CC} ${OFILES} -o ${TARG} ${LDFLAGS}

all: ${TARG}

clean:
	rm -rf ${DFILES} ${OFILES} ${TARG}

install: all
	install -D ${PREFIX}${BINDIR}
	install -sm 755 ${TARG} ${PREFIX}${BINDIR}${BINNAME}

uninstall:
	rm -f ${PREFIX}${BINDIR}${BINNAME}

-include "${DFILES}"

