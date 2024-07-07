#!/bin/bash

#set compiler

CC=gcc

CFLAGS="-lmbus -lm"

#files for master object
MASTER_FILES="src/master.c"

#Files for slave object

SLAVE_FILES="src/slave.c src/generators.c"

#compile master
$CC -o master $MASTER_FILES $CFLAGS

$CC -o slave $SLAVE_FILES $CFLAGS
