all: 
	gcc -g -I src/headers \
      src/main.c \
      src/lex/lex.c \
      src/lex/queue.c \
      src/lex/token/token.c \
      src/lex/token/token_types.c \
      src/sem/sem.c \
      src/sem/sem_types.c \
      src/util/file_reader.c \
      src/util/utils.c \
      -o output/main

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
	make && output/main samples/$(1) $(DBG_FLAG)
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
