#	Makefile for TABULA VIGILANS
CFLAGS = -g
CXXFLAGS = -g -DOSC_HOST_LITTLE_ENDIAN=1 -I OSC
LIBDIR = 
LIBS = -lportmidi -lporttime -lasound -lm -lstdc++
#
#	Files
#
HDRS=   midi.h rules.h tv.h xtab.h

SRCS1=	y.tab.c main.c tvlex.c execute.c tab.c pass2.c prprog.c
SRCS2=	rules1.c rules2.c mathfunc.c tables.c perm.c
SRCS=	$(SRCS1) $(SRCS2)
MSRCS=	midi.c midirule.c
ASRCS=	atari.c
USRCS=	unix.c
OSRCS=	OSC/IpEndpointName.cpp OSC/OscOutboundPacketStream.cpp \
	OSC/p_NetworkingUtils.cpp OSC/p_UdpSocket.cpp OSC/do_send.cpp

OBJS1=	y.tab.o main.o tvlex.o execute.o tab.o pass2.o prprog.o
OBJS2=	rules1.o rules2.o mathfunc.o tables.o perm.o
OBJS=	$(OBJS1) $(OBJS2)
MOBJS=	midi.o midirule.o
AOBJS=	atari.o
UOBJS=	unix.o
OOBJS=	IpEndpointName.o OscOutboundPacketStream.o \
	p_NetworkingUtils.o p_UdpSocket.o do_send.o

all:	tv TAGS encode decode

tv:	makefile $(OBJS) $(UOBJS) $(MOBJS) $(OOBJS)
	$(CC) $(OBJS) $(UOBJS)  $(MOBJS)  $(OOBJS) $(LIBS) -o tv

.c.o:	makefile
	$(CC) -c $(CFLAGS) $*.c

IpEndpointName.o: makefile OSC/IpEndpointName.cpp 
	$(CXX) -c $(CXXFLAGS) -o IpEndpointName.o OSC/IpEndpointName.cpp

OscOutboundPacketStream.o:	makefile OSC/OscOutboundPacketStream.cpp
	$(CXX) -c $(CXXFLAGS) -o OscOutboundPacketStream.o OSC/OscOutboundPacketStream.cpp

p_NetworkingUtils.o:	makefile OSC/p_NetworkingUtils.cpp 
	$(CXX) -c $(CXXFLAGS) -o p_NetworkingUtils.o OSC/p_NetworkingUtils.cpp

p_UdpSocket.o:	makefile OSC/p_UdpSocket.cpp
	$(CXX) -c $(CXXFLAGS) -o p_UdpSocket.o OSC/p_UdpSocket.cpp

do_send.o:	makefile OSC/do_send.cpp
	$(CXX) -c $(CXXFLAGS) -o do_send.o OSC/do_send.cpp

#.cpp.o:	makefile
#	$(CXX) -c $(CXXFLAGS) $*.cpp

TAGS:	$(SRCS) $(HDR) $(MSRCS) $(ASRCS) $(USRCS) $(OSRCS)
	etags $(SRCS) $(HDR) $(ASRCS) $(USRCS) $(OSRCS)

#
#targets
#

TV.zip:	Makefile $(SRCS) $(HDR) $(MSRCS) $(ASRCS) $(USRCS) $(OSRCS)
	zip TV.zip atari.c mac.c main.c Makefile Makefile.osx mathfunc.c \
	midi.c midi.h midirule.c pass2.c perm.c prprog.c rules1.c rules2.c \
	rules.h tab.c tables.c tvdecode.c tvencode.c tv.h tvlex.c tv.lnk \
	tv.mac tvnf.lnk tv.y unix.c win32.c xtab.h y.tab.c y.tab.h \
	$(OSRCS)
#
#dependencies
#

xtab.h:		y.tab.h 
	cp y.tab.h xtab.h

y.tab.c:	tv.y
	yacc -d -t tv.y

y.tab.h:	tv.y
	yacc -d -t tv.y

encode:	tvencoder.c
	cc -o encode tvencoder.c

decode: tvdecode.c
	cc -o decode tvdecode.c

makefile:	Makefile
	$(CC) -M $(SRCS) $(MSRCS) $(USRCS) > deps
	$(CXX) -DOSC_HOST_LITTLE_ENDIAN=1 -M $(OSRCS) >> deps
	cat Makefile deps > makefile
	rm deps

# ***************** DO NOT CHANGE BELOW HERE *****************
