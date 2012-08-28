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

GATE = gated
WORK = workd
MONGO = mongo.so
ZMQ = zmq.so

SHARERCS = pzmq.c config.c
GATERCS = main.c client.c worker.c swap.c
WORKRCS = main.c

LUA_ZMQ = lualib/zmq
LUA_MONGO = lualib/mongo

all : gate work 

lua : zmq mongo

clean :
	rm -rf $(BUILD)

$(BUILD) : $(BUILD_GATE) $(BUILD_WORK) $(BUILD_SHARE)

$(BUILD_GATE) :
	mkdir -p $@
	
$(BUILD_WORK) :
	mkdir -p $@

$(BUILD_SHARE) :
	mkdir -p $@

SHARE_O :=

define BUILD_temp
  TAR :=  $(BUILD_SHARE)/$(notdir $(basename $(1)))
  SHARE_O := $(SHARE_O) $$(TAR).o
  $$(TAR).o : | $(BUILD_SHARE)
  -include $$(TAR).d
  $$(TAR).o : share/$(1)
	$(CC) $(CFLAGS) -c -Ishare -o $$@ -MMD $$< $(LIBS)
endef

$(foreach s,$(SHARERCS),$(eval $(call BUILD_temp,$(s))))

GATE_O :=

define BUILD_temp
  TAR :=  $(BUILD_GATE)/$(notdir $(basename $(1)))
  GATE_O := $(GATE_O) $$(TAR).o
  $$(TAR).o : | $(BUILD_GATE)
  -include $$(TAR).d
  $$(TAR).o : gate/$(1)
	$(CC) $(CFLAGS) -c -Igate -Ishare -o $$@ -MMD $$< $(LIBS)
endef

$(foreach s,$(GATERCS),$(eval $(call BUILD_temp,$(s))))

WORK_O :=

define BUILD_temp
  TAR :=  $(BUILD_WORK)/$(notdir $(basename $(1)))
  WORK_O := $(WORK_O) $$(TAR).o
  $$(TAR).o : | $(BUILD_WORK)
  -include $$(TAR).d
  $$(TAR).o : work/$(1)
	$(CC) $(CFLAGS) -c -Igate -Ishare -o $$@ -MMD $$< $(LIBS)
endef

$(foreach s,$(WORKRCS),$(eval $(call BUILD_temp,$(s))))

gate : $(GATE_O) $(SHARE_O)
	@cd $(BUILD) && $(CC) $(CFLAGS) -o $(GATE) $(addprefix ../,$^) $(LIBS) 

work : $(WORK_O) $(SHARE_O)
	@cd $(BUILD) && $(CC) $(CFLAGS) -o $(WORK) $(addprefix ../,$^) $(LIBS) 
	
lib : $(SHARE_O)

zmq:
	$(MAKE) -C $(LUA_ZMQ)

mongo:
	$(MAKE) -C $(LUA_MONGO)

install: all
	$(INSTALL) $(BUILD)/$(GATE) $(BIN)
	$(INSTALL) $(BUILD)/$(WORK) $(BIN)
	$(INSTALL) $(BUILD)/$(MONGO) $(BIN)
	$(INSTALL) $(BUILD)/$(ZMQ) $(BIN)

.PHONY : all clean lib lua

