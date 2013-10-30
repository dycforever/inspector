CC=g++
export CPLUS_INCLUDE_PATH=../../misc/common:../:.:/home/dyc/tools/python3-protobuf_src/src/

export CPPFLAGS=\
		 -finline-functions \
		 -g \
		 -pipe \
		 -W \
		 -Wall \
		 -Werror \
		 -fPIC

export LDFLAGS=-lcommon -lpthread 
export LIBRARY_PATH=../../misc/common:/home/dyc/tools/python3-protobuf/lib/

.PHONY:clean all

all:Inspector test

Inspector:main.o ../messages/mesg.pb.o
	g++ -o $@ $^ $(CPPFLAGS) -L$(LIBRARY_PATH) $(LDFLAGS) -lelf -lprotobuf

test:test.o ../messages/mesg.pb.o
	g++ -o $@ $^ $(CPPFLAGS) -L$(LIBRARY_PATH) $(LDFLAGS) -lelf -lprotobuf

clean:
	rm -rf Inspector *.o *Header test 
