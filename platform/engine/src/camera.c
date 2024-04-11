#include "camera.h"

#include <raylib.h>

Camera InitCamera()
{
    Camera camera = {0};
    camera.position = (Vector3){0.5f, 0.5f, 1.0f}; // Camera position
    camera.target = (Vector3){0.0f, 0.25f, 0.0f};  // Camera looking at point
    camera.up = (Vector3){0.0f, 0.75f, 0.0f};      // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                           // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;        // Camera projection type
    return camera;
}

void CameraFixed_Look(Camera *camera, Controller player, int cameraDistance, int cameraHeight)
{
    camera->position = (Vector3){
        player.position.x - (player.direction.x * cameraDistance),
        player.position.y + cameraHeight,
        player.position.z - (player.direction.z * cameraDistance)};
    camera->target = (Vector3){
        player.position.x,
        player.position.y + cameraHeight,
        player.position.z};
}

void CameraFollow_Look(Camera *camera, Controller player, int cameraDistance, int cameraHeight)
{
    // TODO
}
