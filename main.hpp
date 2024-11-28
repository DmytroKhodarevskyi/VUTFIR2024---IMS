#include <iostream>
#include <simlib.h>

#define RUNTIME 60 * 60 * 24 * 7 * 4

#define SLOT_BALANCE 10000

#define MIN_DEPOSIT 100
#define STAKE 10

#define NO_WIN_SPIN 0.6
#define COMEBACK_SPIN 0.21 
#define SMALL_WIN_SPIN 0.18 
#define BIG_WIN_SPIN 0.01

#define NO_WIN_RESPIN_CHANCE 0.9
#define COMEBACK_RESPIN_CHANCE 0.95
#define SMALL_WIN_RESPIN_CHANCE 0.8
#define BIG_WIN_RESPIN_CHANCE 0.2

#define MIN_JACKPOT_MULTIPLIER 10
#define MAX_JACKPOT_MULTIPLIER 25

#define MIN_SMALL_WIN 2
#define MAX_SMALL_WIN 4