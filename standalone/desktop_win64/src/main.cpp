#include <windows.h>
#include <shaders_postprocessing.h>

#include <stdio.h>
#include <stdlib.h>

#include <string>

#define GLSL_VERSION 330 // PLATFORM_DESKTOP

const char* getRespath();

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpCmdLine, int nCmdShow)
{

    std::string str = _pgmptr;
    int exepath_len = str.size();
    std::string exebin = "desktop_win64.exe";
    int exebin_len = exebin.size();
    std::string str2 = str.substr(0, exepath_len - exebin_len);
    const char *cstr = str2.c_str();

    AppProperties appProperties;
    strcpy(appProperties.res_path, cstr);
    appProperties.msaa_enable = false;
    appProperties.bloom_enable = false;
    appProperties.glsl_version = GLSL_VERSION;
    appProperties.fps_cap = 60;

    return main(appProperties);
}

const char* getRespath() {
    std::string str = _pgmptr;
    int exepath_len = str.size();
    std::string exebin = "desktop_win64.exe";
    int exebin_len = exebin.size();
    std::string str2 = str.substr(0, exepath_len - exebin_len);
    const char *cstr = str2.c_str();
    return cstr;
}