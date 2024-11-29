#include "main.hpp"

#include <ctime> 
#include <random> 

using namespace std;

Facility Slot("Slot");

Histogram Test("Losses", 0, 100, 20);
Histogram StartBalances("Start Balances", 100, 2000, 20); 
Histogram EndBalances("End Balances", 100, 2000, 20);     

double CasinoBalance = SLOT_BALANCE;
double CasinoStartBalance = SLOT_BALANCE;

RTPData* rtpData = &RTP_95;


void help() {
    cout << "Usage: ./simulation.exe [options]\n";
    cout << "Options:\n";
    cout << "  --help                  Show this help message and exit\n";
    cout << "  --rtp [value]           Set the RTP value (options: 85, 90, 92, 95)\n";
    cout << "   No options              Default is RTP 95\n";
    cout << "Example:\n";
    cout << "  ./simulation.exe --rtp 90\n";
    cout << "  ./simulation.exe --help\n";
    cout << "if ./simulation.exe would be with other paramaters, it will run as usual with RTP 95 \n";
    
    cout << "\nWhat is RTP?\n";
    cout << "RTP (Return to Player) is a percentage that indicates the expected return a player will receive over time. ";
    cout << "For example, if a game has a 95% RTP, the player can expect to receive 95% of their bets back in the long run, on average.\n";
}


class Statistics
{

public:
    struct PlayerStats
    {
        double startBalance;
        double endBalance;
        double timePlayed;

        int losses;
        int returns;
        int wins;
        int bigWins;

        int spins;

        bool won;
    };

    vector<PlayerStats> playerStats;

    void AddPlayerStats(double startBalance,
                        double endBalance,
                        double timePlayed,
                        int spins,
                        int losses,
                        int returns,
                        int wins,
                        int bigWins,
                        bool won = false)
    {
        PlayerStats stats;
        stats.startBalance = startBalance;
        stats.endBalance = endBalance;
        stats.timePlayed = timePlayed;
        stats.spins = spins;
        stats.losses = losses;
        stats.returns = returns;
        stats.wins = wins;
        stats.bigWins = bigWins;
        stats.won = won;
        playerStats.push_back(stats);
    }

    void PrintMeanStats()
    {
        double totalStartBalance = 0;
        double totalEndBalance = 0;
        double totalTimePlayed = 0;
        int totalSpins = 0;
        int totalLosses = 0;
        int totalReturns = 0;
        int totalWins = 0;
        int totalBigWins = 0;

        int totalWon = 0;
        int totalLost = 0;

        for (unsigned int i = 0; i < playerStats.size(); i++)
        {
            totalStartBalance += playerStats[i].startBalance;
            totalEndBalance += playerStats[i].endBalance;
            totalTimePlayed += playerStats[i].timePlayed;
            totalSpins += playerStats[i].spins;
            totalLosses += playerStats[i].losses;
            totalReturns += playerStats[i].returns;
            totalWins += playerStats[i].wins;
            totalBigWins += playerStats[i].bigWins;

            if (playerStats[i].won)
            {
                totalWon++;
            }
            else
            {
                totalLost++;
            }
        }

        double winrate = static_cast<double>(totalWins + totalBigWins) / totalSpins;
        double meanLosses = static_cast<double>(totalLosses) / playerStats.size();
        double meanReturns = static_cast<double>(totalReturns) / playerStats.size();
        double meanWins = static_cast<double>(totalWins) / playerStats.size();
        double meanBigWins = static_cast<double>(totalBigWins) / playerStats.size();
        double meanTimePlayed = static_cast<double>(totalTimePlayed) / playerStats.size();
        double meanSpins = static_cast<double>(totalSpins) / playerStats.size();
        double meanStartBalance = static_cast<double>(totalStartBalance) / playerStats.size();
        double meanEndBalance = static_cast<double>(totalEndBalance) / playerStats.size();

        double probabilities[] = {rtpData->noWinSpin, rtpData->comebackSpin, rtpData->smallWinSpin, rtpData->bigWinSpin};
    
        double meanSmallWinMultiplier = (rtpData->maxSmallWin - rtpData->minSmallWin) / 2.0 + rtpData->minSmallWin;
        double meanJackpotMultiplier = (rtpData->maxJackpotMultiplier - rtpData->minJackpotMultiplier) / 2.0 + rtpData->minJackpotMultiplier;

        double multipliers[] = {0, 1, meanSmallWinMultiplier, meanJackpotMultiplier};

        double rtp = 0.0;
        int num_outcomes = sizeof(probabilities) / sizeof(probabilities[0]);
        for (int i = 0; i < num_outcomes; i++) {
            rtp += probabilities[i] * multipliers[i]; 
        }

        double rtp_percentage = rtp * 100;

        cout << endl
             << "+========================================+" << endl;
        cout << "| " << "Total players: " << playerStats.size() << " |" << endl;
        cout << "| " << "Total spins: " << totalSpins << " |" << endl;
        cout << "| " << "Total losses: " << totalLosses << " |" << endl;
        cout << "| " << "Total returns: " << totalReturns << " |" << endl;
        cout << "| " << "Total wins: " << totalWins << " |" << endl;
        cout << "| " << "Total big wins: " << totalBigWins << " |" << endl;
        cout << "| " << "Total poeple won: " << totalWon << " |" << endl;
        cout << "| " << "Total people lost: " << totalLost << " |" << endl;
        cout << "+========================================+" << endl;
        cout << "| " << "Mean start balance: " << meanStartBalance << " dollars." << " |" << endl;
        cout << "| " << "Mean end balance: " << meanEndBalance << " dollars." << " |" << endl;
        cout << "+========================================+" << endl;
        cout << "| " << "Mean time played: " << meanTimePlayed << " seconds." << " |" << endl;
        cout << "+========================================+" << endl;
        cout << "| " << "Mean spins per person: " << meanSpins << " |" << endl;
        cout << "+========================================+" << endl;
        cout << "| " << "Mean losses per person: " << meanLosses << " |" << endl;
        cout << "| " << "Mean returns per person: " << meanReturns << " |" << endl;
        cout << "| " << "Mean wins per person: " << meanWins << " |" << endl;
        cout << "| " << "Mean big wins per person: " << meanBigWins << " |" << endl;
        cout << "+========================================+" << endl;
        cout << "| " << "Mean winrate: " << winrate << " |" << endl;
        cout << "+========================================+" << endl;
        cout << "| " << "RTP: " << rtp_percentage << "%" << " |" << endl;
        cout << "+========================================+" << endl;
    }
};

Statistics stats;

class Player : public Process
{
    double EntryTime;
    double balance;

    int spins = 0;
    int losses = 0;
    int returns = 0;
    int wins = 0;
    int bigWins = 0;

    void Behavior()

    {
        EntryTime = Time;
        Seize(Slot);
        random_device rd; 
        mt19937 gen(rd());
        uniform_real_distribution<double> dist(100.0, 1000.0);
        balance = dist(gen);

        double startBalance = balance;

        StartBalances(balance);
        if (balance < 100)
        {
            Release(Slot);
            return;
        }

        double respinChance = 0.85;  
        double goHomeChance = 0.15;  

        while (true)
        {
            if (balance < 5)
            {
                break;
            }

            spins++;

            balance -= STAKE;      
            CasinoBalance += STAKE;

            Wait(Exponential(5));

            double outcome = Random();

            double respin = Random();

            if (outcome < rtpData->noWinSpin)
            {
    

                losses++;
                respinChance -= 0.15; 
                goHomeChance += 0.15; 
                if (respin <  respinChance)
                {
                    continue;
                }
                else
                {
                    break;
                }
            }
            else if (outcome <  rtpData->noWinSpin + rtpData->comebackSpin)
            {

                balance += STAKE;
                CasinoBalance -= STAKE;
                respinChance += 0.05; 
                goHomeChance -= 0.05; 
                returns++;

                if (respin < respinChance)
                {
                    continue;
                }
                else
                {
                    break;
                }
            }
            else if (outcome < rtpData->noWinSpin + rtpData->comebackSpin + rtpData->smallWinSpin)
            {
                double smallWinMultiplier = rtpData->minSmallWin + (Random() * (rtpData->maxSmallWin - rtpData->minSmallWin));

                balance += STAKE * smallWinMultiplier;
                CasinoBalance -= STAKE * smallWinMultiplier;
                respinChance += 0.07; 
                goHomeChance -= 0.07; 

                wins++;

                if (respin < respinChance)
                {
                    continue;
                }
                else
                {
                    break;
                }
            }
            else
            {
                double jackpotMultiplier = rtpData->minJackpotMultiplier + (Random() * (rtpData->maxJackpotMultiplier - rtpData->minJackpotMultiplier));

                balance += STAKE * jackpotMultiplier;
                CasinoBalance -= STAKE * jackpotMultiplier;
                respinChance += 0.1; 
                goHomeChance -= 0.1; 


                bigWins++;

                if (respin < respinChance)
                {
                    continue;
                }
                else
                {
                    break;
                }
            }

            if (CasinoBalance < 0)
            {
                cout << "Casino has gone bankrupt at " << Time << endl;
                break;
            }
        }

        EndBalances(balance);
        bool won = false;

        if (startBalance < balance)
        {
            won = true;
        }

        stats.AddPlayerStats(startBalance, balance, Time - EntryTime, spins, losses, returns, wins, bigWins, won);

        Release(Slot);
    }
};

class PeopleGenerator : public Event
{
    void Behavior()
    {
        (new Player)->Activate();
        Activate(Time + Exponential(3600));
    }
};

int main(int argc, char* argv[])
{
    random_device rd;
    mt19937 gen(rd());
    RandomSeed(gen());

    srand(static_cast<unsigned int>(gen())); 

     if (argc == 2 && string(argv[1]) == "--help") {
        help();  
        return 0;
    }

    for (int i = 1; i < argc; i++) {
        if (string(argv[i]) == "--rtp" && i + 1 < argc) {
            string rtpValue = argv[i + 1];
            if (rtpValue == "85") {
                rtpData = &RTP_85;  
            } else if (rtpValue == "90") {
                rtpData = &RTP_90;  
            } else if (rtpValue == "92") {
                rtpData = &RTP_92;  
            } else if (rtpValue == "95") {
                rtpData = &RTP_95;  
            } else {
                cout << "Invalid RTP value: " << rtpValue << ". Available options: 85, 90, 92, 95.\n";
                return 1;  
            }
        }
    }

    SetOutput("kazik.out");
    Init(0, RUNTIME);                  
    (new PeopleGenerator)->Activate(); 
    Run();                            
    StartBalances.Output();
    EndBalances.Output();
    stats.PrintMeanStats();

    cout << "Casino balance: " << CasinoBalance << " dollars." << endl;
    cout << "Casino start balance: " << CasinoStartBalance << " dollars." << endl;
    cout << "Casino profit: " << CasinoBalance - CasinoStartBalance << " dollars." << endl;
    return 0;
}