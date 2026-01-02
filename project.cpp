#include <iostream>
#include "ACar.h"
#include <chrono>
#include <thread>
#include <ctime>


using namespace std;

void print_Help()
{
cout << "Self−Driving Car Simulation" << endl ;
cout << " −−seed <n> Random seed (default: current time) " << endl ;
cout << " −−dimX <n> World width (default: 40) " << endl ;
cout << " −−dimY <n> World height(default : 40) " << endl ;
cout << " −−numMovingCars <n> Number of moving cars (default: 3) " << endl ;
cout << " −−numMovingBikes <n> Number of moving bikes (default: 4) " << endl ;
cout << " −−numParkedCars <n> Number of parked cars (defaul t: 5) " << endl ;
cout << " −−numStopSigns <n> Number of stop signs (default: 2) " << endl ;
cout << " −−numTrafficLights <n> Number of traffic lights (default: 2) " << endl ;
cout << " −−simulationTicks <n> Maximum simulation ticks (default : 100) " << endl ;
cout << " −−gps <x1> <y1> GPS target coordinates (required) " << endl ;
}


int main() {

    srand(time(NULL));

    print_Help();
    cout << "Would you like the automatic/default values for everything or set?\n";
    string answer;
    cin >> answer;

        int mX = DEAFULT_SIZE_X;
        int mY = DEAFULT_SIZE_Y;
        int n_MC = DEAFULT_COUNT_MCAR;
        int n_MB = DEAFULT_COUNT_MBIKE;
        int n_PV = DEAFULT_COUNT_PVECHILES;
        int n_SS = DEAFULT_COUNT_STOP;
        int n_TC = DEAFULT_COUNT_TRAFFICLIGHTS;
        int s = rand();
        int tick_speed = 1;
    if (!(answer[0] == 'a' || answer[0] == 'd')) {
        cout << "X border: ";
        cin >> mX;
        cout << "\nY border: ";
        cin >> mY;
        cout << "\nMoving Car count: ";
        cin >> n_MC;
        cout << "\nMoving Bike count: ";
        cin >> n_MB;
        cout << "\nParked Vechiles count: ";
        cin >> n_PV;
        cout << "\nStop Signs count: ";
        cin >> n_SS;
        cout << "\nTraffic Lights count: ";
        cin >> n_TC;
        cout << "Seed: ";
        cin >> s;
        cout << "Tick speed: ";
        cin >> tick_speed;
    }
    World W(mX, mY, n_MC, n_MB, n_SS, n_PV, n_TC, s);


    int x, y;
    cout << "Wanted final destination: \n\tx: ";
    cin >> x;
    cout << "\ty: ";
    cin >> y;
    Vector2 f_d(x,y);
    AutomaticCar A(&W, f_d);
    

    // WorldObject* WO = new AutomaticCar(&W);
    // W.print_world();

    // WorldObject* A = new AutomaticCar;
    // A->Full_Update();
    
    W.print_world();
    while (!(A.get_position()==f_d)) {
        A.Full_Update(&W);
        this_thread::sleep_for(chrono::seconds(tick_speed));
    }



    cout << endl;
    return 0;
}