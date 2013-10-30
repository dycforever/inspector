cp swap.o swap.full
objcopy --only-keep-debug swap.full swap.debug 
strip -g swap.o
