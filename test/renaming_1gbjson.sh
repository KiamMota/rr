#!/bin/bash

# Caminho para o executável
EXEC="../build/rr"

# Teste de renomeação massiva em 1GB
echo "'{' to '{&' in movies1gb.json"
TIME=$( (time $EXEC '{' '{&' movies1gb.json) 2>&1 | grep real )
echo "Tempo: $TIME"

