all: 
	gcc -g -I src/headers src/*/*.c -o output/main

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

# gcc -g -fsanitize=address -I src/headers src/main.c src/lex/*.c src/lex/token/*.c src/sem/*.c src/util/*.c -o output/main_asan