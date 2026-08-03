all: 
	gcc -g src/main.c -o output/main

ARG := $(word 2, $(MAKECMDGOALS))

ifneq ($(ARG),)
  $(eval $(ARG):;@:)
endif

run:
ifeq ($(ARG),1)
	@echo "=> Iniciando teste em: variable.y" 
	output/main samples/variable.y
	@echo "=> Teste finalizando em: variable.y" 
else ifeq ($(ARG),2)
	@echo "=> Iniciando teste em: if.y" 
	output/main samples/if.y
	@echo "=> Teste finalizando em: if.y" 
else ifeq ($(ARG),3)
	@echo "=> Iniciando teste em: while.y" 
	output/main samples/while.y
	@echo "=> Teste finalizando em: while.y" 
else ifeq ($(ARG),4)
	@echo "=> Iniciando teste em: call_function.y" 
	output/main samples/call_function.y
	@echo "=> Teste finalizando em: call_function.y" 
else ifeq ($(ARG),5)
	@echo "=> Iniciando teste em: define_function.y" 
	output/main samples/define_function.y
	@echo "=> Teste finalizando em: define_function.y" 
else
	@echo "Opção inválida ou ausente!"
	@echo "Uso correto: make run [1|2|3]"
endif
