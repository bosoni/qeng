rem  parameters: bgfx/scripts/shader.mk

mkdir ..\dx9
mkdir ..\dx11
mkdir ..\glsl
mkdir ..\spirv

for %%f in (*_vs.sc) DO ( 
	if exist %%f (
@echo ===================== DX9 VERTEX =========================
shadercDebug.exe -f %%f -o %%f.bin --type vertex --platform windows --profile vs_3_0  -O 3  --varyingdef DEF.sc
move *.bin ..\dx9
@echo ===================== DX11 VERTEX =========================
shadercDebug.exe -f %%f -o %%f.bin --type vertex --platform windows --profile vs_5_0  -O 3  --varyingdef DEF.sc
move *.bin ..\dx11
@echo ===================== GL VERTEX =========================
shadercDebug.exe -f %%f -o %%f.bin --type vertex  -p 120  --varyingdef DEF.sc
move *.bin ..\glsl
@echo ===================== VULKAN VERTEX =========================
shadercDebug.exe -f %%f -o %%f.bin --type vertex --platform linux --profile spirv  -O 3  --varyingdef DEF.sc
move *.bin ..\spirv
	)
)

for %%f in (*_fs.sc) DO ( 
	if exist %%f (
@echo ===================== DX9 FRAGMENT =========================
shadercDebug.exe -f %%f -o %%f.bin --type fragment --platform windows  --profile ps_3_0  -O 3  --varyingdef DEF.sc
move *.bin ..\dx9
@echo ===================== DX11 FRAGMENT =========================
shadercDebug.exe -f %%f -o %%f.bin --type fragment --platform windows  --profile ps_5_0  -O 3  --varyingdef DEF.sc
move *.bin ..\dx11
@echo ===================== GL FRAGMENT =========================
shadercDebug.exe -f %%f -o %%f.bin --type fragment  -p 120  --varyingdef DEF.sc
move *.bin ..\glsl
@echo ===================== VULKAN FRAGMENT =========================
shadercDebug.exe -f %%f -o %%f.bin --type fragment --platform linux --profile spirv  -O 3  --varyingdef DEF.sc
move *.bin ..\spirv
	)
)

pause
