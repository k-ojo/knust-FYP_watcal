#!/bin/bash

#set compiler

CC=gcc

CFLAGS="-lmbus -lm"

#files for master object

MASTER_FILES="src/try.c src/controllers.c"

#Files for slave object

SLAVE_FILES="src/Ex.c src/controllers.c"

#compile master
$CC -o master $MASTER_FILES $CFLAGS

$CC -o slave $SLAVE_FILES $CFLAGS
