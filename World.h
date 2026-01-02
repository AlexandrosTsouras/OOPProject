#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include "WorldObject.h"

using namespace std;

#define DEAFULT_SIZE_X 40
#define DEAFULT_SIZE_Y 40
#define DEAFULT_COUNT_MCAR 3
#define DEAFULT_COUNT_MBIKE 4
#define DEAFULT_COUNT_STOP 5
#define DEAFULT_COUNT_PVECHILES 2
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

    World(int maxX = DEAFULT_SIZE_X, int maxY = DEAFULT_SIZE_Y, int mcar_max = DEAFULT_COUNT_MCAR,
          int mbike_max = DEAFULT_COUNT_MBIKE, int stop_max = DEAFULT_COUNT_STOP, int parked_max = DEAFULT_COUNT_PVECHILES,
          int traffic_lights_max = DEAFULT_COUNT_TRAFFICLIGHTS, int s = 0) {
        max_size.setX(maxX);
        max_size.setY(maxY);
        
        //world generation        

        srand(s);
        bool done;
        Grid.reserve(max_size.getY());

        int Rcount_road = (max_size.getX()-2)*(max_size.getY()-2) - (mcar_max+mbike_max+stop_max+parked_max+traffic_lights_max);    
        int Rcount_MC = mcar_max;
        int Rcount_MB = mbike_max;
        int Rcount_SS = stop_max;
        int Rcount_SP = parked_max;
        int Rcount_TL = traffic_lights_max;
        int car = 1;

        for (int i=0; i<max_size.getY(); i++) {
            Grid[i].reserve(max_size.getX()*sizeof(WorldObject*));
            for (int j=0; j<max_size.getX(); j++) {
                WorldObject* newObject;
                if (i%(max_size.getY()-1)==0 || j%(max_size.getX()-1)==0) {
                    newObject = new Border(j, i);
                }
                else {
                    int rand_num;
                    rand_num = rand() % (Rcount_road+Rcount_MB+Rcount_MC+Rcount_SP+Rcount_SS+Rcount_TL)+car;
                    // cout << "Random Number: " << rand_num << endl;
                    if (rand_num<Rcount_road) {
                        // cout << "Came in road" << endl;
                        newObject = new Road(j,i);
                        Rcount_road--;
                    }
                    else if ((rand_num%Rcount_road)<Rcount_MC) {
                        // cout << "Came in moving car" << endl;
                        newObject = new MovingCar(j,i);
                        MCars.push_back(newObject);
                        done = true;
                        Rcount_MC--;
                    }
                    else if ((rand_num%(Rcount_road+Rcount_MC))<Rcount_MB) { 
                        // cout << "Came in moving Bike" << endl;
                        newObject = new MovingBike(j,i);
                        MBikes.push_back(newObject);
                        done = true;
                        Rcount_MB--;
                    }
                    else if ((rand_num%(Rcount_road+Rcount_MC+Rcount_MB)) < Rcount_SS) {
                        newObject = new STOPSign(j,i);
                        SSTOPs.push_back(newObject);
                        done = true;
                        Rcount_SS--;
                    }
                    else if ((rand_num%(Rcount_road+Rcount_MC+Rcount_MB+Rcount_SS)) < Rcount_SP) {
                        newObject = new ParkedVechile(j,i);
                        SParkedVechiles.push_back(newObject);
                        done = true;
                        Rcount_SP--;
                    }
                    else if ((rand_num%(Rcount_road+Rcount_MC+Rcount_MB+Rcount_SS+Rcount_SP))<Rcount_TL) {
                        newObject = new TrafficLight(j,i);
                        STrafficLights.push_back(newObject);
                        done = true;
                        Rcount_TL--;
                    }
                    else {
                        newObject = new CarReservation(j, i);
                        car--;
                    }
                    // cout << "Left:" << endl << "\tRoad: " << Rcount_road << "\n\tCars: " << Rcount_MC << "\n\tBikes: " << Rcount_MB;
                    // cout << "\n\tParked: " << Rcount_SP << "\n\tStop: " << Rcount_SS << "\n\tTL: " << Rcount_TL << endl;
                }
                Grid[i].push_back(newObject);
            }
        }
   
    }
    ~World() {
        vector<WorldObject*>::iterator it;
        for (it = MCars.begin(); it !=MCars.end(); ++it) {
            delete (*it);
        }
        for (it = MBikes.begin(); it !=MBikes.end(); ++it) {
            delete (*it);
        }
        for (it = SSTOPs.begin(); it !=SSTOPs.end(); ++it) {
            delete (*it);
        }
        for (it = SParkedVechiles.begin(); it !=SParkedVechiles.end(); ++it) {
            delete (*it);
        }
        for (it = STrafficLights.begin(); it !=STrafficLights.end(); ++it) {
            delete (*it);
        }

        MCars.clear();
        MBikes.clear();
        SSTOPs.clear();
        SParkedVechiles.clear();
        STrafficLights.clear();
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


    
    WorldObject* get_WObject(int y, int x) {
        //get the Grid thing at that ps IDK WHAT IM DOING OK?
        
        WorldObject* cur = Grid[y][x];
        return cur;
    }


    void set_thing(WorldObject* ptr, int x, int y) {
        this->Grid[y][x] = ptr;
    }
    void set_thing(WorldObject* ptr, Vector2 V) {
        this->Grid[V.getY()][V.getX()] = ptr;
    }

    WorldObject* get_thing(int x, int y) {
        return Grid[y][x];
    }
    Vector2 get_maxsize() {
        return max_size;
    }

    void Update() {
        vector<WorldObject*>::iterator it;

        for (it = STrafficLights.begin(); it!=STrafficLights.end(); ++it) {
            (*it)->update_traffic_light();
        }



        int i = 0;
        // cout << "where tho\n";
        for (it = MCars.begin(); it!=MCars.end(); ++it) {
            // cout << "it: " << *it << endl;
            // cout << "possibly\n";
            Vector2 move_to = (*it)->get_position()+(*it)->get_direction()*(*it)->get_speed(); 
            // cout << "World Info:\n\tMaxSize: (" << max_size.getX() << ", " << max_size.getY() << ")\n"; 
            Vector2 max_size_copy = max_size;
            max_size_copy = max_size_copy-2;
            if (move_to < 1 || move_to > max_size_copy) {
                // cout << "The problem is in replacement\n";
                // cout << "old: " << MCars.size() << endl;
                set_thing((*it)->get_replace(), (*it)->get_position());
                // cout << "The problem is in erasement\n";
                MCars.erase(MCars.begin()+i);
                it--;
                // cout << "neither\n";
                continue;
                // cout << "new: " << MCars.size();
            }
            Vector2 new_pos = (*it)->get_position()+(*it)->get_direction()*(*it)->get_speed();
            WorldObject* newthing = this->get_thing(new_pos.getX(), new_pos.getY());
            set_thing((*it)->get_replace(), (*it)->get_position());
            (*it)->set_position(new_pos);
            set_thing(*it, new_pos);
            if (newthing->get_symbol() == '@') newthing = new Road(newthing->get_position().getX(), newthing->get_position().getY());
            (*it)->set_replace(newthing);


            i++;
        }
        i = 0;
        // cout << "outside\n";
        for (it = MBikes.begin(); it!=MBikes.end(); ++it) {
            // cout << "did you even get here?\n";
            Vector2 move_to = (*it)->get_position()+(*it)->get_direction()*(*it)->get_speed(); 
            Vector2 max_size_copy = max_size;
            max_size_copy = max_size_copy-2;
            if (move_to < 1 || move_to > max_size_copy) {
                // cout << "The problem is in replacement\n";
                set_thing((*it)->get_replace(), (*it)->get_position());
                // cout << "The problem is in erasing\n";
                MBikes.erase(MBikes.begin()+i);
                it--;
                // cout << "neither\n";
                continue;
            }
            // cout << "no way\n";
            Vector2 new_pos = (*it)->get_position()+(*it)->get_direction()*(*it)->get_speed();
            // cout << "new and old same:"
            WorldObject* newthing = this->get_thing(new_pos.getX(), new_pos.getY());
            set_thing((*it)->get_replace(), (*it)->get_position());
            (*it)->set_position(new_pos);
            set_thing(*it, new_pos);
            if (newthing->get_symbol() == '@') newthing = new Road(newthing->get_position().getX(), newthing->get_position().getY());
            (*it)->set_replace(newthing);

            i++;
        }
        // cout << "done here??\n";
        // cout << "ουπδατε\n";
    }


};

