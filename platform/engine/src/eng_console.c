#include "eng_console.h"

#include <stdbool.h>
#include <raylib.h>

//---------------------------------------------------------
// Types and Structures Definition
//---------------------------------------------------------

static char consoleOut[999];

//---------------------------------------------------------
// Module specific Functions Definition
//---------------------------------------------------------

void LogConsole(char* str)
{
    strcpy(consoleOut, str);
}

void DrawConsole()
{
    const int LINE_HEIGHT = 20;
    const int LINE_1 = 0;
    const int LINE_2 = 1;
    const int LINES_COUNT = 4;

    // Init
    const char line1[999] = "Console:";
    const char line2[999];
    snprintf(line2, sizeof(line2), "> %s", consoleOut);
    // Draw
    DrawRectangle(0, 0, GetScreenWidth(), LINE_HEIGHT * LINES_COUNT, BLACK);
    DrawText(line1, 10, 20 + LINE_HEIGHT * LINE_1, 10, WHITE);
    DrawText(line2, 10, 20 + LINE_HEIGHT * LINE_2, 10, WHITE);

    DrawGrid(50, 1.0f);
    DrawCubeWiresV((Vector3){9.0f, 1.0f, 11.0f}, (Vector3){1.0f, 2.0f, 1.0f}, RED);
}
