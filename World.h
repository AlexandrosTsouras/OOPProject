#include <iostream>
#include <vector>
#include "WorldObject.h"

using namespace std;

#define DEAFULT_SIZE_X 40
#define DEAFULT_SIZE_Y 40
#define DEAFULT_COUNT_MCAR 4
#define DEAFULT_COUNT_MBIKE 2
#define DEAFULT_COUNT_STOP 6
#define DEAFULT_COUNT_PVECHILES 5
#define DEAFULT_COUNT_TRAFFICLIGHTS 10


class World {

    Vector2 max_size; //actual size++

    //Vectors for every thing
    //Moving
    vector<MovingCar> MCars;
    vector<MovingBike> MBikes;
    //Static
    vector<STOPSign> SSTOPs;
    vector<ParkedVechile> SParkedVechiles;
    vector<TrafficLight> STrafficLights;


    //World
    vector<vector<WorldObject>> Grid;


public:

    World(int maxX = DEAFULT_SIZE_X, int maxY = DEAFULT_SIZE_Y, int mcar_count = DEAFULT_COUNT_MBIKE,
          int mbike_count = DEAFULT_COUNT_MBIKE, int stop_count = DEAFULT_COUNT_STOP, int parked_count = DEAFULT_COUNT_PVECHILES,
          int traffic_lights_count = DEAFULT_COUNT_TRAFFICLIGHTS)
    {
        Vector2 temp(maxX, maxY);
        max_size = temp;
    }

};