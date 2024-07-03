#!/bin/bash

#set compiler

CC=gcc

CFLAGS="-lmbus"

#files for master object

MASTER_FILES="master.c helper.c"

#Files for slave object

SLAVE_FILES="slave.c helper.c"

#compile master
$CC -o master $MASTER_FILES $CFLAGS

$CC -o slave $SLAVE_FILES $CFLAGS
