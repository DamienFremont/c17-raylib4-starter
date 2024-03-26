#include <time.h>

#pragma once

static int TIME_1_SECOND = 1 * 1000;

typedef struct TickState
{
    int rateInHz;

    int current;
    int lastUpdate;

    clock_t startClock;
    clock_t currentClock;

} TickState;

TickState InitTick(int tickRateInHz);

void StartTick(TickState *state);

int IsTickUpdate(TickState *state);

int UpdateTick(TickState *state);
