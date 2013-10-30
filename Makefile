
.PHONY:all

all:
	/home/dyc/tools/python3-protobuf/bin/protoc ./messages/mesg.proto --cpp_out=.
	/home/dyc/tools/python3-protobuf/bin/protoc ./messages/mesg.proto --python_out=.
	make -j4 -C src
	cp -f src/Inspector .

clean:
	rm -f Inspector
	make clean -C src