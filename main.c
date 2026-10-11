#include <OCTAVIANEngine.h>

#define COLOR_WHITE ((OCT_vec4){1.0f, 1.0f, 1.0f, 1.0f})
#define REGULAR_DIMS ((OCT_vec2){100, 100})
#define BIG_DIMS ((OCT_vec2){300, 300})
#define UV_WHOLE ((OCT_vec4){0.0f, 0.0f, 1.0f, 1.0f})
#define IMPULSE 3000
#define FORCE 5000

static void floorAnyaCollisionCallback(OCT_local floor, OCT_local collidedWith, OCT_collision_states state, void* anyaColl);

int main(void) {
    OCT_platform_setCWD(GAME_CWD);

    OCT_engine_init();

    // Engine config
    OCT_config_physics physConfig = {
        .gravity = (OCT_vec2){0, -3000},
        .unitsPerMeter = 100
    };
    OCT_physicsSystem_config(physConfig);
    // End engine config


    // Texture setup
    OCT_global anyaTexGroup = OCT_textureGroup_open((OCT_vec2){272, 321}, 15);
    OCT_global hannesTexGroup = OCT_textureGroup_open((OCT_vec2){505, 595}, 15);
    OCT_global anyaTex = OCT_texture_new(anyaTexGroup, "sprites/anya.png");
    OCT_global hannesTex = OCT_texture_new(hannesTexGroup, "sprites/hannes.png");
    // End texture setup


    // Entity init
        // A context's root serves as an anchor for all other entities in the context
    OCT_local mainRoot;
    OCT_global mainContext = OCT_entityContext_open(&mainRoot);

    OCT_local anya = OCT_entity_new(mainContext);
    OCT_transform2D_attach(anya, mainRoot);
    OCT_sprite2D_attach(anya, anyaTex, UV_WHOLE, REGULAR_DIMS, 2);
    OCT_physics2D_attachNew(anya, 10, true);
    OCT_physics2D_lockRotation(anya, 0);
    OCT_local anyaColl = OCT_collider2D_new(anya, OCT_shapeType_rect2, REGULAR_DIMS, OCT_VEC2_ZERO, 0, 1);
    OCT_camera2D_attach(anya, OCT_VEC2_ZERO, 0, (OCT_vec2){3200, 1800});

    OCT_local floor = OCT_entity_new(mainContext);
    OCT_transform2D_attach(floor, mainRoot);
    OCT_transform2D_moveBy(floor, (OCT_vec2){0, -500});
    OCT_sprite2D_attach(floor, hannesTex, UV_WHOLE, (OCT_vec2){20000, 300}, 2);
    OCT_physics2D_attachNew(floor, 10, false);
    OCT_local floorColl = OCT_collider2D_new(floor, OCT_shapeType_rect2, (OCT_vec2){20000, 300}, OCT_VEC2_ZERO, 0, 10);

    OCT_collider2D_watch(floorColl, floorAnyaCollisionCallback, &anyaColl);
    // End entity setup


    // Window setup
    OCT_global mainWindow = OCT_window_open("This terrible test game", 1920, 1080, (OCT_vec4){0.0f, 0.0f, 0.0f, 1.0f});
    OCT_window_setScreenSpaceZoom(mainWindow, (OCT_vec2){1000, 1000});
    OCT_camera2D_displayTo(anya, mainWindow);
    // End window setup


    while (OCT_window_anyOpen()) {
        OCT_engine_frame_start();

        if (OCT_button_read(OCT_KEY_A, NULL, NULL) == OCT_BUTTONSTATE_PRESSED) {
            OCT_vec2 oldVelocity = OCT_physics2D_read(anya);
            OCT_physics2D_setVelocity(anya, (OCT_vec2){0, oldVelocity.y});
            OCT_physics2D_addImpulse(anya, (OCT_vec2){-IMPULSE, 0});
        }
        if (OCT_button_read(OCT_KEY_D, NULL, NULL) == OCT_BUTTONSTATE_PRESSED) {
            OCT_vec2 oldVelocity = OCT_physics2D_read(anya);
            OCT_physics2D_setVelocity(anya, (OCT_vec2){0, oldVelocity.y});
            OCT_physics2D_addImpulse(anya, (OCT_vec2){IMPULSE, 0});
        }
        if (OCT_button_read(OCT_KEY_A, NULL, NULL) == OCT_BUTTONSTATE_HELD) {
            OCT_physics2D_addForce(anya, (OCT_vec2){-FORCE, 0.0f});
        }
        if (OCT_button_read(OCT_KEY_D, NULL, NULL) == OCT_BUTTONSTATE_HELD) {
            OCT_physics2D_addForce(anya, (OCT_vec2){FORCE, 0.0f});
        }
        if (OCT_button_read(OCT_KEY_SPACE, NULL, NULL) == OCT_BUTTONSTATE_PRESSED) {
            OCT_physics2D_addImpulse(anya, (OCT_vec2){0, IMPULSE});
        }

        OCT_engine_updateContext(mainContext);
        OCT_engine_frame_show();
    }

    OCT_engine_shutdown();
    return 0;
}

static void floorAnyaCollisionCallback(OCT_local floor, OCT_local collidedWith, OCT_collision_states state, void* anyaColl) {
    if (state == OCT_COLLISION_COLLIDED) {
        if (OCT_local_isEqual(*(OCT_local*)anyaColl, collidedWith )) {
            printf("I think that was Anya. Ow.\n");
        }
    }
}