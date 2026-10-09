PROJ_NAME=ypsilon
BUILD_DIR=output
SRC := $(shell find src -name '*.c' | sort)
TARGET=$(BUILD_DIR)/$(PROJ_NAME)

CC=gcc
CFLAGS=-Wall -Wextra -Werror -g -fdiagnostics-color=always -I ./src/headers
LDFLAGS=-lm

ALL_DEPS := $(shell find src src/headers -type f \( -name '*.c' -o -name '*.h' \) | sort)

all: $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(TARGET): $(SRC) $(ALL_DEPS) | $(BUILD_DIR)
	@echo "Running: " 
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

ARG := $(word 2, $(MAKECMDGOALS))
FLAG := $(word 3, $(MAKECMDGOALS))

ifneq ($(ARG),)
  $(eval $(ARG):;@:)
endif

ifneq ($(FLAG),)
  $(eval $(FLAG):;@:)
endif

DBG_FLAG :=
FORCE_BUILD :=
ifneq ($(filter dbg -dbg,$(FLAG)),)
  DBG_FLAG := -dbg
  FORCE_BUILD := -B
endif

define exec
	@echo "\n\e[0;32m=> Compiling <= \e[m"
	$(MAKE) $(FORCE_BUILD)
	@echo "\n\e[1;32m=> Starting test in: $(1) <=\e[m\n" 
	$(TARGET) samples/$(1) $(DBG_FLAG)
	@echo "\n\n\e[1;32m=> Test finished in: $(1) <=\e[m\n" 
endef

test:
ifeq ($(ARG),variable)
	$(call exec,variable.y)
else ifeq ($(ARG),if)
	$(call exec,if.y)
else ifeq ($(ARG),while)
	$(call exec,while.y)
else ifeq ($(ARG),call_function)
	$(call exec,call_function.y)
else ifeq ($(ARG),define_function)
	$(call exec,define_function.y)
else
	@echo "\n\e[1;33mInválid option\e[m"
	@echo "\e[1;33mTry: make test <name_of_test>\e[m\n"
endif

