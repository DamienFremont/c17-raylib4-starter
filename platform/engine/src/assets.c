#include "assets.h"

#include <raylib.h>

static char RESOURCES_PATH[PATH_MAX];
static int GLSL_VERSION = 330;

void InitAssets(char *path, int glsl_ver)
{
    strcpy(RESOURCES_PATH, path);
    GLSL_VERSION = glsl_ver;
}

char *GetAssetPath(char *tmp, char *assetpath)
{
    JoinPath(tmp, RESOURCES_PATH, TextFormat(assetpath, GLSL_VERSION));
    return tmp;
}

// TODO
// char *GetGLSLVersionPath(char *tmp, char *assetpath)
// {
//     JoinPath(tmp, RESOURCES_PATH, TextFormat(assetpath, GLSL_VERSION));
//     return tmp;
// }

char *JoinPath(char *dest, char *path1, char *path2)
{
    // TODO: end with or start with '/'
    const char tmp[PATH_MAX];
    snprintf(tmp, sizeof(tmp), "%s/%s", path1, path2);
    strcpy(dest, tmp);
    return dest;
}