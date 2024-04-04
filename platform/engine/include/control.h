#include "input.h"

#pragma once

#include "control.h"
#include "gameobject.h"

typedef struct Controller
{
    Vector3 position;
    Vector3 direction;
} Controller;

void TankControl_Move(Controller *playerGO, InputActionValue value, float char_speed, float char_rot);
