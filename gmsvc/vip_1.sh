#!/bin/sh
OUT=include/longzoro/longzoro.h
moshi=$1
moshi2=$2
make clean
echo "#ifndef __LONGZORO_H__
#define __LONGZORO_H__

//特殊功能控制----------------------------------------
#define _ATTESTAION_ID 1
#define _NO_WAIGUA
#define _175_SA_
#define _175_SA_2
#define _NEW_STONEAGE_TYPE
"${moshi}"
"${moshi2}"
#endif" > $OUT
make -j8
