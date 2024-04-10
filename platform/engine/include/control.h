#include "input.h"
#include "gameobject.h"

#pragma once

typedef struct Controller
{
    Vector3 position;
    Vector3 direction;
} Controller;

void TankControl_Move(Controller *player, InputActionValue value, float char_speed, float char_rot);

float TankControl_ModelRotationAngle(Vector3 playerDirection);
