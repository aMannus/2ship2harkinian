#ifndef MODES_CHAOS_MODE_H
#define MODES_CHAOS_MODE_H

#include <libultraship/bridge.h>

typedef struct EnemyEntry {
    int16_t id;
    int16_t params;
} EnemyEntry;

#define RANDOMIZED_ENEMY_SPAWN_TABLE_SIZE 4

void RegisterChaosMode();

#endif // MODES_CHAOS_MODE_H
