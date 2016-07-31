@echo off

set CompilerFlags=-Od -MTd -EHsc -Z7 -FC
set LinkerFlags=-incremental:no -opt:ref opengl32.lib user32.lib gdi32.lib winmm.lib

if not exist ..\build mkdir ..\build
pushd ..\build

del *.pdb > NUL 2> NUL

REM 64-bit dll
cl %CompilerFlags% -DAGL_EXPORT ..\code\win32_agl.cpp /LD /link %LinkerFlags%

REM 64-bit build
cl -I..\code\imgui %CompilerFlags% ..\code\game.cpp /link %LinkerFlags% win32_agl.lib

popd
