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
ifneq ($(filter dbg -dbg,$(FLAG)),)
  DBG_FLAG := -dbg
endif

define exec
	@echo "=> Iniciando teste em: $(1)" 
	$(MAKE) && $(TARGET) samples/$(1) $(DBG_FLAG)
	@echo "=> Teste finalizando em: $(1)" 
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
	@echo "Inválid option"
	@echo "Try: make test <name_of_test>"
endif

