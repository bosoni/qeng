rem  parameters: bgfx/scripts/shader.mk

mkdir ..\dx9
mkdir ..\dx11
mkdir ..\glsl
mkdir ..\spirv

for %%f in (*_vs.sc) DO ( 
	if exist %%f (
@echo ===================== DX11 VERTEX =========================
shadercDebug.exe -f %%f -o %%f.bin --type vertex --platform windows --profile vs_5_0  -O 3  --varyingdef DEF.sc
move *.bin ..\dx11
	)
)

for %%f in (*_fs.sc) DO ( 
	if exist %%f (
@echo ===================== DX11 FRAGMENT =========================
shadercDebug.exe -f %%f -o %%f.bin --type fragment --platform windows  --profile ps_5_0  -O 3  --varyingdef DEF.sc
move *.bin ..\dx11
	)
)

pause
