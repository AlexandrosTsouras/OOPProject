#include <iostream>
#include <vector>
#include "Vector2.h"
#include <ctime>
#include <cstdlib>

using namespace std;
#define STOPPED 0
#define HALF_SPEED 1
#define FULL_SPEED 2 


class WorldObject {
    string ID;
    char symbol;
protected:
    Vector2 position;
public:
    WorldObject(string id, char s, int x, int y)
    :ID(id), symbol(s), position(x,y) {}

    virtual void print_object() {
        cout << symbol;
    }

    virtual void print_info() {
        cout << "\tID:" << ID << "\n\tPosition: (" << position.getX() << ", " << position.getY() << ")\n\t";
    }

    virtual void set_symbol(char c) {symbol = c;}

    char get_symbol() {return symbol;}

};

class MovingObject: public WorldObject {
   Vector2 direction;
    int speed;
    public:
    static string count;
    MovingObject(char ch, int x,int y)
    :WorldObject(count, ch, x, y), /**CHECK, NOT READY **/ speed(HALF_SPEED), direction(1,0) {
        if (count[count.size()-1]-'0' == 9) {
            count[count.size()-2]+=1;
            count[count.size()-1] = '0';
        }
        else {
            count[count.size()-1]+=1;
        }
    }
    ~MovingObject() {}

    virtual void print_info() {
        WorldObject::print_info();
        cout << "Direction: (" << direction.getX() << ", " << direction.getY() << ")\n\t";
        cout << "Speed: " << speed << endl;        
    }

    virtual void print_object() {
        WorldObject::print_object();
    }

};
//////////////
            class MovingCar: public MovingObject {
            public:
                MovingCar(int x, int y) 
                :MovingObject('C', x, y) {}

                void print_info() override {
                    cout << "Moving Car" << endl;
                    MovingObject::print_info();
                }

                void print_object() override {
                    MovingObject::print_object();
                }

            };

//////////////////
            class MovingBike: public MovingObject {
            public:
                MovingBike(int x, int y) 
                :MovingObject('B', x, y) {}

                void print_info() override {
                    cout << "Moving Bike" << endl;
                    MovingObject::print_info();
                }

                void print_object() override {
                    MovingObject::print_object();
                }
            };
/////////////////////////////////////////////////////////////////////////////
class StaticObject: public WorldObject {
    public:
    static string count;
        StaticObject(char c, int x, int y)
        :WorldObject(count, c, x, y) {
            if (count[count.size()-1]-'0' == 9) {
                count[count.size()-2]+=1;
                count[count.size()-1] = '0';
            }
            else {
                count[count.size()-1]+=1;
            }
        }

        virtual void print_info() {
            WorldObject::print_info();
        }

        void set_symbol(char c) override {WorldObject::set_symbol(c);}

        virtual void print_object() {WorldObject::print_object();}
    };
///////////////////////////////////////
        class Sign: public StaticObject {
            string SignText;
        public:
            Sign(char c, int x, int y, string text)
            :StaticObject(c, x, y), SignText(text) {}

            virtual void print_info() {
                StaticObject::print_info();
                cout << "\tSignText: " << SignText << endl;
            }

            virtual void print_object() {StaticObject::print_object();}
        };
/////////////////////////////////////
                            class STOPSign: public Sign {
                            public:
                                STOPSign(int x, int y)
                                :Sign('S', x, y, "STOP") {}

                                void print_object() override {Sign::print_object();}
                            };
/////////////////////////////
        class ParkedVechile: public StaticObject {
        public:
            ParkedVechile(int x,int y)
            :StaticObject('P', x, y) {}

            void print_info() override {
                cout << "Parked Vechile:" << endl;
                StaticObject::print_info();
            }

            void print_object() override {StaticObject::print_object();}
        };
///////////////////////////
        class TrafficLight: public StaticObject {
            int ticks_passed;
        public:
            TrafficLight(int x,int y)
            :StaticObject('X', x, y) {
                srand(time(NULL));
                int r = rand() % 3;
                switch(r) {
                    case 0: StaticObject::set_symbol('R'); break;
                    case 1: StaticObject::set_symbol('Y'); break;
                    case 2: StaticObject::set_symbol('G'); break;
                }
            }

            void print_info() override {
                cout << "Traffic Light:" << endl;
                StaticObject::print_info();
            }
        
            void update_traffic_light() {
                if (WorldObject::get_symbol() == 'R' && ticks_passed==4) {
                    StaticObject::set_symbol('G');
                    return;
                }
                if (WorldObject::get_symbol() == 'Y' && ticks_passed==2) {
                    StaticObject::set_symbol('R');
                    return;
                }
                if (WorldObject::get_symbol() == 'G' && ticks_passed==8) {
                    StaticObject::set_symbol('Y');
                    return;
                }
            }


            void print_object() override {StaticObject::print_object();}

        };

class Border: public WorldObject {
public:
    Border(int x, int y)
    :WorldObject("0", 'X', x, y) {}

    void print_object() override {WorldObject::print_object();}

};

class Road: public WorldObject {
public:
    Road(int x, int y)
    :WorldObject("0", '.', x, y) {}

    void print_object() override {WorldObject::print_object();}

};


string MovingObject::count = "100";
string StaticObject::count = "000";
