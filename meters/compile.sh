#!/bin/bash

#set compiler

CC=gcc

CFLAGS="-lmbus -lm -lcurl"

MASTER_FILES="src/master.c src/watCal.c"

#Files for slave object

SLAVE_FILES="src/slave.c src/watCal.c"

#compile master
$CC -o master $MASTER_FILES $CFLAGS

$CC -o slave $SLAVE_FILES $CFLAGS
