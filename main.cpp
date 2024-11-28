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

// Histogram Losses("Losses", 0, 100, 20);
Histogram Test("Losses", 0, 100, 20);
Histogram StartBalances("Start Balances", 100, 2000, 20);  // Histogram for start balances
Histogram EndBalances("End Balances", 100, 2000, 20);  // Histogram for end balances

class Player : public Process
{
    double EntryTime;
    double balance;
    void Behavior()
    {
        EntryTime = Time;
        Seize(Slot);
        random_device rd;          // Obtain a random number from hardware
        mt19937 gen(rd());         
        uniform_real_distribution<double> dist(100.0, 1000.0); 
        balance = dist(gen);      

        cout << "Player " << Name() << " starts with a balance of " << balance << " dollars." << endl;
        StartBalances(balance);
        if (balance < 100)
        {
            cout << "Player " << Name() << " cannot play. Not enough money!" << endl;
            Release(Slot);
            return;  
        }


        while (true)
        {
            if (balance < 5)
            {
                cout << "Player " << Name() << " doesn't have enough balance to play. Going home at " << Time << endl;
                break; // End the game if the player has less than 5 dollars
            }

            balance -= STAKE; // Deduct 5 dollars per spin
            cout << "Player " << Name() << " spent 5 dollars for a spin. Remaining balance: " << balance << " dollars." << endl;

            Wait(Exponential(5));

            double outcome = Random();
            printf("Outcome: %f\n", outcome);
            enum SPIN_RESULT result;

            double respin = Random();

            if (outcome < NO_WIN_SPIN)
            {
                cout << "Player " << Name() << " lost at " << Time << endl;
                result = NO_WIN;

                if (respin < NO_WIN_RESPIN_CHANCE)
                {
                    cout << "Player " << Name() << " respinned at " << Time << endl;
                }
                else
                {
                    break;
                }
            }
            else if (outcome < NO_WIN_SPIN + COMEBACK_SPIN)
            {
                cout << "Player " << Name() << " won nothing at " << Time << endl;
                result = COMEBACK;
                balance += STAKE;
                if (respin < COMEBACK_RESPIN_CHANCE)
                {
                    cout << "Player " << Name() << " respinned at " << Time << endl;
                }
                else
                {
                    break;
                }
            }
            else if (outcome < NO_WIN_SPIN + COMEBACK_SPIN + SMALL_WIN_SPIN)
            {
                cout << "Player " << Name() << " had a small win at " << Time << endl;
                result = SMALL_WIN;
                double smallWinMultiplier = MIN_SMALL_WIN + (Random() * (MAX_SMALL_WIN - MIN_SMALL_WIN));
                balance += STAKE * smallWinMultiplier;
                if (respin < SMALL_WIN_RESPIN_CHANCE)
                {
                    cout << "Player " << Name() << " respinned at " << Time << endl;
                }
                else
                {
                    break;
                }
            }
            else
            {
                cout << "Player " << Name() << " won a JACKPOT at " << Time << endl;
                result = BIG_WIN;
                double jackpotMultiplier = MIN_JACKPOT_MULTIPLIER + (Random() * (MAX_JACKPOT_MULTIPLIER - MIN_JACKPOT_MULTIPLIER));
                balance += STAKE * jackpotMultiplier;
                if (respin < BIG_WIN_RESPIN_CHANCE)
                {
                    cout << "Player " << Name() << " respinned at " << Time << endl;
                }
                else
                {
                    break;
                }
            }
            // cout << "Player " << Name() << " started playing at " << Time << endl;
            // Wait(Exponential(1e3/150));
            // cout << "Player " << Name() << " finished playing at " << Time << endl;
        }

        cout << "Player " << Name() << " End balance " << balance << " dollars." << endl;
        EndBalances(balance);
        Test(Time - EntryTime);

        Release(Slot);
    }
};

class PeopleGenerator : public Event
{
    void Behavior()
    {
        (new Player)->Activate();
        Activate(Time + Exponential(3600));
        // Activate(Time + Exponential(3600));
    }
};

int main()
{ 

    // Initialize the random number generator with a time-based seed
    random_device rd;  // Obtain a random number from hardware
    mt19937 gen(rd()); // Use that number to seed a random number generator
    RandomSeed(gen());

    // Set this generator to be used globally (if Random uses the global RNG)
    srand(static_cast<unsigned int>(gen()));  // Seed the C++ random function

    int Runtime = 60 * 60 * 24;
    // int Runtime = 1000;

    Print("KAZIK\n");
    SetOutput("kazik.out");
    Init(0, Runtime);                  // experiment initialization for time 0..1000
    (new PeopleGenerator)->Activate(); // customer generator
    Run();                             // simulation
    //Test.Output();                     // print of results
    // Slot.Output();
    StartBalances.Output();
    EndBalances.Output();
    //   Box.Output();              // print of results
    //   Table.Output();
    return 0;
}