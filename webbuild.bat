@echo off
mkdir ATLAS-HUGGED-WEB
emcc -o index.html Test\GameControls.cpp Test\LevelObj.cpp Test\Main.cpp Test\Player.cpp Test\PowerUp.cpp Test\screen_gameover.cpp Test\screen_gameplay.cpp Test\screen_title.cpp Test\World.cpp -Os -Wall %RAYLIB_DIR%\src\libraylib.a %BATTY_ENGINE_DIR%\build\battyenginelib.a -I%RAYLIB_DIR%\src -I%BATTY_ENGINE_DIR%\src -sMAX_WEBGL_VERSION=2 -s USE_GLFW=3 -s EXPORTED_FUNCTIONS="['_malloc','_main']" -DPLATFORM_WEB --preload-file 2k_earth_daymap.png --preload-file resources -s TOTAL_MEMORY=1073741824
Copy index.data ATLAS-HUGGED-WEB
Copy theindex.html ATLAS-HUGGED-WEB\index.html
Copy index.js ATLAS-HUGGED-WEB
Copy index.wasm ATLAS-HUGGED-WEB