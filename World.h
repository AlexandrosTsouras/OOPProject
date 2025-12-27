#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include "WorldObject.h"


using namespace std;

#define DEAFULT_SIZE_X 20
#define DEAFULT_SIZE_Y 20
#define DEAFULT_COUNT_MCAR 4
#define DEAFULT_COUNT_MBIKE 2
#define DEAFULT_COUNT_STOP 6
#define DEAFULT_COUNT_PVECHILES 5
#define DEAFULT_COUNT_TRAFFICLIGHTS 10


class World {

    int seed;

    Vector2 max_size; //actual size++

    //Vectors for every thing
    //Moving
    vector<WorldObject*> MCars;
    vector<WorldObject*> MBikes;
    //Static
    vector<WorldObject*> SSTOPs;
    vector<WorldObject*> SParkedVechiles;
    vector<WorldObject*> STrafficLights;


    //World
    
    
    vector<vector<WorldObject*>> Grid;
public:

    World(int maxX = DEAFULT_SIZE_X, int maxY = DEAFULT_SIZE_Y, int mcar_max = DEAFULT_COUNT_MBIKE,
          int mbike_max = DEAFULT_COUNT_MBIKE, int stop_max = DEAFULT_COUNT_STOP, int parked_max = DEAFULT_COUNT_PVECHILES,
          int traffic_lights_max = DEAFULT_COUNT_TRAFFICLIGHTS, int s = 0) 
    {
        max_size.setX(maxX);
        max_size.setY(maxY);
        
        //world generation
        cout << max_size.getX();
        cout << max_size.getY();
        
        


        srand(time(NULL));
        bool done;
        Grid.reserve(max_size.getY());
        for (int i=0; i<max_size.getY(); i++) {
            Grid[i].reserve(max_size.getX()*sizeof(WorldObject*));
            for (int j=0; j<max_size.getX(); j++) {
                WorldObject* newObject;
                if (i%(max_size.getY()-1)==0 || j%(max_size.getX()-1)==0) {
                    newObject = new Border(j, i);
                }
                else{
                done = false;
                while (done == false) {
                    int rand_num = rand() % 36;
                
                    switch(rand_num) {
                    case 0 ... 30:
                        newObject = new Road(j,i);
                        done = true;
                        continue;
                    case 31:
                        if (MCars.size() == mcar_max) break;
                        newObject = new MovingCar(j,i);
                        MCars.push_back(newObject);
                        done = true;
                        continue;
                    case 32:
                        if (MBikes.size() == mbike_max) break;
                        newObject = new MovingBike(j,i);
                        MBikes.push_back(newObject);
                        done = true;
                        continue;
                    case 33:
                        if (SSTOPs.size() == stop_max) break;
                        newObject = new STOPSign(j,i);
                        SSTOPs.push_back(newObject);
                        done = true;
                        continue;
                    case 34:
                        if (SParkedVechiles.size() == parked_max) break;
                        newObject = new ParkedVechile(j,i);
                        SParkedVechiles.push_back(newObject);
                        done = true;
                        continue;
                    case 35:
                        if (STrafficLights.size() == traffic_lights_max) break;
                        newObject = new TrafficLight(j,i);
                        STrafficLights.push_back(newObject);
                        done = true;
                        continue;
                    }
                }}
                newObject->print_object();
                Grid[i].push_back(newObject);

            }
        }
        
        
    }
    


    
    
    void print_world() {
        
        for (int i=0; i<max_size.getY(); i++) {
            for (int j=0; j<max_size.getX(); j++) {
                
                Grid[i][j]->print_object();
                
                cout << ' ';
            }
            cout << endl;
        }
    }


    
    WorldObject get_WObject(int y, int x) {
        //get the Grid thing at that ps IDK WHAT IM DOING OK?
        cout <<"head\n";
        WorldObject cur = Grid[y][x]->get_Object(*Grid[y][x]);
        cout << "tail\n";
        return *cur;
    }

    Vector2 get_maxsize() {
        return max_size;
    }

};