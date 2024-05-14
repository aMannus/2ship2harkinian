#include "Enhancements/GameInteractor/GameInteractor.h"
#include "ChaosMode.h"

extern "C" {
#include <variables.h>
#include "functions.h"
}

static EnemyEntry enemySpawnTable[RANDOMIZED_ENEMY_SPAWN_TABLE_SIZE] = {
    { ACTOR_EN_TITE, -4 }, // Tektite
    { ACTOR_EN_DODONGO, 0 }, // Dodongo
    { ACTOR_EN_FIREFLY, 1 }, // Keese
    { ACTOR_EN_PEEHAT, 0 },  // Peahat
};

static uint8_t frameCounter = 0;

void ChaosSpawnEnemy() {
    Player* player = GET_PLAYER(gPlayState);
    uint16_t enemyIndex = rand() % RANDOMIZED_ENEMY_SPAWN_TABLE_SIZE;
    EnemyEntry randomEnemy = enemySpawnTable[enemyIndex];

    CollisionPoly poly;
    Vec3f pos;
    f32 raycastResult;

    pos.x = player->actor.world.pos.x + 100;
    pos.y = player->actor.world.pos.y + 50;
    pos.z = player->actor.world.pos.z;
    raycastResult = BgCheck_AnyRaycastFloor1(&gPlayState->colCtx, &poly, &pos);

    // If ground is found below actor, move actor to that height.
    if (raycastResult > BGCHECK_Y_MIN) {
        pos.y = raycastResult;
    }

    Actor* spawnedActor =
        Actor_Spawn(&gPlayState->actorCtx, gPlayState, randomEnemy.id, pos.x, pos.y, pos.z, 0, 0, 0, randomEnemy.params);

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
