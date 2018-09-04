TARGET := mqtt_door
CC := arm-hisiv500-linux-gcc

INCLUDE:= -I ../include 

LIBS:= -L ../lib -Wl,-Bdynamic -l16cv300app -Wl,-Bstatic -lpaho-mqtt3a-static -Wl,-Bstatic -lsqlite3 -Wl,-Bdynamic -lpthread -ldl

#LIBS:= -L ../lib -l16cv300app -lpaho-mqtt3a -lsqlite3 -lpthread -ldl

CSRCS := $(wildcard *.c)
OBJS := $(patsubst %.c, %.o, $(CSRCS))

$(TARGET): $(OBJS) 
	$(CC) $+ -o $@  $(LIBS)
%.o:%.c
	$(CC) -c -g $< -o $@ $(INCLUDE)
clean:
	rm -rf $(OBJS) $(TARGET)
	#rm *.o
