export CC=gcc
export RM=rm -f
export AR=ar cr
export MV=mv
export RANLIB=ranlib
export SED=sed


export SHELL=/bin/sh

INCFLAGS=-I.. -I../include -I../include/lua

CFLAGS=-w -O3 -rdynamic $(INCFLAGS)
#Pentium 4 (Intel)
#CFLAGS=-march=pentium4 -O3 -pipe -fomit-frame-pointer $(INCFLAGS)
#Athlon XP (AMD)
#CFLAGS="-march=athlon-xp -O3 -pipe -fomit-frame-pointer" $(INCFLAGS)
#Athlon64��
#CFLAGS="-O2 -march=athlon64 -ftracer -fprefetch-loop-arrays -pipe" $(INCFLAGS)

PROG=libbattle.a

SRC=battle.c battle_event.c battle_command.c battle_ai.c battle_magic.c pet_skill.c battle_item.c profession_skill.c
OBJ=$(SRC:.c=.o)

ifeq (0,$(MAKELEVEL))
CC=gcc
RM=rm -f
AR=ar cr
MV=mv
RANLIB=ranlib
SED=sed
SHELL=/bin/sh
endif

all: $(PROG)

$(PROG): $(OBJ)
	$(RM) $(PROG)
	$(AR) $(PROG) $(OBJ)
	$(RANLIB) $(PROG)

dos2unix:
	dos2unix $(SRC) makefile
	
chmod:
	chmod 777 $(SRC) makefile
	
depend:
	$(MV) makefile makefile.bak
	$(SED) -ne '1,/^# DO NOT DELETE THIS LINE/p' makefile.bak>makefile
	$(CC) $(INCFLAGS) -M $(SRC) >> makefile 

clean:
	$(RM) $(PROG)
	$(RM) $(OBJ)
	$(RM) *~

distclean:
	$(RM) $(PROG)
	$(RM) $(OBJ)
	$(RM) *~
	$(MV) makefile makefile.bak
	$(SED) -ne '1,/^# DO NOT DELETE THIS LINE/p' makefile.bak>makefile
	$(RM) makefile.bak

# DO NOT DELETE THIS LINE
battle.o: battle.c ../include/version.h ../include/correct_bug.h \
  ../include/common.h /usr/include/stdio.h /usr/include/features.h \
  /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h \
  /usr/lib/gcc/x86_64-redhat-linux/3.4.6/include/stddef.h \
  /usr/include/bits/types.h /usr/include/bits/wordsize.h \
  /usr/include/bits/typesizes.h /usr/include/libio.h \
  /usr/include/_G_config.h /usr/include/wchar.h /usr/include/bits/wchar.h \
  /usr/include/gconv.h \
  /usr/lib/gcc/x86_64-redhat-linux/3.4.6/include/stdarg.h \
  /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h \
  /usr/include/errno.h /usr/include/bits/errno.h \
  /usr/include/linux/errno.h /usr/include/asm/errno.h \
  /usr/include/asm-x86_64/errno.h ../include/longzoro/version.h \
  ../include/longzoro/longzoro.h ../include/longzoro/debug.h \
  /usr/include/string.h /usr/include/stdlib.h /usr/include/sys/types.h \
  /usr/include/time.h /usr/include/endian.h /usr/include/bits/endian.h \
  /usr/include/sys/select.h /usr/include/bits/select.h \
  /usr/include/bits/sigset.h /usr/include/bits/time.h \
  /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h \
  /usr/include/bits/sched.h /usr/include/alloca.h ../include/buf.h \
  ../include/object.h ../include/char.h ../include/char_base.h \
  ../include/version.h ../include/skill.h ../include/util.h \
  /usr/include/sys/time.h ../include/title.h ../include/addressbook.h \
  ../include/net.h ../include/autil.h ../include/md5_encrypt.h \
  /usr/include/netinet/in.h /usr/include/stdint.h \
  /usr/include/sys/socket.h /usr/include/sys/uio.h \
  /usr/include/bits/uio.h /usr/include/bits/socket.h \
  /usr/lib/gcc/x86_64-redhat-linux/3.4.6/include/limits.h \
  /usr/lib/gcc/x86_64-redhat-linux/3.4.6/include/syslimits.h \
  /usr/include/limits.h /usr/include/bits/posix1_lim.h \
  /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
  /usr/include/bits/posix2_lim.h /usr/include/bits/sockaddr.h \
  /usr/include/asm/socket.h /usr/include/asm-x86_64/socket.h \
  /usr/include/asm/sockios.h /usr/include/asm-x86_64/sockios.h \
  /usr/include/bits/in.h /usr/include/bits/byteswap.h \
  /usr/include/signal.h /usr/include/bits/signum.h \
  /usr/include/bits/siginfo.h /usr/include/bits/sigaction.h \
  /usr/include/bits/sigcontext.h /usr/include/bits/sigstack.h \
  /usr/include/bits/sigthread.h /usr/include/pthread.h \
  /usr/include/sched.h /usr/include/bits/initspin.h ../include/link.h \
  ../include/ggsrv30.h ../include/lua/lua.h ../include/lua/luaconf.h \
  ../include/lua/lauxlib.h ../include/lua/lua.h ../include/lua/lualib.h \
  ../include/char_data.h ../include/item.h ../include/char.h \
  ../include/char_base.h ../include/char_data.h ../include/battle.h \
  ../include/trade.h ../include/battle_event.h \
  ../include/battle_command.h ../include/battle_ai.h \
  ../include/configfile.h ../include/lssproto_serv.h \
  ../include/lssproto_util.h /usr/include/strings.h /usr/include/unistd.h \
  /usr/include/bits/posix_opt.h /usr/include/bits/confname.h \
  /usr/include/getopt.h ../include/encount.h ../include/enemy.h \
  ../include/handletime.h ../include/readmap.h ../include/pet_skill.h \
  ../include/npcutil.h ../include/magic.h ../include/npc_npcenemy.h \
  ../include/log.h ../include/pet_skillinfo.h ../include/anim_tbl.h \
  ../include/common.h ../include/battle_magic.h ../include/item.h \
  ../include/trade.h ../include/npc_autopk.h ../include/npc_healer.h \
  ../include/mylua/function.h ../include/mylua/mylua.h \
  ../include/lua/lua.h ../include/lua/lauxlib.h ../include/lua/lualib.h
battle_event.o: battle_event.c ../include/version.h \
  ../include/correct_bug.h ../include/common.h /usr/include/stdio.h \
  /usr/include/features.h /usr/include/sys/cdefs.h \
  /usr/include/gnu/stubs.h \
  /usr/lib/gcc/x86_64-redhat-linux/3.4.6/include/stddef.h \
  /usr/include/bits/types.h /usr/include/bits/wordsize.h \
  /usr/include/bits/typesizes.h /usr/include/libio.h \
  /usr/include/_G_config.h /usr/include/wchar.h /usr/include/bits/wchar.h \
  /usr/include/gconv.h \
  /usr/lib/gcc/x86_64-redhat-linux/3.4.6/include/stdarg.h \
  /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h \
  /usr/include/errno.h /usr/include/bits/errno.h \
  /usr/include/linux/errno.h /usr/include/asm/errno.h \
  /usr/include/asm-x86_64/errno.h ../include/longzoro/version.h \
  ../include/longzoro/longzoro.h ../include/longzoro/debug.h \
  /usr/include/string.h /usr/include/math.h /usr/include/bits/huge_val.h \
  /usr/include/bits/mathdef.h /usr/include/bits/mathcalls.h \
  ../include/char.h ../include/char_base.h ../include/version.h \
  ../include/skill.h ../include/util.h /usr/include/sys/time.h \
  /usr/include/time.h /usr/include/bits/time.h /usr/include/sys/select.h \
  /usr/include/bits/select.h /usr/include/bits/sigset.h \
  /usr/include/stdlib.h /usr/include/sys/types.h /usr/include/endian.h \
  /usr/include/bits/endian.h /usr/include/sys/sysmacros.h \
  /usr/include/bits/pthreadtypes.h /usr/include/bits/sched.h \
  /usr/include/alloca.h ../include/title.h ../include/addressbook.h \
  ../include/net.h ../include/autil.h ../include/md5_encrypt.h \
  /usr/include/netinet/in.h /usr/include/stdint.h \
  /usr/include/sys/socket.h /usr/include/sys/uio.h \
  /usr/include/bits/uio.h /usr/include/bits/socket.h \
  /usr/lib/gcc/x86_64-redhat-linux/3.4.6/include/limits.h \
  /usr/lib/gcc/x86_64-redhat-linux/3.4.6/include/syslimits.h \
  /usr/include/limits.h /usr/include/bits/posix1_lim.h \
  /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
  /usr/include/bits/posix2_lim.h /usr/include/bits/sockaddr.h \
  /usr/include/asm/socket.h /usr/include/asm-x86_64/socket.h \
  /usr/include/asm/sockios.h /usr/include/asm-x86_64/sockios.h \
  /usr/include/bits/in.h /usr/include/bits/byteswap.h \
  /usr/include/signal.h /usr/include/bits/signum.h \
  /usr/include/bits/siginfo.h /usr/include/bits/sigaction.h \
  /usr/include/bits/sigcontext.h /usr/include/bits/sigstack.h \
  /usr/include/bits/sigthread.h /usr/include/pthread.h \
  /usr/include/sched.h /usr/include/bits/initspin.h ../include/link.h \
  ../include/ggsrv30.h ../include/lua/lua.h ../include/lua/luaconf.h \
  ../include/lua/lauxlib.h ../include/lua/lua.h ../include/lua/lualib.h \
  ../include/char_data.h ../include/item.h ../include/char.h \
  ../include/char_event.h ../include/family.h ../include/handletime.h \
  ../include/battle.h ../include/trade.h ../include/battle_event.h \
  ../include/configfile.h ../include/pet.h ../include/log.h \
  ../include/lssproto_serv.h ../include/lssproto_util.h \
  /usr/include/strings.h /usr/include/unistd.h \
  /usr/include/bits/posix_opt.h /usr/include/bits/confname.h \
  /usr/include/getopt.h ../include/anim_tbl.h ../include/battle_magic.h \
  ../include/util.h ../include/enemy.h ../include/pet_skill.h \
  ../include/char_base.h ../include/item_event.h ../include/autil.h \
  ../include/mylua/function.h
battle_command.o: battle_command.c ../include/version.h \
  ../include/correct_bug.h ../include/common.h /usr/include/stdio.h \
  /usr/include/features.h /usr/include/sys/cdefs.h \
  /usr/include/gnu/stubs.h \
  /usr/lib/gcc/x86_64-redhat-linux/3.4.6/include/stddef.h \
  /usr/include/bits/types.h /usr/include/bits/wordsize.h \
  /usr/include/bits/typesizes.h /usr/include/libio.h \
  /usr/include/_G_config.h /usr/include/wchar.h /usr/include/bits/wchar.h \
  /usr/include/gconv.h \
  /usr/lib/gcc/x86_64-redhat-linux/3.4.6/include/stdarg.h \
  /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h \
  /usr/include/errno.h /usr/include/bits/errno.h \
  /usr/include/linux/errno.h /usr/include/asm/errno.h \
  /usr/include/asm-x86_64/errno.h ../include/longzoro/version.h \
  ../include/longzoro/longzoro.h ../include/longzoro/debug.h \
  /usr/include/string.h /usr/include/stdlib.h /usr/include/sys/types.h \
  /usr/include/time.h /usr/include/endian.h /usr/include/bits/endian.h \
  /usr/include/sys/select.h /usr/include/bits/select.h \
  /usr/include/bits/sigset.h /usr/include/bits/time.h \
  /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h \
  /usr/include/bits/sched.h /usr/include/alloca.h ../include/char.h \
  ../include/char_base.h ../include/version.h ../include/skill.h \
  ../include/util.h /usr/include/sys/time.h ../include/title.h \
  ../include/addressbook.h ../include/net.h ../include/autil.h \
  ../include/md5_encrypt.h /usr/include/netinet/in.h \
  /usr/include/stdint.h /usr/include/sys/socket.h /usr/include/sys/uio.h \
  /usr/include/bits/uio.h /usr/include/bits/socket.h \
  /usr/lib/gcc/x86_64-redhat-linux/3.4.6/include/limits.h \
  /usr/lib/gcc/x86_64-redhat-linux/3.4.6/include/syslimits.h \
  /usr/include/limits.h /usr/include/bits/posix1_lim.h \
  /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
  /usr/include/bits/posix2_lim.h /usr/include/bits/sockaddr.h \
  /usr/include/asm/socket.h /usr/include/asm-x86_64/socket.h \
  /usr/include/asm/sockios.h /usr/include/asm-x86_64/sockios.h \
  /usr/include/bits/in.h /usr/include/bits/byteswap.h \
  /usr/include/signal.h /usr/include/bits/signum.h \
  /usr/include/bits/siginfo.h /usr/include/bits/sigaction.h \
  /usr/include/bits/sigcontext.h /usr/include/bits/sigstack.h \
  /usr/include/bits/sigthread.h /usr/include/pthread.h \
  /usr/include/sched.h /usr/include/bits/initspin.h ../include/link.h \
  ../include/ggsrv30.h ../include/lua/lua.h ../include/lua/luaconf.h \
  ../include/lua/lauxlib.h ../include/lua/lua.h ../include/lua/lualib.h \
  ../include/char_data.h ../include/item.h ../include/char.h \
  ../include/char_base.h ../include/battle.h ../include/trade.h \
  ../include/pet_skill.h ../include/battle_event.h \
  ../include/battle_command.h ../include/lssproto_serv.h \
  ../include/lssproto_util.h /usr/include/strings.h /usr/include/unistd.h \
  /usr/include/bits/posix_opt.h /usr/include/bits/confname.h \
  /usr/include/getopt.h ../include/magic.h ../include/magic_base.h \
  ../include/handletime.h ../include/configfile.h
battle_ai.o: battle_ai.c ../include/version.h ../include/correct_bug.h \
  ../include/common.h /usr/include/stdio.h /usr/include/features.h \
  /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h \
  /usr/lib/gcc/x86_64-redhat-linux/3.4.6/include/stddef.h \
  /usr/include/bits/types.h /usr/include/bits/wordsize.h \
  /usr/include/bits/typesizes.h /usr/include/libio.h \
  /usr/include/_G_config.h /usr/include/wchar.h /usr/include/bits/wchar.h \
  /usr/include/gconv.h \
  /usr/lib/gcc/x86_64-redhat-linux/3.4.6/include/stdarg.h \
  /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h \
  /usr/include/errno.h /usr/include/bits/errno.h \
  /usr/include/linux/errno.h /usr/include/asm/errno.h \
  /usr/include/asm-x86_64/errno.h ../include/longzoro/version.h \
  ../include/longzoro/longzoro.h ../include/longzoro/debug.h \
  ../include/common.h ../include/char.h ../include/char_base.h \
  ../include/version.h ../include/skill.h ../include/util.h \
  /usr/include/sys/time.h /usr/include/time.h /usr/include/bits/time.h \
  /usr/include/sys/select.h /usr/include/bits/select.h \
  /usr/include/bits/sigset.h /usr/include/stdlib.h \
  /usr/include/sys/types.h /usr/include/endian.h \
  /usr/include/bits/endian.h /usr/include/sys/sysmacros.h \
  /usr/include/bits/pthreadtypes.h /usr/include/bits/sched.h \
  /usr/include/alloca.h ../include/title.h ../include/addressbook.h \
  ../include/net.h ../include/autil.h ../include/md5_encrypt.h \
  /usr/include/string.h /usr/include/netinet/in.h /usr/include/stdint.h \
  /usr/include/sys/socket.h /usr/include/sys/uio.h \
  /usr/include/bits/uio.h /usr/include/bits/socket.h \
  /usr/lib/gcc/x86_64-redhat-linux/3.4.6/include/limits.h \
  /usr/lib/gcc/x86_64-redhat-linux/3.4.6/include/syslimits.h \
  /usr/include/limits.h /usr/include/bits/posix1_lim.h \
  /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
  /usr/include/bits/posix2_lim.h /usr/include/bits/sockaddr.h \
  /usr/include/asm/socket.h /usr/include/asm-x86_64/socket.h \
  /usr/include/asm/sockios.h /usr/include/asm-x86_64/sockios.h \
  /usr/include/bits/in.h /usr/include/bits/byteswap.h \
  /usr/include/signal.h /usr/include/bits/signum.h \
  /usr/include/bits/siginfo.h /usr/include/bits/sigaction.h \
  /usr/include/bits/sigcontext.h /usr/include/bits/sigstack.h \
  /usr/include/bits/sigthread.h /usr/include/pthread.h \
  /usr/include/sched.h /usr/include/bits/initspin.h ../include/link.h \
  ../include/ggsrv30.h ../include/lua/lua.h ../include/lua/luaconf.h \
  ../include/lua/lauxlib.h ../include/lua/lua.h ../include/lua/lualib.h \
  ../include/char_data.h ../include/item.h ../include/char.h \
  ../include/char_base.h ../include/battle.h ../include/trade.h \
  ../include/npcutil.h ../include/pet_skill.h
battle_magic.o: battle_magic.c ../include/version.h \
  ../include/correct_bug.h ../include/common.h /usr/include/stdio.h \
  /usr/include/features.h /usr/include/sys/cdefs.h \
  /usr/include/gnu/stubs.h \
  /usr/lib/gcc/x86_64-redhat-linux/3.4.6/include/stddef.h \
  /usr/include/bits/types.h /usr/include/bits/wordsize.h \
  /usr/include/bits/typesizes.h /usr/include/libio.h \
  /usr/include/_G_config.h /usr/include/wchar.h /usr/include/bits/wchar.h \
  /usr/include/gconv.h \
  /usr/lib/gcc/x86_64-redhat-linux/3.4.6/include/stdarg.h \
  /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h \
  /usr/include/errno.h /usr/include/bits/errno.h \
  /usr/include/linux/errno.h /usr/include/asm/errno.h \
  /usr/include/asm-x86_64/errno.h ../include/longzoro/version.h \
  ../include/longzoro/longzoro.h ../include/longzoro/debug.h \
  /usr/include/string.h ../include/char.h ../include/char_base.h \
  ../include/version.h ../include/skill.h ../include/util.h \
  /usr/include/sys/time.h /usr/include/time.h /usr/include/bits/time.h \
  /usr/include/sys/select.h /usr/include/bits/select.h \
  /usr/include/bits/sigset.h /usr/include/stdlib.h \
  /usr/include/sys/types.h /usr/include/endian.h \
  /usr/include/bits/endian.h /usr/include/sys/sysmacros.h \
  /usr/include/bits/pthreadtypes.h /usr/include/bits/sched.h \
  /usr/include/alloca.h ../include/title.h ../include/addressbook.h \
  ../include/net.h ../include/autil.h ../include/md5_encrypt.h \
  /usr/include/netinet/in.h /usr/include/stdint.h \
  /usr/include/sys/socket.h /usr/include/sys/uio.h \
  /usr/include/bits/uio.h /usr/include/bits/socket.h \
  /usr/lib/gcc/x86_64-redhat-linux/3.4.6/include/limits.h \
  /usr/lib/gcc/x86_64-redhat-linux/3.4.6/include/syslimits.h \
  /usr/include/limits.h /usr/include/bits/posix1_lim.h \
  /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
  /usr/include/bits/posix2_lim.h /usr/include/bits/sockaddr.h \
  /usr/include/asm/socket.h /usr/include/asm-x86_64/socket.h \
  /usr/include/asm/sockios.h /usr/include/asm-x86_64/sockios.h \
  /usr/include/bits/in.h /usr/include/bits/byteswap.h \
  /usr/include/signal.h /usr/include/bits/signum.h \
  /usr/include/bits/siginfo.h /usr/include/bits/sigaction.h \
  /usr/include/bits/sigcontext.h /usr/include/bits/sigstack.h \
  /usr/include/bits/sigthread.h /usr/include/pthread.h \
  /usr/include/sched.h /usr/include/bits/initspin.h ../include/link.h \
  ../include/ggsrv30.h ../include/lua/lua.h ../include/lua/luaconf.h \
  ../include/lua/lauxlib.h ../include/lua/lua.h ../include/lua/lualib.h \
  ../include/char_data.h ../include/item.h ../include/char.h \
  ../include/char_base.h ../include/battle.h ../include/trade.h \
  ../include/battle_event.h ../include/magic_base.h \
  ../include/battle_magic.h ../include/item_event.h ../include/anim_tbl.h \
  ../include/common.h ../include/lssproto_serv.h \
  ../include/lssproto_util.h /usr/include/strings.h /usr/include/unistd.h \
  /usr/include/bits/posix_opt.h /usr/include/bits/confname.h \
  /usr/include/getopt.h
pet_skill.o: pet_skill.c ../include/version.h ../include/correct_bug.h \
  ../include/common.h /usr/include/stdio.h /usr/include/features.h \
  /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h \
  /usr/lib/gcc/x86_64-redhat-linux/3.4.6/include/stddef.h \
  /usr/include/bits/types.h /usr/include/bits/wordsize.h \
  /usr/include/bits/typesizes.h /usr/include/libio.h \
  /usr/include/_G_config.h /usr/include/wchar.h /usr/include/bits/wchar.h \
  /usr/include/gconv.h \
  /usr/lib/gcc/x86_64-redhat-linux/3.4.6/include/stdarg.h \
  /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h \
  /usr/include/errno.h /usr/include/bits/errno.h \
  /usr/include/linux/errno.h /usr/include/asm/errno.h \
  /usr/include/asm-x86_64/errno.h ../include/longzoro/version.h \
  ../include/longzoro/longzoro.h ../include/longzoro/debug.h \
  /usr/include/string.h ../include/common.h ../include/buf.h \
  ../include/configfile.h ../include/char_base.h ../include/version.h \
  ../include/skill.h ../include/util.h /usr/include/sys/time.h \
  /usr/include/time.h /usr/include/bits/time.h /usr/include/sys/select.h \
  /usr/include/bits/select.h /usr/include/bits/sigset.h \
  /usr/include/stdlib.h /usr/include/sys/types.h /usr/include/endian.h \
  /usr/include/bits/endian.h /usr/include/sys/sysmacros.h \
  /usr/include/bits/pthreadtypes.h /usr/include/bits/sched.h \
  /usr/include/alloca.h ../include/title.h ../include/addressbook.h \
  ../include/net.h ../include/autil.h ../include/md5_encrypt.h \
  /usr/include/netinet/in.h /usr/include/stdint.h \
  /usr/include/sys/socket.h /usr/include/sys/uio.h \
  /usr/include/bits/uio.h /usr/include/bits/socket.h \
  /usr/lib/gcc/x86_64-redhat-linux/3.4.6/include/limits.h \
  /usr/lib/gcc/x86_64-redhat-linux/3.4.6/include/syslimits.h \
  /usr/include/limits.h /usr/include/bits/posix1_lim.h \
  /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
  /usr/include/bits/posix2_lim.h /usr/include/bits/sockaddr.h \
  /usr/include/asm/socket.h /usr/include/asm-x86_64/socket.h \
  /usr/include/asm/sockios.h /usr/include/asm-x86_64/sockios.h \
  /usr/include/bits/in.h /usr/include/bits/byteswap.h \
  /usr/include/signal.h /usr/include/bits/signum.h \
  /usr/include/bits/siginfo.h /usr/include/bits/sigaction.h \
  /usr/include/bits/sigcontext.h /usr/include/bits/sigstack.h \
  /usr/include/bits/sigthread.h /usr/include/pthread.h \
  /usr/include/sched.h /usr/include/bits/initspin.h ../include/link.h \
  ../include/ggsrv30.h ../include/lua/lua.h ../include/lua/luaconf.h \
  ../include/lua/lauxlib.h ../include/lua/lua.h ../include/lua/lualib.h \
  ../include/char_data.h ../include/item.h ../include/char.h \
  ../include/char_base.h ../include/char_data.h ../include/pet_skill.h \
  ../include/battle.h ../include/trade.h ../include/battle_event.h \
  ../include/battle_magic.h ../include/item.h ../include/item_gen.h \
  ../include/char_talk.h ../include/autil.h
battle_item.o: battle_item.c ../include/version.h \
  ../include/correct_bug.h ../include/common.h /usr/include/stdio.h \
  /usr/include/features.h /usr/include/sys/cdefs.h \
  /usr/include/gnu/stubs.h \
  /usr/lib/gcc/x86_64-redhat-linux/3.4.6/include/stddef.h \
  /usr/include/bits/types.h /usr/include/bits/wordsize.h \
  /usr/include/bits/typesizes.h /usr/include/libio.h \
  /usr/include/_G_config.h /usr/include/wchar.h /usr/include/bits/wchar.h \
  /usr/include/gconv.h \
  /usr/lib/gcc/x86_64-redhat-linux/3.4.6/include/stdarg.h \
  /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h \
  /usr/include/errno.h /usr/include/bits/errno.h \
  /usr/include/linux/errno.h /usr/include/asm/errno.h \
  /usr/include/asm-x86_64/errno.h ../include/longzoro/version.h \
  ../include/longzoro/longzoro.h ../include/longzoro/debug.h \
  /usr/include/string.h ../include/char.h ../include/char_base.h \
  ../include/version.h ../include/skill.h ../include/util.h \
  /usr/include/sys/time.h /usr/include/time.h /usr/include/bits/time.h \
  /usr/include/sys/select.h /usr/include/bits/select.h \
  /usr/include/bits/sigset.h /usr/include/stdlib.h \
  /usr/include/sys/types.h /usr/include/endian.h \
  /usr/include/bits/endian.h /usr/include/sys/sysmacros.h \
  /usr/include/bits/pthreadtypes.h /usr/include/bits/sched.h \
  /usr/include/alloca.h ../include/title.h ../include/addressbook.h \
  ../include/net.h ../include/autil.h ../include/md5_encrypt.h \
  /usr/include/netinet/in.h /usr/include/stdint.h \
  /usr/include/sys/socket.h /usr/include/sys/uio.h \
  /usr/include/bits/uio.h /usr/include/bits/socket.h \
  /usr/lib/gcc/x86_64-redhat-linux/3.4.6/include/limits.h \
  /usr/lib/gcc/x86_64-redhat-linux/3.4.6/include/syslimits.h \
  /usr/include/limits.h /usr/include/bits/posix1_lim.h \
  /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
  /usr/include/bits/posix2_lim.h /usr/include/bits/sockaddr.h \
  /usr/include/asm/socket.h /usr/include/asm-x86_64/socket.h \
  /usr/include/asm/sockios.h /usr/include/asm-x86_64/sockios.h \
  /usr/include/bits/in.h /usr/include/bits/byteswap.h \
  /usr/include/signal.h /usr/include/bits/signum.h \
  /usr/include/bits/siginfo.h /usr/include/bits/sigaction.h \
  /usr/include/bits/sigcontext.h /usr/include/bits/sigstack.h \
  /usr/include/bits/sigthread.h /usr/include/pthread.h \
  /usr/include/sched.h /usr/include/bits/initspin.h ../include/link.h \
  ../include/ggsrv30.h ../include/lua/lua.h ../include/lua/luaconf.h \
  ../include/lua/lauxlib.h ../include/lua/lua.h ../include/lua/lualib.h \
  ../include/char_data.h ../include/item.h ../include/char.h \
  ../include/char_base.h ../include/battle.h ../include/trade.h \
  ../include/battle_event.h ../include/battle_item.h \
  ../include/battle_magic.h ../include/item_event.h ../include/log.h \
  ../include/anim_tbl.h ../include/npcutil.h ../include/magic_base.h \
  ../include/lssproto_serv.h ../include/lssproto_util.h \
  /usr/include/strings.h /usr/include/unistd.h \
  /usr/include/bits/posix_opt.h /usr/include/bits/confname.h \
  /usr/include/getopt.h ../include/magic.h
profession_skill.o: profession_skill.c ../include/version.h \
  ../include/correct_bug.h ../include/common.h /usr/include/stdio.h \
  /usr/include/features.h /usr/include/sys/cdefs.h \
  /usr/include/gnu/stubs.h \
  /usr/lib/gcc/x86_64-redhat-linux/3.4.6/include/stddef.h \
  /usr/include/bits/types.h /usr/include/bits/wordsize.h \
  /usr/include/bits/typesizes.h /usr/include/libio.h \
  /usr/include/_G_config.h /usr/include/wchar.h /usr/include/bits/wchar.h \
  /usr/include/gconv.h \
  /usr/lib/gcc/x86_64-redhat-linux/3.4.6/include/stdarg.h \
  /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h \
  /usr/include/errno.h /usr/include/bits/errno.h \
  /usr/include/linux/errno.h /usr/include/asm/errno.h \
  /usr/include/asm-x86_64/errno.h ../include/longzoro/version.h \
  ../include/longzoro/longzoro.h ../include/longzoro/debug.h
