#include "screen_unrealthirdperson.h"

#include <raylib.h>
#include <time.h>
#define RLIGHTS_IMPLEMENTATION
#include <rlights.h>
#include <raymath.h>
#include <stdio.h>
#include <string.h>
#include "loader_unrealthirdperson.h"
#include "config.h"
#include "console.h"
#include "camera.h"
#include "input.h"
#include "control.h"
#include "assets.h"
#include "gameobject.h"
#include "screens.h"
#include "text.h"
#include "tick.h"
#include "console.h"
#include "lighting.h"
#include "render.h"

//----------------------------------------------------------------------------------
// Const
//----------------------------------------------------------------------------------

const int TICK_ANIMAT = 25;
const int TICK_INPUT = 120;
const int TICK_RENDER = 30;
const float MAX_WALK_SPEED = 0.08f;
const float MAX_WALK_ROTAT = 0.08f * 15;
const int CAM_DIST = 4;
const int CAM_HEIGHT = 1;
const Vector3 SCENE_FORWARD = {1, 0, 0};

// TODO: move to Load_LevelTree()
const Vector3 LIGHT_TRANSFORM = {0.0f, 9.0f, 39.0f};
const Color LIGHT_COLOR = {255, 255, 230, 255}; // YELLOW

//----------------------------------------------------------------------------------
// Vars
//----------------------------------------------------------------------------------

static bool postprocessing = false;

// TODO: move to Load_LevelTree()
static Camera camera;
static Model skybox;
static Shader light_shader = {0};
static Light light_point = {0};

static GameObject gos[LEVEL_SIZE];
static TickState animationTick = {0};
static TickState inputTick = {0};
static TickState renderTick = {0};
static InputActions actions;
static Controller playerController;
static ModelAnimation playerAnimations[2];

UnrealThirdPerson_State Init_UnrealThirdPerson(AppConfiguration appConfig, RenderTexture2D *target)
{
    char tmp[PATH_MAX];
    char tmp2[PATH_MAX];

    // init
    UnrealThirdPerson_State state = {0};
    state.showConsole = 0;
    state.appConfig = appConfig;
    camera = InitCamera();

    // SHADERS
    // TODO: move to Load_LevelTree()
    // SOURCE: https://www.raylib.com/examples/shaders/loader.html?name=shaders_postprocessing
    // shaders_postprocessing
    Shader shaderDefault = LoadShader(0, GetAssetPath(tmp, "resources/shaders/glsl%i/default.fs"));
    Shader shaderPostpro = LoadShader(0, GetAssetPath(tmp, "resources/shaders/glsl%i/bloom.fs"));
    postprocessing = appConfig.postpro_bloom_enable;
    state.postproShader = (postprocessing == true) ? shaderPostpro : shaderDefault;
    state.target = target;

    // GAME OBJECTS
    Load_LevelTree(gos);
    skybox = Load_LevelSkybox(LIGHT_COLOR, postprocessing);

    // ANIMATION
    // TODO: move to Load_LevelTree()
    int tmpAnimCount = 0;
    playerAnimations[0] = LoadModelAnimations(GetAssetPath(tmp, "resources/animations/Idle.m3d"), &tmpAnimCount)[0];
    playerAnimations[1] = LoadModelAnimations(GetAssetPath(tmp, "resources/animations/Running.m3d"), &tmpAnimCount)[0];
    state.animCurrentFrame = 0;

    // LIGHTING
    light_shader = LoadLighting();
    light_point = CreateLight(LIGHT_POINT, LIGHT_TRANSFORM, Vector3Zero(), LIGHT_COLOR, light_shader);
    for (int i = 0; i < LEVEL_SIZE; i++)
        SetModelLighting(gos[i].model, light_shader);

    // TICKS
    animationTick = InitTick(TICK_ANIMAT);
    inputTick = InitTick(TICK_INPUT);
    renderTick = InitTick(TICK_RENDER);
    StartTick(&animationTick);
    StartTick(&inputTick);
    StartTick(&renderTick);

    // PLAYER
    // TODO: move to Load_LevelTree()
    playerController = (Controller){
        (Vector3){9.0f, 0.0f, 11.0f},
        SCENE_FORWARD,
    };
    InitInputActions(&actions);

    return state;
}

void UpdateRender()
{
    UpdateLighting(light_shader, camera);
}

void UpdatePlayerAnimation(UnrealThirdPerson_State *state)
{
    if (!IsTickUpdate(&animationTick))
        return;
    else
        UpdateTick(&animationTick);
    ModelAnimation anim = state->animIndex == 0 ? playerAnimations[0] : playerAnimations[1];
    int animationFPS = 25; // Blender export
    int HACK = 3;          // FIXME
    int frameInMs = TIME_1_SECOND / animationFPS;
    int tickInMs = TIME_1_SECOND / animationTick.rateInHz;
    int frames = tickInMs / frameInMs;
    state->animCurrentFrame = (state->animCurrentFrame + frames * HACK) % anim.frameCount;
    UpdateModelAnimation(gos[0].model, anim, state->animCurrentFrame);
    UpdateModelAnimation(gos[12].model, anim, state->animCurrentFrame);
}

void UpdatePlayerCamera()
{
    FixedCamera_Look(&camera, playerController, CAM_DIST, CAM_HEIGHT);
}

void UpdatePlayerPosition(UnrealThirdPerson_State *state)
{
    // player
    gos[0].transform.translation = (Vector3){
        playerController.position.x,
        playerController.position.y,
        playerController.position.z};
    gos[0].transform.rotation = (Rotation2){
        ROTATION_YAW,
        TankControl_ModelRotationAngle(playerController.direction)};
    // shadow
    gos[12].transform.translation = (Vector3){
        playerController.position.x,
        playerController.position.y + 0.01f,
        playerController.position.z};
    gos[12].transform.rotation = gos[0].transform.rotation;
}

void SetupPlayerInputComponent(UnrealThirdPerson_State *state, InputActions *actions)
{
    // console
    if (actions->ConsoleAction.State.Completed == true)
    {
        state->showConsole = !state->showConsole;
        actions->ConsoleAction.State.Completed = false;
    }
    // TODO: Jumping
    // Moving
    if (actions->MoveAction.State.Triggered == true)
        TankControl_Move(&playerController.position, actions->MoveAction.Value, MAX_WALK_SPEED, MAX_WALK_ROTAT);
    // TODO: Looking
}

void SetupPlayerAnimation(UnrealThirdPerson_State *state, InputActions *actions)
{
    state->animIndex = 0;
    if (actions->MoveAction.State.Triggered == true)
        state->animIndex = 1;
}

void UpdatePlayerInput(UnrealThirdPerson_State *state)
{
    if (!IsTickUpdate(&inputTick))
        return;
    else
        UpdateTick(&inputTick);
    ExecuteInputActions(&actions);
    SetupPlayerInputComponent(state, &actions);
    SetupPlayerAnimation(state, &actions);
}

void UpdatePhysics(UnrealThirdPerson_State *state)
{
    // TODO: https://www.raylib.com/examples/models/loader.html?name=models_box_collisions
}

int Update_UnrealThirdPerson(UnrealThirdPerson_State *state)
{
    // tick
    UpdatePlayerInput(state);
    UpdatePlayerPosition(state);
    UpdatePlayerAnimation(state);
    UpdatePlayerCamera();
    // TODO: UpdatePhysics(state);
    // no tick
    UpdateRender();
    if (IsKeyPressed(KEY_TAB))
    {
        return MENU;
    }
    return GAMEPLAY;
}

void Draw_3D_Console(UnrealThirdPerson_State *state)
{
}

void Draw_2D(UnrealThirdPerson_State *state)
{
    if (state->showConsole == 1)
    {
        // LogConsole(TextFormat("animationTick.current/lastUpdate: %i %i", animationTick.current, animationTick.lastUpdate));
        LogConsole(TextFormat("playerController.direction.x,y,z: %f %f %f", playerController.direction.x, playerController.direction.y, playerController.direction.z));
        DrawConsole();
    }
    else
    {
        DrawText("Press [TAB] to toggle menu", 10, 10 + 30 * 0, 20, GRAY);
        DrawText("Use keys [W][A][S][D][SPACE] or arrows to move character", 10, 10 + 30 * 1, 20, GRAY);
        DrawText("Press [F1] to toggle console", 10, 10 + 30 * 2, 20, GRAY);
    }
    if (state->appConfig.fps_counter_show == true)
    {
        DrawFPS(state->appConfig.screen_width - 100, 15);
    }
}

void Draw_3D_Models(UnrealThirdPerson_State *state)
{
    BeginMode3D(camera);
    {
        if (state->showConsole == 1)
        {
            for (size_t i = 0; i < LEVEL_SIZE; i++)
            {
                Draw_GameObject(gos[i]);
                Draw_GameObject_Console(gos[i]);
            }
            DrawGrid(50, 1.0f);
            Draw_GameObject_PlayerHitBox(gos[0]);
            // TODO: move to Load_LevelTree()
            // light spot
            DrawCubeWiresV(LIGHT_TRANSFORM, (Vector3){1.0f, 1.0f, 1.0f}, YELLOW);
        }
        else
            for (size_t i = 0; i < LEVEL_SIZE; i++)
                Draw_GameObject(gos[i]);
    }
    EndMode3D();
}

void Draw_Pipeline_Default(UnrealThirdPerson_State *state, RenderTexture2D *target)
{
    // Stage 1/1 Draw
    BeginDrawing();
    {
        ClearBackground(RAYWHITE);

        Draw_3D_Skybox(skybox, camera);
        Draw_3D_Models(state);
        Draw_2D(state);
    }
    EndDrawing();
}

void Draw_Pipeline_PostProcessing(UnrealThirdPerson_State *state, RenderTexture2D *target)
{
    // Stage 1/2 Render
    BeginTextureMode(*target);
    {
        ClearBackground(RAYWHITE);

        Draw_3D_Skybox(skybox, camera);
        Draw_3D_Models(state);
    }
    EndTextureMode();
    // Stage 2/2 Draw
    BeginDrawing();
    {
        ClearBackground(RAYWHITE);

        Draw_PostProcessing(state->postproShader, target);
        Draw_2D(state);
    }
    EndDrawing();
}

void Draw_UnrealThirdPerson(UnrealThirdPerson_State *state, RenderTexture2D *target)
{
    if (postprocessing == true)
    {
        Draw_Pipeline_PostProcessing(state, target);
        return;
    }
    Draw_Pipeline_Default(state, target);
}

void Unload_UnrealThirdPerson(UnrealThirdPerson_State *state)
{
    // skybox
    UnloadSkybox(skybox);
    // animations
    UnloadModelAnimations(playerAnimations, sizeof(playerAnimations));
    // level
    for (int i = 1; i < sizeof(gos); i++)
        UnloadModel(gos[i].model);
    // shaders
    UnloadShader(light_shader);
    // TODO: UnloadShader() shaders
}
