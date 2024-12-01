#include <iostream>
#include <simlib.h>

// #define RUNTIME 60 * 60 * 24 * 7 * 4 * 12 * 10 // 10 years
#define RUNTIME 60 * 60 * 24 * 7 * 4 * 12 // 1 year

#define SLOT_BALANCE 10000

#define START_PEOPLE_FREQUENCY 3600
#define LOSER_FREQUENCY_MULTIPLIER 1.01
#define WINNER_FREQUENCY_MULTIPLIER 0.99

#define MIN_DEPOSIT 100
#define MAX_DEPOSIT 1000
#define STAKE 5

#define START_RESPIN_CHANCE 0.98

#define NO_WIN_RESPIN_CHANCE_MULTIPLIER 0.93
#define COMEBACK_RESPIN_CHANCE_MULTIPLIER 1.2
#define SMALL_WIN_RESPIN_CHANCE_MULTIPLIER 1.5
#define BIG_WIN_RESPIN_CHANCE_MULTIPLIER 1.8

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

RTPData RTP_65 = {
    0.69,   
    0.2,   
    0.10,   
    0.01,  

    10,     
    20,     

    2,      
    4       
};