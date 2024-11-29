#include <iostream>
#include <simlib.h>

#define RUNTIME 60 * 60 * 24 * 7 * 4 * 12

#define SLOT_BALANCE 10000

#define MIN_DEPOSIT 100
#define STAKE 10

#define START_RESPIN_CHANCE 0.98

#define NO_WIN_RESPIN_CHANCE_MULTIPLIER 0.95
#define COMEBACK_RESPIN_CHANCE_MULTIPLIER 1.0
#define SMALL_WIN_RESPIN_CHANCE_MULTIPLIER 1.2
#define BIG_WIN_RESPIN_CHANCE_MULTIPLIER 1.4

struct RTPData {
    double noWinSpin;
    double comebackSpin;
    double smallWinSpin;
    double bigWinSpin;

    int minJackpotMultiplier;
    int maxJackpotMultiplier;

    int minSmallWin;
    int maxSmallWin;
};


// RTP 95%
RTPData RTP_95 = {
    0.55,    
    0.26,  
    0.18,   
    0.01, 

    10,     
    20,     

    2,      
    4       
};

// RTP 92%
RTPData RTP_92 = {
    0.6,    
    0.20,  
    0.19,  
    0.01,   

    10,     
    20,    

    2,     
    4      
};

// RTP 90%
RTPData RTP_90 = {
    0.62,   
    0.18,   
    0.19,   
    0.01,   

    10,     
    20,     

    2,      
    4       
};

// RTP 85%
RTPData RTP_85 = {
    0.63,   
    0.19,   
    0.17,   
    0.01,  

    10,     
    20,     

    2,      
    4       
};