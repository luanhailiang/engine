CC= g++
CFLAGS= -g -O2 -shared -fPIC  -I/usr/include/mongo/ -I/usr/include/lua5.2/
AR= ar rcu
RANLIB= ranlib
RM= rm -f
LIBS=-lmongoclient -lboost_thread -lboost_filesystem
OUTLIB=mongo.so

BUILD = ../../build
BUILD_MONGO = $(BUILD)/mongo

LDFLAGS= $(LIBS)

OBJS = main.o mongo_bsontypes.o mongo_dbclient.o mongo_replicaset.o mongo_connection.o \
	mongo_cursor.o mongo_gridfile.o mongo_gridfs.o mongo_gridfschunk.o mongo_query.o utils.o

all: luamongo

clean:
	$(RM) $(BUILD_MONGO) $(BUILD)/$(OUTLIB)

luamongo: $(OBJS)
	$(CC) $(CFLAGS) $(addprefix $(BUILD_MONGO)/,$(OBJS)) -o $(BUILD)/$(OUTLIB) $(LDFLAGS)

$(BUILD) : $(BUILD_MONGO)

$(BUILD_MONGO) :
	mkdir -p $@

echo:
	@echo "CC = $(CC)"
	@echo "CFLAGS = $(CFLAGS)"
	@echo "AR = $(AR)"
	@echo "RANLIB = $(RANLIB)"
	@echo "RM = $(RM)"
	@echo "LDFLAGS = $(LDFLAGS)"

main.o: main.cpp utils.h
	$(CC) -c -o $(BUILD_MONGO)/$@ $< $(CFLAGS)
mongo_dbclient.o: mongo_dbclient.cpp common.h utils.h
	$(CC) -c -o $(BUILD_MONGO)/$@ $< $(CFLAGS)
mongo_connection.o: mongo_connection.cpp common.h utils.h
	$(CC) -c -o $(BUILD_MONGO)/$@ $< $(CFLAGS)
mongo_cursor.o: mongo_cursor.cpp common.h utils.h
	$(CC) -c -o $(BUILD_MONGO)/$@ $< $(CFLAGS)
mongo_gridfile.o: mongo_gridfile.cpp common.h utils.h
	$(CC) -c -o $(BUILD_MONGO)/$@ $< $(CFLAGS)
mongo_gridfs.o: mongo_gridfs.cpp common.h utils.h
	$(CC) -c -o $(BUILD_MONGO)/$@ $< $(CFLAGS)
mongo_gridfschunk.o: mongo_gridfschunk.cpp common.h utils.h
	$(CC) -c -o $(BUILD_MONGO)/$@ $< $(CFLAGS)
mongo_query.o: mongo_query.cpp common.h utils.h
	$(CC) -c -o $(BUILD_MONGO)/$@ $< $(CFLAGS)
mongo_replicaset.o: mongo_replicaset.cpp common.h utils.h
	$(CC) -c -o $(BUILD_MONGO)/$@ $< $(CFLAGS)
mongo_bsontypes.o: mongo_bsontypes.cpp common.h
	$(CC) -c -o $(BUILD_MONGO)/$@ $< $(CFLAGS)
utils.o: utils.cpp common.h utils.h
	$(CC) -c -o $(BUILD_MONGO)/$@ $< $(CFLAGS)

.PHONY: all 
