rem Usage: geometryc -f <in> -o <out>
rem  -f <file path>           Input file path.
rem  -o <file path>           Output file path.
rem  -s, --scale <num>        Scale factor.
rem      --ccw                Front face is counter-clockwise winding order.
rem      --flipv              Flip texture coordinate V.
rem      --obb <num>          Number of steps for calculating oriented bounding box.
rem           Default value is 17. Less steps less precise OBB is.
rem           More steps slower calculation.
rem      --packnormal <num>   Normal packing.
rem           0 - unpacked 12 bytes (default).
rem           1 - packed 4 bytes.
rem      --packuv <num>       Texture coordinate packing.
rem           0 - unpacked 8 bytes (default).
rem           1 - packed 4 bytes.
rem      --tangent            Calculate tangent vectors (packing mode is the same as normal).
rem      --barycentric        Adds barycentric vertex attribute (packed in bgfx::Attrib::Color1).
rem  -c, --compress           Compress indices.
rem      --[l/r]h-up+[y/z]   Coordinate system. Default is '--lh-up+y' Left-Handed +Y is up.
rem ------------------------------------------------------------------------------------------

for %%f in (*.glb) DO (
	if exist %%f (
rem		geometrycDebug.exe -f %%f  -o %%f.mesh
	)
)

for %%f in (*.gltf) DO (
	if exist %%f (
rem		geometrycDebug.exe -f %%f  -o %%f.mesh
	)
)

for %%f in (*.obj) DO (
	if exist %%f (
		geometrycDebug.exe -f %%f  -o %%f.mesh --flipv
	)
)

for %%f in (*.obj) DO (
	if exist %%f (
		geometrycDebug.exe -f %%f  -o %%f_nm.mesh --tangent --flipv
	)
)

pause
