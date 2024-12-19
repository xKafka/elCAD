@echo off
set GLSLC_PATH=C:/VulkanSDK/1.3.283.0/Bin/glslc.exe

echo Compiling shaders...

for %%f in (*.vert *.frag *.comp *.geom *.tesc *.tese) do (
    echo Compiling %%f...
    %GLSLC_PATH% %%f -o %%~nf.spv
    if %ERRORLEVEL% neq 0 (
        echo Failed to compile %%f!
        exit /b 1
    )
)

echo All shaders compiled successfully!
pause
