@echo off
echo ====================================
echo  GLCanvasApp Deployment Script
echo ====================================

REM Create deployment directory
if exist "GLCanvasApp-Release" rmdir /s /q "GLCanvasApp-Release"
mkdir "GLCanvasApp-Release"

REM Copy executable
echo Copying executable...
copy "build\Release\GLCanvasApp.exe" "GLCanvasApp-Release\"

REM Copy icon resource
echo Copying resources...
copy "build\Release\button.png" "GLCanvasApp-Release\"

REM Copy wxWidgets DLLs
echo Copying wxWidgets DLLs...
if exist "%WXWIN%\wxMSW-3.3.0_vc14x_x64_ReleaseDLL\lib\vc14x_x64_dll\*.dll" (
    copy "%WXWIN%\wxMSW-3.3.0_vc14x_x64_ReleaseDLL\lib\vc14x_x64_dll\*.dll" "GLCanvasApp-Release\"
) else (
    echo Warning: wxWidgets DLLs not found. You may need to copy them manually.
)

REM Copy documentation
echo Copying documentation...
copy "README.md" "GLCanvasApp-Release\"

REM Create info file
echo Creating deployment info...
echo GLCanvasApp - Interactive OpenGL Canvas > "GLCanvasApp-Release\INFO.txt"
echo Built on %DATE% at %TIME% >> "GLCanvasApp-Release\INFO.txt"
echo. >> "GLCanvasApp-Release\INFO.txt"
echo To run: Double-click GLCanvasApp.exe >> "GLCanvasApp-Release\INFO.txt"
echo. >> "GLCanvasApp-Release\INFO.txt"
echo Features: >> "GLCanvasApp-Release\INFO.txt"
echo - OpenGL rendering with interactive triangle >> "GLCanvasApp-Release\INFO.txt"
echo - Click top-left button to toggle control panel >> "GLCanvasApp-Release\INFO.txt"
echo - Use slider to rotate triangle >> "GLCanvasApp-Release\INFO.txt"
echo - Use checkbox to show/hide triangle >> "GLCanvasApp-Release\INFO.txt"

echo.
echo ====================================
echo  Deployment Complete!
echo ====================================
echo Package created in: GLCanvasApp-Release\
echo.
echo Contents:
dir "GLCanvasApp-Release"

pause 