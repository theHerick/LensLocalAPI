@echo off
title LensLocalAPI - Google Lens & Firebase Gateway
color 0A

echo ======================================================================
echo           LENSLOCALAPI - INICIALIZADOR DE APLICAÇÃO
echo ======================================================================
echo.

taskkill /F /IM LensLocalAPI.exe >nul 2>&1

dotnet --version >nul 2>&1
if %errorlevel% neq 0 (
    color 0C
    echo [ERRO] .NET SDK nao foi encontrado no sistema!
    echo Por favor, instale o .NET 8.0 ou superior para executar.
    echo.
    pause
    exit /b 1
)

echo [1/3] Compilando aplicacao LensLocalAPI (.NET WPF)...
dotnet build
if %errorlevel% neq 0 (
    color 0C
    echo [ERRO] Falha ao compilar o projeto.
    pause
    exit /b 1
)
echo [OK] Compilacao concluida com sucesso!
echo.

echo [2/3] Verificando navegadores do Microsoft Playwright...
powershell.exe -ExecutionPolicy Bypass -File bin/Debug/net10.0-windows/playwright.ps1 install chromium >nul 2>&1
echo [OK] Chromium pronto para automacao!
echo.

echo [3/3] Iniciando a aplicacao LensLocalAPI...
echo ======================================================================
echo  LensLocalAPI iniciado com sucesso.
echo ======================================================================
echo.

dotnet run --no-build
