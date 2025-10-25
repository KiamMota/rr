#!/bin/bash

# Caminho para o executável
EXEC="../build/rr"

# Primeiro teste
echo "1: a b moviesjson10mb.json"
TIME1=$( (time $EXEC a b moviesjson10mb.json) 2>&1 | grep real )
echo "Tempo Teste 1: $TIME1"

# Segundo teste (argumentos invertidos)
echo "2: b a moviesjson10mb.json"
TIME2=$( (time $EXEC b a moviesjson10mb.json) 2>&1 | grep real )
echo "Tempo Teste 2: $TIME2"

