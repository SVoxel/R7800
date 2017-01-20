#
# fw_pack is utility to create the signed image of the firmware file
# 
OBJS=obj
SRC=src
INC=inc
CFLAGS +=-g -I${INC}
LDFLAGS=
CC=gcc

fw_pack: ${OBJS} ${OBJS}/fw_pack.o ${OBJS}/util.o
	${CC} ${OBJS}/fw_pack.o ${OBJS}/util.o  -o fw_pack

${OBJS}/fw_pack.o: ${SRC}/fw_pack.c ${INC}/*.h
	${CC} ${CFLAGS} -c ${SRC}/fw_pack.c -o ${OBJS}/fw_pack.o

${OBJS}/util.o: ${SRC}/util.c ${INC}/*.h
	${CC} ${CFLAGS} -c ${SRC}/util.c -o ${OBJS}/util.o
${OBJS} :
	mkdir ${OBJS}
clean:
	rm -rf obj
	rm -f fw_pack 
