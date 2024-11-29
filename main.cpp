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
    cout << "Usage: kazik_simulator [options]\n";
    cout << "Options:\n";
    cout << "  --help                  Show this help message and exit\n";
    cout << "  --rtp [value]           Set the RTP value (options: 85, 90, 92, 95)\n";
    cout << "                          Default is 95\n";
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

        double finalLosersOverWinners = static_cast<double>(totalLost) / totalWon;

        double winrate = static_cast<double>(totalWins + totalBigWins) / totalSpins;
        // double meanWinrate = winrate / static_cast<double>(playerStats.size());
        double meanLosses = static_cast<double>(totalLosses) / playerStats.size();
        double meanReturns = static_cast<double>(totalReturns) / playerStats.size();
        double meanWins = static_cast<double>(totalWins) / playerStats.size();
        double meanBigWins = static_cast<double>(totalBigWins) / playerStats.size();
        double meanTimePlayed = static_cast<double>(totalTimePlayed) / playerStats.size();
        double meanSpins = static_cast<double>(totalSpins) / playerStats.size();
        double meanStartBalance = static_cast<double>(totalStartBalance) / playerStats.size();
        double meanEndBalance = static_cast<double>(totalEndBalance) / playerStats.size();


        // double probabilities[] = {NO_WIN_SPIN, COMEBACK_SPIN, SMALL_WIN_SPIN, BIG_WIN_SPIN};
        // double meanWinMultiplier = MAX_SMALL_WIN - MIN_SMALL_WIN / 2;
        // double meanJackpotMultiplier = MAX_JACKPOT_MULTIPLIER - MIN_JACKPOT_MULTIPLIER / 2;

      
        double probabilities[] = {rtpData->noWinSpin, rtpData->comebackSpin, rtpData->smallWinSpin, rtpData->bigWinSpin};
        double meanWinMultiplier = rtpData->maxSmallWin - rtpData->minSmallWin / 2;
        double meanJackpotMultiplier = rtpData->maxJackpotMultiplier - rtpData->minJackpotMultiplier / 2;


        double multipliers[] = {0, 1, meanWinMultiplier, meanJackpotMultiplier};

        int num_outcomes = sizeof(probabilities) / sizeof(probabilities[0]);
        double rtp = 0.0;

        for (int i = 0; i < num_outcomes; i++)
        {
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
        cout << "| " << "Losers Over Winners coefficient: " << finalLosersOverWinners << " |" << endl;
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

        // cout << "Player " << Name() << " starts with a balance of " << balance << " dollars." << endl;
        StartBalances(balance);
        if (balance < 100)
        {
            // cout << "Player " << Name() << " cannot play. Not enough money!" << endl;
            Release(Slot);
            return;
        }

        while (true)
        {
            if (balance < 5)
            {
                // cout << "Player " << Name() << " doesn't have enough balance to play. Going home at " << Time << endl;
                break;
            }

            spins++;

            balance -= STAKE;      
            CasinoBalance += STAKE;
            // cout << "Player " << Name() << " spent 5 dollars for a spin. Remaining balance: " << balance << " dollars." << endl;

            Wait(Exponential(5));

            double outcome = Random();

            double respin = Random();

            if (outcome < rtpData->noWinSpin)
            {
                // cout << "Player " << Name() << " lost at " << Time << endl;

                losses++;

                if (respin <  rtpData->noWinRespinChance)
                {
                    // cout << "Player " << Name() << " respinned at " << Time << endl;
                }
                else
                {
                    break;
                }
            }
            else if (outcome <  rtpData->noWinSpin + rtpData->comebackSpin)
            {
                // cout << "Player " << Name() << " won nothing at " << Time << endl;

                balance += STAKE;
                CasinoBalance -= STAKE;

                returns++;

                if (respin < rtpData->comebackRespinChance)
                {
                    // cout << "Player " << Name() << " respinned at " << Time << endl;
                }
                else
                {
                    break;
                }
            }
            else if (outcome < rtpData->noWinSpin + rtpData->comebackSpin + rtpData->smallWinSpin)
            {
                // cout << "Player " << Name() << " had a small win at " << Time << endl;
                double smallWinMultiplier = rtpData->minSmallWin + (Random() * (rtpData->maxSmallWin - rtpData->minSmallWin));

                balance += STAKE * smallWinMultiplier;
                CasinoBalance -= STAKE * smallWinMultiplier;

                wins++;

                if (respin < rtpData->smallWinRespinChance)
                {
                    // cout << "Player " << Name() << " respinned at " << Time << endl;
                }
                else
                {
                    break;
                }
            }
            else
            {
                // cout << "Player " << Name() << " won a JACKPOT at " << Time << endl;
                double jackpotMultiplier = rtpData->minJackpotMultiplier + (Random() * (rtpData->maxJackpotMultiplier - rtpData->minJackpotMultiplier));

                balance += STAKE * jackpotMultiplier;
                CasinoBalance -= STAKE * jackpotMultiplier;

                bigWins++;

                if (respin <  rtpData->bigWinRespinChance)
                {
                    // cout << "Player " << Name() << " respinned at " << Time << endl;
                }
                else
                {
                    break;
                }
            }

            if (CasinoBalance < 0)
            {
                cout << "Casino has gone bankrupt at " << Time << endl;
                return;
                // break;
            }
        }

        // cout << "Player " << Name() << " End balance " << balance << " dollars." << endl;
        EndBalances(balance);
        // Test(Time - EntryTime);

        // cout << "WINS: " << wins << " BIG WINS: " << bigWins << " RETURNS: " << returns << " LOSSES: " << losses << endl;

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

    Print("KAZIK\n");
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