CC = gcc
MAKE = make
INSTALL=install -c
CFLAGS = -g #-pg
LIBS = 	-lzmq -llua -ldl 
BIN = ./script/bin

BUILD = build
BUILD_GATE = $(BUILD)/gate
BUILD_SHARE = $(BUILD)/share
BUILD_WORKER = $(BUILD)/worker
BUILD_MASTER = $(BUILD)/master

GATE = gated
WORKER = workerd
MASTER = masterd
MONGO = mongo.so
ZMQ = zmq.so

SHARERCS = pzmq.c config.c option.c utils.c
GATERCS = main.c client.c worker.c swap.c
WORKERRCS = main.c
MASTERRCS = main.c

LUA_ZMQ = lualib/zmq
LUA_MONGO = lualib/mongo

all : gate worker master

lua : zmq mongo

clean :
	rm -rf $(BUILD)

$(BUILD) : $(BUILD_GATE) $(BUILD_WORKER) $(BUILD_SHARE) $(BUILD_MASTER)

$(BUILD_GATE) :
	mkdir -p $@
	
$(BUILD_WORKER) :
	mkdir -p $@

$(BUILD_SHARE) :
	mkdir -p $@
	
$(BUILD_MASTER):
	mkdir -p $@

SHARE_O :=

define BUILD_TEMP
  TAR :=  $(BUILD_SHARE)/$(notdir $(basename $(1)))
  SHARE_O := $(SHARE_O) $$(TAR).o
  $$(TAR).o : | $(BUILD_SHARE)
  -include $$(TAR).d
  $$(TAR).o : share/$(1)
	$(CC) $(CFLAGS) -c -Ishare -o $$@ -MMD $$< $(LIBS)
endef

$(foreach s,$(SHARERCS),$(eval $(call BUILD_TEMP,$(s))))

GATE_O :=

define BUILD_TEMP
  TAR :=  $(BUILD_GATE)/$(notdir $(basename $(1)))
  GATE_O := $(GATE_O) $$(TAR).o
  $$(TAR).o : | $(BUILD_GATE)
  -include $$(TAR).d
  $$(TAR).o : gate/$(1)
	$(CC) $(CFLAGS) -c -Igate -Ishare -o $$@ -MMD $$< $(LIBS)
endef

$(foreach s,$(GATERCS),$(eval $(call BUILD_TEMP,$(s))))

WORKER_O :=

define BUILD_TEMP
  TAR :=  $(BUILD_WORKER)/$(notdir $(basename $(1)))
  WORKER_O := $(WORKER_O) $$(TAR).o
  $$(TAR).o : | $(BUILD_WORKER)
  -include $$(TAR).d
  $$(TAR).o : worker/$(1)
	$(CC) $(CFLAGS) -c -Igate -Ishare -o $$@ -MMD $$< $(LIBS)
endef

$(foreach s,$(WORKERRCS),$(eval $(call BUILD_TEMP,$(s))))

MASTER_O :=

define BUILD_TEMP
  TAR :=  $(BUILD_MASTER)/$(notdir $(basename $(1)))
  MASTER_O := $(MASTER_O) $$(TAR).o
  $$(TAR).o : | $(BUILD_MASTER)
  -include $$(TAR).d
  $$(TAR).o : master/$(1)
	$(CC) $(CFLAGS) -c -Igate -Ishare -o $$@ -MMD $$< $(LIBS)
endef

$(foreach s,$(MASTERRCS),$(eval $(call BUILD_TEMP,$(s))))

gate : $(GATE_O) $(SHARE_O)
	@cd $(BUILD) && $(CC) $(CFLAGS) -o $(GATE) $(addprefix ../,$^) $(LIBS) 

worker : $(WORKER_O) $(SHARE_O)
	@cd $(BUILD) && $(CC) $(CFLAGS) -o $(WORKER) $(addprefix ../,$^) $(LIBS) 

master : $(MASTER_O) $(SHARE_O)
	@cd $(BUILD) && $(CC) $(CFLAGS) -o $(MASTER) $(addprefix ../,$^) $(LIBS) 
	
lib : $(SHARE_O)

zmq:
	$(MAKE) -C $(LUA_ZMQ)

mongo:
	$(MAKE) -C $(LUA_MONGO)

install: all
	$(INSTALL) $(BUILD)/$(GATE) $(BIN)
	$(INSTALL) $(BUILD)/$(WORKER) $(BIN)
	$(INSTALL) $(BUILD)/$(MASTER) $(BIN)
	$(INSTALL) $(BUILD)/$(MONGO) $(BIN)
	$(INSTALL) $(BUILD)/$(ZMQ) $(BIN)

.PHONY : all clean lib lua

