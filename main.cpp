////////////////////////////////////////////////////////////////////////////
// Model model2.cc                SIMLIB/C++
//
// Simple test model of queuing system
//

// #include "simlib.h"
#include "main.hpp"

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

class Player : public Process
{
    double EntryTime;
    void Behavior()
    {
        EntryTime = Time;
        Seize(Slot);

        while (true)
        {

            Wait(5);

            double outcome = Random();
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

        Test(Time - EntryTime);

        Release(Slot);
    }
};

class PeopleGenerator : public Event
{
    void Behavior()
    {
        (new Player)->Activate();
        Activate(Time + Exponential(1.0 / 3600));
    }
};

int main()
{ // experiment description

    // int Runtime = 60 * 60 * 60 * 24;
    int Runtime = 1000;

    Print("KAZIK\n");
    SetOutput("kazik.out");
    Init(0, Runtime);                  // experiment initialization for time 0..1000
    (new PeopleGenerator)->Activate(); // customer generator
    Run();                             // simulation
    Test.Output();                     // print of results
    Slot.Output();
    //   Box.Output();              // print of results
    //   Table.Output();
    return 0;
}