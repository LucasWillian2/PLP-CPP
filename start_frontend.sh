#!/bin/bash

echo "========================================"
echo "   PLANNER VIRTUAL - FRONTEND WEB"
echo "========================================"
echo

echo "Verificando dependências Python..."
if ! command -v python3 &> /dev/null; then
    echo "ERRO: Python não encontrado!"
    echo "Por favor, instale Python 3.8 ou superior"
    exit 1
fi

echo "Instalando dependências..."
pip3 install -r requirements.txt

echo
echo "Iniciando servidor da API..."
echo "Frontend disponível em: http://localhost:5000"
echo
echo "Pressione Ctrl+C para parar o servidor"
echo

python3 api/server.py
