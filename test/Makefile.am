
bin_PROGRAMS=queue_demo


queue_demo_SOURCES = queue_demo.c
queue_demo_LDADD=../src/libgen.a

bin_PROGRAMS+=stack_demo
stack_demo_SOURCES = stack_demo.c
stack_demo_LDADD=../src/libgen.a

bin_PROGRAMS+=list_demo
list_demo_SOURCES = list_demo.c

AM_CFLAGS = -msse4.1 -I../include
