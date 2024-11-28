#include <iostream>
#include <simlib.h>


#define MIN_DEPOSIT 100
#define STAKE 5

#define NO_WIN_SPIN 0.5
#define COMEBACK_SPIN 0.3
#define SMALL_WIN_SPIN 0.18
#define BIG_WIN_SPIN 0.02 

#define NO_WIN_RESPIN_CHANCE 0.7
#define COMEBACK_RESPIN_CHANCE 0.95
#define SMALL_WIN_RESPIN_CHANCE 0.4
#define BIG_WIN_RESPIN_CHANCE 0.1

enum SPIN_RESULT {
    NO_WIN,
    COMEBACK,
    SMALL_WIN,
    BIG_WIN
};

