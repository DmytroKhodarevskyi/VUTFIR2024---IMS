////////////////////////////////////////////////////////////////////////////
// Model model2.cc                SIMLIB/C++
//
// Simple test model of queuing system
//

// #include "simlib.h"
#include "main.hpp"

#include <ctime>  // Include time for seeding random number generator
#include <random> // For random_device

using namespace std;

// global objects:
// Facility  Box("Box");
// Histogram Table("Table",0,25,20);

// class Customer : public Process {
//   double Prichod;                 // atribute of each customer
//   void Behavior() {               // --- behavoir specification ---
//     Prichod = Time;               // incoming time
//     Seize(Box);                   // start of service
//     Wait(10);                     // time of service
//     Release(Box);                 // end of service
//     Table(Time-Prichod);          // waiting and service time
//   }
// };

// class Generator : public Event {  // model of system's input
//   void Behavior() {               // --- behavior specification ---
//     (new Customer)->Activate();   // new customer
//     Activate(Time+Exponential(1e3/150));  //
//   }
// };

Facility Slot("Slot");

Histogram Test("Losses", 0, 100, 20);
Histogram StartBalances("Start Balances", 100, 2000, 20); // Histogram for start balances
Histogram EndBalances("End Balances", 100, 2000, 20);     // Histogram for end balances

double CasinoBalance = SLOT_BALANCE;
double CasinoStartBalance = SLOT_BALANCE;

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
        // double meanWinrate = winrate / static_cast<double>(playerStats.size());
        double meanLosses = static_cast<double>(totalLosses) / playerStats.size();
        double meanReturns = static_cast<double>(totalReturns) / playerStats.size();
        double meanWins = static_cast<double>(totalWins) / playerStats.size();
        double meanBigWins = static_cast<double>(totalBigWins) / playerStats.size();
        double meanTimePlayed = static_cast<double>(totalTimePlayed) / playerStats.size();
        double meanSpins = static_cast<double>(totalSpins) / playerStats.size();
        double meanStartBalance = static_cast<double>(totalStartBalance) / playerStats.size();
        double meanEndBalance = static_cast<double>(totalEndBalance) / playerStats.size();

        double probabilities[] = {NO_WIN_SPIN, COMEBACK_SPIN, SMALL_WIN_SPIN, BIG_WIN_SPIN};
        double meanWinMultiplier = MAX_SMALL_WIN - MIN_SMALL_WIN / 2;
        double meanJackpotMultiplier = MAX_JACKPOT_MULTIPLIER - MIN_JACKPOT_MULTIPLIER / 2;

        double multipliers[] = {0, 1, meanWinMultiplier, meanJackpotMultiplier};

        int num_outcomes = sizeof(probabilities) / sizeof(probabilities[0]);

        double rtp = 0.0;

        // Iterate through all outcomes to calculate weighted payout
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
        random_device rd; // Obtain a random number from hardware
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
                break; // End the game if the player has less than 5 dollars
            }

            spins++;

            balance -= STAKE;       // Deduct 5 dollars per spin
            CasinoBalance += STAKE; // Add 5 dollars to the casino balance
            // cout << "Player " << Name() << " spent 5 dollars for a spin. Remaining balance: " << balance << " dollars." << endl;

            Wait(Exponential(5));

            double outcome = Random();

            double respin = Random();

            if (outcome < NO_WIN_SPIN)
            {
                // cout << "Player " << Name() << " lost at " << Time << endl;

                losses++;

                if (respin < NO_WIN_RESPIN_CHANCE)
                {
                    // cout << "Player " << Name() << " respinned at " << Time << endl;
                }
                else
                {
                    break;
                }
            }
            else if (outcome < NO_WIN_SPIN + COMEBACK_SPIN)
            {
                // cout << "Player " << Name() << " won nothing at " << Time << endl;

                balance += STAKE;
                CasinoBalance -= STAKE;

                returns++;

                if (respin < COMEBACK_RESPIN_CHANCE)
                {
                    // cout << "Player " << Name() << " respinned at " << Time << endl;
                }
                else
                {
                    break;
                }
            }
            else if (outcome < NO_WIN_SPIN + COMEBACK_SPIN + SMALL_WIN_SPIN)
            {
                // cout << "Player " << Name() << " had a small win at " << Time << endl;
                double smallWinMultiplier = MIN_SMALL_WIN + (Random() * (MAX_SMALL_WIN - MIN_SMALL_WIN));

                balance += STAKE * smallWinMultiplier;
                CasinoBalance -= STAKE * smallWinMultiplier;

                wins++;

                if (respin < SMALL_WIN_RESPIN_CHANCE)
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
                double jackpotMultiplier = MIN_JACKPOT_MULTIPLIER + (Random() * (MAX_JACKPOT_MULTIPLIER - MIN_JACKPOT_MULTIPLIER));

                balance += STAKE * jackpotMultiplier;
                CasinoBalance -= STAKE * jackpotMultiplier;

                bigWins++;

                if (respin < BIG_WIN_RESPIN_CHANCE)
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
                break;
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

int main()
{

    // Initialize the random number generator with a time-based seed
    random_device rd;  // Obtain a random number from hardware
    mt19937 gen(rd()); // Use that number to seed a random number generator
    RandomSeed(gen());

    // Set this generator to be used globally (if Random uses the global RNG)
    srand(static_cast<unsigned int>(gen())); // Seed the C++ random function

    Print("KAZIK\n");
    SetOutput("kazik.out");
    Init(0, RUNTIME);                  // experiment initialization for time 0..1000
    (new PeopleGenerator)->Activate(); // customer generator
    Run();                             // simulation
    // Test.Output();                     // print of results
    //  Slot.Output();
    StartBalances.Output();
    EndBalances.Output();
    //   Box.Output();              // print of results
    //   Table.Output();

    stats.PrintMeanStats();

    cout << "Casino balance: " << CasinoBalance << " dollars." << endl;
    cout << "Casino start balance: " << CasinoStartBalance << " dollars." << endl;
    cout << "Casino profit: " << CasinoBalance - CasinoStartBalance << " dollars." << endl;
    return 0;
}