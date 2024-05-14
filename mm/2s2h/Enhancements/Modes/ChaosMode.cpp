#include <libultraship/bridge.h>
#include "Enhancements/GameInteractor/GameInteractor.h"
#include "ChaosMode.h"

extern "C" {
#include <variables.h>
#include "functions.h"
}

typedef struct EnemyEntry {
    int16_t id;
    int16_t params;
} EnemyEntry;

static EnemyEntry enemySpawnTable[RANDOMIZED_ENEMY_SPAWN_TABLE_SIZE] = {
    { ACTOR_EN_OKUTA, 0 },   // Oktorok
    { ACTOR_EN_WALLMAS, 0 }, // Wallmaster
    { ACTOR_EN_DODONGO, 0 }, // Dodongo
    { ACTOR_EN_FIREFLY, 0 }, // Keese Fire
    { ACTOR_EN_FIREFLY, 3 },  // Keese Normal
    { ACTOR_EN_FIREFLY, 4 },  // Keese Ice
    { ACTOR_EN_TITE, -4 },   // Tektite
    { ACTOR_EN_PEEHAT, 0 },  // Peahat
    { ACTOR_EN_DINOFOS, 0 }, // Dinolfos
    { ACTOR_EN_BUBBLE, 0 },  // Shabom
    { ACTOR_EN_ST, 0 },      // Large Skulltula
    { ACTOR_EN_FAMOS, 0 },   // Death Armos (Inv. Stone Tower)
    { ACTOR_EN_AM, 0 },      // Armos
    { ACTOR_EN_DEKUBABA, 0 }, // Deku Baba
    { ACTOR_EN_DEKUNUTS, 0 }, // Mad Scrub
    { ACTOR_EN_BB, 0 },       // Blue Bubble
    { ACTOR_EN_VM, 0 },       // Beamos
    { ACTOR_EN_FLOORMAS, 0 }, // Floormaster
    { ACTOR_EN_RD, 0 },       // Redead/Gibdo
    { ACTOR_EN_SB, 0 },       // Shellblade
    { ACTOR_EN_KAREBABA, 0 }, // Wilted Baba
    { ACTOR_EN_RR, 0 },       // Like Like
    //{ ACTOR_EN_IK, 0 },       // Iron Knuckle
    { ACTOR_EN_TUBO_TRAP, 0 }, // Flying Pot Trap
    { ACTOR_EN_FZ, 0 },        // Freezard
    { ACTOR_EN_WF, 0 },        // Wolfos
};

static uint8_t frameCounter = 0;

void ChaosSpawnEnemy() {
    Player* player = GET_PLAYER(gPlayState);
    uint16_t enemyIndex = rand() % RANDOMIZED_ENEMY_SPAWN_TABLE_SIZE;
    EnemyEntry randomEnemy = enemySpawnTable[enemyIndex];

    CollisionPoly poly;
    Vec3f pos;
    f32 raycastResult;

    int32_t xOffset = (rand() % 2) ? 100 : -100;
    int32_t zOffset = (rand() % 2) ? 100 : -100;

    pos.x = player->actor.world.pos.x + xOffset;
    pos.y = player->actor.world.pos.y + 50;
    pos.z = player->actor.world.pos.z + zOffset;
    raycastResult = BgCheck_AnyRaycastFloor1(&gPlayState->colCtx, &poly, &pos);

    // If ground is found below actor, move actor to that height.
    if (raycastResult > BGCHECK_Y_MIN) {
        pos.y = raycastResult;
    }

    Actor* spawnedActor = Actor_Spawn(&gPlayState->actorCtx, gPlayState, randomEnemy.id, pos.x, pos.y, pos.z, 0, 0, 0,
                                      randomEnemy.params);

    spawnedActor->category = ACTORCAT_BG;
}

void RegisterChaosMode() {
    GameInteractor::Instance->RegisterGameHook<GameInteractor::OnGameStateUpdate>([]() {
        if (!CVarGetInteger("gEnhancements.Modes.ChaosMode", 0) || gPlayState == NULL) {
            return;
        }

        frameCounter++;

        if (frameCounter > 100) {
            frameCounter = 0;

            ChaosSpawnEnemy();
        }
        
    });
}
