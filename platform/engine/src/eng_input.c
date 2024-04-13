#include "eng_input.h"

#include <stdbool.h>

//---------------------------------------------------------
// Local Functions Declaration
//---------------------------------------------------------

bool MoveAction(InputActions *actions);
bool ConsoleAction(InputActions *out);

//---------------------------------------------------------
// Module specific Functions Definition
//---------------------------------------------------------

void InitInputActions(InputActions *actions)
{
    actions->ConsoleAction = (InputAction){(InputActionState){false, false, false}, 0};
    actions->MoveAction = (InputAction){0, 0};
}

void ExecuteInputActions(InputActions *actions)
{
    ConsoleAction(actions);
    MoveAction(actions);
    // TODO: JumpAction(&out);
    // TODO: LookAction(&out);
}

//---------------------------------------------------------
// Local Functions Definition
//---------------------------------------------------------

bool MoveAction(InputActions *actions)
{
    const InputActionValue axis2D = (InputActionValue){
        0, // NULL
        0, // NULL
        (Vector2){0, 0}};
    actions->MoveAction.Value = axis2D;
    actions->MoveAction.State.Triggered = false;
    // input
    bool up = IsKeyDown(KEY_W) || IsKeyDown(KEY_UP);
    bool down = IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN);
    bool left = IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT);
    bool right = IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT);
    // action
    if (left || right || up || down)
        actions->MoveAction.State.Triggered = true;
    // X
    if (left)
        actions->MoveAction.Value.Axis2D.x = -1;
    else if (right)
        actions->MoveAction.Value.Axis2D.x = 1;
    // Y
    if (up)
        actions->MoveAction.Value.Axis2D.y = 1;
    else if (down)
        actions->MoveAction.Value.Axis2D.y = -1;
    // find out which way is forward
    // const FRotator Rotation = Controller->GetControlRotation();
    // const FRotator YawRotation(0, Rotation.Yaw, 0);
    // float ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
    // float RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
}

bool ConsoleAction(InputActions *out)
{
    if (IsKeyDown(KEY_F1) && !out->ConsoleAction.State.Started)
    {
        out->ConsoleAction.State.Started = true;
        out->ConsoleAction.State.Completed = false;
    }
    if (IsKeyUp(KEY_F1) && out->ConsoleAction.State.Started)
    {
        out->ConsoleAction.State.Started = false;
        out->ConsoleAction.State.Completed = true;
    }
}
