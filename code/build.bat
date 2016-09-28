@echo off

set CompilerFlags=-Od -MTd -EHsc -Z7 -FC -D"UNICODE"
set LinkerFlags=-incremental:no -opt:ref opengl32.lib user32.lib gdi32.lib winmm.lib

if not exist ..\build mkdir ..\build
pushd ..\build

del *.pdb > NUL 2> NUL

REM 64-bit dll
REM cl %CompilerFlags% -DAGL_EXPORT ..\code\win32_agl.cpp /LD /link %LinkerFlags%

REM 64-bit build
REM cl %CompilerFlags% ..\code\game.cpp /link %LinkerFlags% win32_agl.lib %SuppressConsole%

REM cl %CompilerFlags% ..\code\model_editor.cpp /link %LinkerFlags%

cl %CompilerFlags% ..\code\test.cpp /link %LinkerFlags%
REM cl %CompilerFlags% ..\code\new_test.cpp /link %LinkerFlags%
cl %CompilerFlags% ..\code\init_test.cpp /link %LinkerFlags%
popd
