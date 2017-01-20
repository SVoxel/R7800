noinst_PROGRAMS= phddns
phddns_SOURCES = base64.c blowfish.c generate.c log.c main.c md5.c phglobal.c phsocket.c phupdate.c phruncall.c
INCLUDES=-I.
bindir=$(prefix)/phddns
EXTRA_DIST= blowfish.h  generate.h  log.h  lutil.h  md5.h  phglobal.h  phsocket.h  phupdate.h phruncall.h
phddns_LDFLAGS=-s
phddns_LDADD=$(all_libraries)
