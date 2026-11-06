SRCROOT  = .
BUILD    = build
TARGET   = humidity_controller

CFLAGS  := -Wall -Wextra -pedantic -std=c99
override CFLAGS += -I$(SRCROOT) -I$(SRCROOT)/include

LDFLAGS :=
override LDFLAGS += -L $(BUILD)

LDLIBS  :=
override LDLIBS += -lrtos -lcmd

# For the ST Nucleo F103RB board:
# ==================
AR       = arm-none-eabi-ar
CC       = arm-none-eabi-gcc
AS       = $(CC) -x assembler-with-cpp
OBJCOPY  = arm-none-eabi-objcopy
SIZE     = arm-none-eabi-size

override CFLAGS +=                                 \
	-mcpu=cortex-m3 -mthumb                    \
	-DSTM32F103xB

override LDFLAGS +=                                \
	-mcpu=cortex-m3 -mthumb                    \
	-T $(SRCROOT)/src/linker.ld       \
	-nostdlib

override LDLIBS += -lperiph

override CFLAGS := $(strip $(CFLAGS))
override LDFLAGS := $(strip $(LDFLAGS))
override LDLIBS := $(strip $(LDLIBS))

.DEFAULT_GOAL = all

# periph
# ==================

periph_srcs = $(wildcard $(SRCROOT)/periph/*.c)
periph_objs = $(patsubst $(SRCROOT)/periph/%,$(BUILD)/periph.dir/%.o,$(periph_srcs))
periph_deps = $(periph_objs:.o=.d)

$(BUILD)/periph.dir/%.c.o: $(SRCROOT)/periph/%.c | $(BUILD)/periph.dir
	$(CC) $(CFLAGS) -MMD -MP -MF $(@:.o=.d) -c $< -o $@

$(BUILD)/periph.dir:
	mkdir -p $@

-include $(periph_deps)

.PHONY: periph
periph: $(BUILD)/libperiph.a
$(BUILD)/libperiph.a: $(periph_objs)
	$(AR) rcs $@ $^

# rtos
# ==================

rtos_srcs = $(wildcard $(SRCROOT)/rtos/*.c) $(wildcard $(SRCROOT)/rtos/*.s)
rtos_objs = $(patsubst $(SRCROOT)/rtos/%,$(BUILD)/rtos.dir/%.o,$(rtos_srcs))
rtos_deps = $(rtos_objs:.o=.d)

$(BUILD)/rtos.dir/%.c.o: $(SRCROOT)/rtos/%.c | $(BUILD)/rtos.dir
	$(CC) $(CFLAGS) -MMD -MP -MF $(@:.o=.d) -c $< -o $@

$(BUILD)/rtos.dir/%.s.o: $(SRCROOT)/rtos/%.s | $(BUILD)/rtos.dir
	$(AS) $(CFLAGS) -c $< -o $@

$(BUILD)/rtos.dir:
	mkdir -p $@

-include $(rtos_deps)

.PHONY: rtos
rtos: $(BUILD)/librtos.a
$(BUILD)/librtos.a: $(rtos_objs)
	$(AR) rcs $@ $^

# cmd
# ==================

cmd_srcs = $(wildcard $(SRCROOT)/cmd/*.c) $(wildcard $(SRCROOT)/cmd/*.s)
cmd_objs = $(patsubst $(SRCROOT)/cmd/%,$(BUILD)/cmd.dir/%.o,$(cmd_srcs))
cmd_deps = $(cmd_objs:.o=.d)

$(BUILD)/cmd.dir/%.c.o: $(SRCROOT)/cmd/%.c | $(BUILD)/cmd.dir
	$(CC) $(CFLAGS) -MMD -MP -MF $(@:.o=.d) -c $< -o $@

$(BUILD)/cmd.dir/%.s.o: $(SRCROOT)/cmd/%.s | $(BUILD)/cmd.dir
	$(AS) $(CFLAGS) -c $< -o $@

$(BUILD)/cmd.dir:
	mkdir -p $@

-include $(cmd_deps)

.PHONY: cmd
cmd: $(BUILD)/libcmd.a
$(BUILD)/libcmd.a: $(cmd_objs)
	$(AR) rcs $@ $^

# Main target
# ==================
srcs = $(wildcard $(SRCROOT)/src/*.c) $(wildcard $(SRCROOT)/src/*.s)
objs = $(patsubst $(SRCROOT)/src/%,$(BUILD)/$(TARGET).dir/%.o,$(srcs))
deps = $(objs:.o=.d)

$(BUILD)/$(TARGET).dir/%.c.o: $(SRCROOT)/src/%.c | $(BUILD)/$(TARGET).dir
	$(CC) $(CFLAGS) -MMD -MP -MF $(@:.o=.d) -c $< -o $@

$(BUILD)/$(TARGET).dir/%.s.o: $(SRCROOT)/src/%.s | $(BUILD)/$(TARGET).dir
	$(AS) $(CFLAGS) -c $< -o $@

$(BUILD)/$(TARGET).dir:
	mkdir -p $@

-include $(deps)

.PHONY: $(TARGET)
$(TARGET): $(BUILD)/$(TARGET).hex
$(BUILD)/$(TARGET).hex: $(objs) $(addprefix $(BUILD)/,$(patsubst -l%,lib%.a,$(LDLIBS)))
	$(CC) $(LDFLAGS) -o $(@:.hex=.elf) $(objs) $(LDLIBS)
	$(SIZE) $(@:.hex=.elf)
	$(OBJCOPY) -O ihex $(@:.hex=.elf) $@

# All
# ==================
.PHONY: all clean flash
all: $(TARGET)
clean:
	rm -rf $(BUILD)

flash: $(BUILD)/$(TARGET).hex
	openocd -f board/st_nucleo_f103rb.cfg -c "program $< verify reset exit"

$(BUILD):
	mkdir -p $@
