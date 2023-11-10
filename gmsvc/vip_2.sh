#!/bin/sh
OUT=include/longzoro/longzoro.h
moshi=$1
moshi2=$2
if [ "$moshi" == "gm" ]; then
	moshi=""
else
	moshi="#define _NO_GM_MAGIC"
fi
if [ "$moshi2" == "debug" ]; then
	moshi2="#define _DEBUG_ALL"
fi
make clean
echo "#ifndef __LONGZORO_H__
#define __LONGZORO_H__

//特殊功能控制----------------------------------------
#define _ATTESTAION_ID 1
#define _NO_WAIGUA
#define _175_SA_
#define _NEW_STONEAGE_TYPE
#define _182_SA_2
#define _NEW_PACKET_CODE
"${moshi}"
"${moshi2}"
#endif" > $OUT
make
