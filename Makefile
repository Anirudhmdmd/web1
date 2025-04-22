CC=gcc
CFLAGS=-fPIC -Wall -I. -I/opt/comptel/eventlink/sdk/el-c-cxx-sdk/include
LDFLAGS=-shared
TARGET=app_bln.so
SRC=app_bln.c

all:
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)
