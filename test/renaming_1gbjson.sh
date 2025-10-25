#!/bin/bash

# Caminho para o executável
EXEC="../build/rr"

# Teste de substituição de aspas em 1GB
echo 'Substituindo " por '\'' em movies1gb.json'
TIME=$( (time $EXEC '"' "'" movies1gb.json) 2>&1 | grep real )
echo "Tempo: $TIME"

