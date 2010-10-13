g711$(OBJ): ../media/g711.c
        $(CC) -c $(CFLAGS) $(IPATHS) ../media/g711.c


ooCommon$(OBJ): ../media/ooCommon.c ../media/ooCommon.h ../media/ooSock.h
        $(CC) -c $(CFLAGS) $(IPATHS) ../media/ooCommon.c


ooSock$(OBJ): ../media/ooSock.c ../media/ooSock.h
        $(CC) -c $(CFLAGS) $(IPATHS) ../media/ooSock.c


ooWave$(OBJ): ../media/ooWave.c ../media/ooWave.h ../media/ooCommon.h \
  ../media/ooSock.h
        $(CC) -c $(CFLAGS) $(IPATHS) ../media/ooWave.c


oortp$(OBJ): ../media/oortp.c ../media/ooCommon.h ../media/ooSock.h \
  ../media/oortp.h ../media/g711.h ../media/oomedialx.h
        $(CC) -c $(CFLAGS) $(IPATHS) ../media/oortp.c

