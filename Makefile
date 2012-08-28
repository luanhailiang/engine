CC = gcc
MAKE = make
INSTALL=install -c
CFLAGS = -g #-pg
LIBS = 	-lzmq -llua -ldl
BIN = ./script/bin

BUILD = build
BUILD_GATE = $(BUILD)/gate
BUILD_WORK = $(BUILD)/work
BUILD_SHARE = $(BUILD)/share
BUILD_MASTER = $(BUILD)/master

GATE = gated
WORK = workd
MASTER = masterd
MONGO = mongo.so
ZMQ = zmq.so

SHARERCS = pzmq.c config.c
GATERCS = main.c client.c worker.c swap.c
WORKRCS = main.c
MASTERRCS = main.c

LUA_ZMQ = lualib/zmq
LUA_MONGO = lualib/mongo

all : gate work master

lua : zmq mongo

clean :
	rm -rf $(BUILD)

$(BUILD) : $(BUILD_GATE) $(BUILD_WORK) $(BUILD_SHARE) $(BUILD_MASTER)

$(BUILD_GATE) :
	mkdir -p $@
	
$(BUILD_WORK) :
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

WORK_O :=

define BUILD_TEMP
  TAR :=  $(BUILD_WORK)/$(notdir $(basename $(1)))
  WORK_O := $(WORK_O) $$(TAR).o
  $$(TAR).o : | $(BUILD_WORK)
  -include $$(TAR).d
  $$(TAR).o : work/$(1)
	$(CC) $(CFLAGS) -c -Igate -Ishare -o $$@ -MMD $$< $(LIBS)
endef

$(foreach s,$(WORKRCS),$(eval $(call BUILD_TEMP,$(s))))

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

work : $(WORK_O) $(SHARE_O)
	@cd $(BUILD) && $(CC) $(CFLAGS) -o $(WORK) $(addprefix ../,$^) $(LIBS) 

master : $(MASTER_O) $(SHARE_O)
	@cd $(BUILD) && $(CC) $(CFLAGS) -o $(MASTER) $(addprefix ../,$^) $(LIBS) 
	
lib : $(SHARE_O)

zmq:
	$(MAKE) -C $(LUA_ZMQ)

mongo:
	$(MAKE) -C $(LUA_MONGO)

install: all
	$(INSTALL) $(BUILD)/$(GATE) $(BIN)
	$(INSTALL) $(BUILD)/$(WORK) $(BIN)
	$(INSTALL) $(BUILD)/$(MASTER) $(BIN)
	$(INSTALL) $(BUILD)/$(MONGO) $(BIN)
	$(INSTALL) $(BUILD)/$(ZMQ) $(BIN)

.PHONY : all clean lib lua

