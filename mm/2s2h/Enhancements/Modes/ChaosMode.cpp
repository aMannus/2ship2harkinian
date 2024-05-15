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
    { ACTOR_EN_OKUTA, 0 },      // Oktorok
    { ACTOR_EN_WALLMAS, 0 },    // Wallmaster
    { ACTOR_EN_DODONGO, 0 },    // Dodongo
    { ACTOR_EN_FIREFLY, 0 },    // Keese Fire
    { ACTOR_EN_FIREFLY, 3 },    // Keese Normal
    { ACTOR_EN_FIREFLY, 4 },    // Keese Ice
    { ACTOR_EN_TITE, -4 },      // Tektite
    { ACTOR_EN_PEEHAT, 0 },     // Peahat
    { ACTOR_EN_DINOFOS, 0 },    // Dinolfos
    { ACTOR_EN_BUBBLE, 0 },     // Shabom
    { ACTOR_EN_ST, 0 },         // Large Skulltula
    { ACTOR_EN_FAMOS, 0 },      // Death Armos (Inv. Stone Tower)
    { ACTOR_EN_AM, 0 },         // Armos
    { ACTOR_EN_DEKUBABA, 0 },   // Deku Baba
    { ACTOR_EN_DEKUNUTS, 0 },   // Mad Scrub
    { ACTOR_EN_BB, 0 },         // Blue Bubble
    { ACTOR_EN_VM, 0 },         // Beamos
    { ACTOR_EN_FLOORMAS, 0 },   // Floormaster
    { ACTOR_EN_RD, 0 },         // Redead/Gibdo
    { ACTOR_EN_SB, 0 },         // Shellblade
    { ACTOR_EN_KAREBABA, 0 },   // Wilted Baba
    { ACTOR_EN_RR, 0 },         // Like Like
    { ACTOR_EN_IK, 1 },         // Iron Knuckle
    { ACTOR_EN_TUBO_TRAP, 0 },  // Flying Pot Trap
    { ACTOR_EN_FZ, 0 },         // Freezard
    { ACTOR_EN_WF, 0 },         // Wolfos
    { ACTOR_EN_SKB, 0 },        // Stalchild
    { ACTOR_EN_CROW, 0 },       // Guay
    { ACTOR_EN_SLIME, 0 },      // Chuchu
    { ACTOR_EN_PR, 0 },         // Desbreko
    { ACTOR_EN_BAGUO, 0 },      // Nejiron
    { ACTOR_EN_BAT, 0 },        // Bad Bat
    { ACTOR_EN_MKK, 0 },        // Black Boe
    { ACTOR_EN_RAT, 0 },        // Real Bombchu
    { ACTOR_EN_KAME, 0 },       // Snapper
    { ACTOR_EN_WDHAND, 0 },     // Dexihand
    { ACTOR_EN_SNOWMAN, 0 },    // Eeno
    { ACTOR_EN_PO_SISTERS, 0 }, // Poe Sister
    { ACTOR_EN_PP, 0 },         // Hiploop
    { ACTOR_EN_POH, 0 },        // Poe
    { ACTOR_EN_BEE, 0 },        // Giant Bee
    { ACTOR_EN_DRAGON, 0 },     // Deep Python
    { ACTOR_EN_KAIZOKU, 0 },    // Fighter pirate
    { ACTOR_EN_THIEFBIRD, 0 },  // Takkuri
    { ACTOR_EN_BAL, 0 },        // Tingle with balloon
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

    gPlayState->actorCtx.actorLists[spawnedActor->category].unk_08 = 1;
    spawnedActor->category = ACTORCAT_BG;
}

void RegisterChaosMode() {
    GameInteractor::Instance->RegisterGameHook<GameInteractor::OnGameStateUpdate>([]() {

        if (!CVarGetInteger("gEnhancements.Modes.ChaosMode", 0) || gPlayState == NULL) {
            return;
        }

        Player* player = GET_PLAYER(gPlayState);

        if (Player_InBlockingCsMode(gPlayState, player) || gPlayState->pauseCtx.state != 0 ||
            gPlayState->msgCtx.msgMode != 0) {
            return;
        }

        frameCounter++;

        if (frameCounter > 100) {
            frameCounter = 0;

            ChaosSpawnEnemy();
        }
        
    });
}
