#include <iostream>
#include <simlib.h>

#define RUNTIME 60 * 60 * 24 * 7 * 4

#define SLOT_BALANCE 10000

#define MIN_DEPOSIT 100
#define STAKE 10


struct RTPData {
    double noWinSpin;
    double comebackSpin;
    double smallWinSpin;
    double bigWinSpin;

    double noWinRespinChance;
    double comebackRespinChance;
    double smallWinRespinChance;
    double bigWinRespinChance;

    int minJackpotMultiplier;
    int maxJackpotMultiplier;

    int minSmallWin;
    int maxSmallWin;
};

// RTP 95%
RTPData RTP_95 = {
    0.6,    
    0.21,  
    0.18,   
    0.01, 

    0.9,   
    0.95,   
    0.8,   
    0.2,   

    10,     
    25,     

    2,      
    4       
};

// RTP 92%
RTPData RTP_92 = {
    0.6,    
    0.20,  
    0.19,  
    0.01,   

    0.9,    
    0.95,   
    0.8,    
    0.2,    

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

    0.9,   
    0.95,   
    0.8,    
    0.2,   

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

    0.9,    
    0.95,   
    0.8,    
    0.25,   

    10,     
    20,     

    2,      
    4       
};