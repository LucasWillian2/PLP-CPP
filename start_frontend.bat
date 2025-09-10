@echo off
echo ========================================
echo    PLANNER VIRTUAL - FRONTEND WEB
echo ========================================
echo.

echo Verificando dependencias Python...
python --version >nul 2>&1
if errorlevel 1 (
    echo ERRO: Python nao encontrado!
    echo Por favor, instale Python 3.8 ou superior
    pause
    exit /b 1
)

echo Instalando dependencias...
pip install -r requirements.txt

echo.
echo Iniciando servidor da API...
echo Frontend disponivel em: http://localhost:5000
echo.
echo Pressione Ctrl+C para parar o servidor
echo.

python api/server.py

pause
