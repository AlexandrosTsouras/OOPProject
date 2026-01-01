#include <iostream>
#include <vector>
#include "Vector2.h"
#include <ctime>
#include <cstdlib>

using namespace std;

const int STOPPED = 0;
const int HALF_SPEED = 1;
const int FULL_SPEED = 2;


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
        cout << "\tID:" << ID << "\n\tPosition: (" << position.getX() << ", " << position.getY() << ")\n";
    }

    virtual void set_symbol(char c) {symbol = c;}
    void set_position(Vector2 V) {position = V;}
    void set_ID(string s) {ID = s;}
    virtual void set_speed(int n) {return;}

   

    //GETS
    char get_symbol() {return symbol;}
    string get_ID() {return ID;}
    Vector2 get_position() {return position;}

    //for moving object
    virtual int get_speed() {return -1;}
    virtual Vector2 get_direction() {Vector2 empty(-2); return empty;}
    //for everything else
    virtual string get_signText() {return "EMPTY";}
    virtual char get_TLC() {return 'X';} //we love alt code


    //not sure
    WorldObject* get_Object(WorldObject* W) {
        return W;
    }
};

class MovingObject: public WorldObject {
    Vector2 direction;
    int speed;
    public:
    static string count;
    MovingObject(char ch, int x,int y)
    :WorldObject(count, ch, x, y), speed(HALF_SPEED) {
        srand(time(NULL));
        int num = rand() % 4;
        switch(num) {
            case 0:
                direction.setX(1);
                direction.setY(0);
                break;
            case 1:
                direction.setX(-1);
                direction.setY(0);
                break;
            case 2:
                direction.setX(0);
                direction.setY(1);
                break;
            case 3:
                direction.setX(0);
                direction.setY(-1);
                break;
        }

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
        cout << "\tDirection: (" << direction.getX() << ", " << direction.getY() << ")\n";
        cout << "\tSpeed: " << speed << endl;        
    }

    virtual void print_object() {
        WorldObject::print_object();
    }

    void set_speed(int n) override {speed = n;}

    int get_speed() override {return speed;}
    Vector2 get_direction() override {return direction;}
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

        // char get_symbol() {return WorldObject::get_symbol();}
        // virtual string get_signText();
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

            string get_signText() override {return SignText;}
        };
/////////////////////////////////////
                            class STOPSign: public Sign {
                            public:
                                STOPSign(int x, int y)
                                :Sign('S', x, y, "STOP") {}

                                void print_object() override {
                                    
                                    Sign::print_object();
                                }

                                void print_info() override {
                                    cout << "StopSign:\n";
                                    Sign::print_info();
                                }

                                string get_signText() {return Sign::get_signText();}

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
                cout << endl;
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

            char get_TLC() override {
                return StaticObject::get_symbol();
            }

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
class CarReservation: public WorldObject {

public:
    CarReservation(int x, int y)
    :WorldObject("-1", '@', x, y) {}
};

string MovingObject::count = "100";
string StaticObject::count = "000";
