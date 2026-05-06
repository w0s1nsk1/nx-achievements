# NXAchievements switch-viewer (libnx)

TARGET      := NXAchievements
BUILD       := build
SOURCES     := source
INCLUDES    := include ../core/include ../core/vendor

# Reuse portable core sources directly for now.
CORE_SOURCES := \
	../core/src/AchievementEngine.cpp \
	../core/src/ConditionEvaluator.cpp \
	../core/src/PackLoader.cpp \
	../core/src/ProfileStore.cpp

# Include all cpp files from source + core list.
CFILES      :=
CPPFILES    := $(foreach dir,$(SOURCES),$(wildcard $(dir)/*.cpp)) $(CORE_SOURCES)

ARCH        := -march=armv8-a+crc+crypto -mtune=cortex-a57 -mtp=soft -fPIE
CFLAGS      := -g -Wall -O2 -ffunction-sections $(ARCH)
CXXFLAGS    := $(CFLAGS) -std=gnu++17
LDFLAGS     := -specs=$(DEVKITPRO)/libnx/switch.specs -g $(ARCH) -Wl,-Map,$(notdir $*.map)
LIBS        := -lnx

export OUTPUT := $(CURDIR)/$(TARGET)
export TOPDIR := $(CURDIR)

ifneq ($(BUILD),$(notdir $(CURDIR)))
export DEPSDIR   := $(CURDIR)/$(BUILD)

export VPATH     := $(foreach dir,$(SOURCES),$(CURDIR)/$(dir))
export VPATH     += $(CURDIR)/../core/src
export INCLUDES  := $(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir))

export CFILES    := $(CFILES)
export CPPFILES  := $(notdir $(CPPFILES))

.PHONY: all clean

all: $(BUILD)

$(BUILD):
	@[ -d $@ ] || mkdir -p $@
	@$(MAKE) --no-print-directory -C $(BUILD) -f $(TOPDIR)/Makefile

clean:
	@echo clean ...
	@rm -fr $(BUILD) $(TARGET).nro $(TARGET).elf $(TARGET).nacp

else

.PHONY: all

all: $(OUTPUT).nro

$(OUTPUT).nro: $(OUTPUT).elf

include $(DEVKITPRO)/libnx/switch_rules

endif
