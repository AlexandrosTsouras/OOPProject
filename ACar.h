#include <iostream>
#include <vector>
#include "World.h"

using namespace std;

vector<char> problems = {'S', 'P', '?', 'Y', 'R'};
const Vector2 UP(0, -1);
const Vector2 DOWN(0, 1);
const Vector2 LEFT(-1,0);
const Vector2 RIGHT(1,0);

//first make the sensors to put in the car
class SensorReading {
    string type;
    int distance;
    Vector2 position;
    string ID;
    int confidence; //in x%
    int speed;
    Vector2 direction;
    string signText;
    char TrafficLightColor;
public:
    //G for Given
    SensorReading(string typeG = "NULL", int distanceG = 0, string IDG = "XXX", int confidenceG = 0, int speedG = STOPPED, string signTextG = "XXX", char TLCG = 'X')
    :type(typeG), distance(distanceG), position(), ID(IDG), confidence(confidenceG), speed(speedG), direction(), signText(signTextG), TrafficLightColor(TLCG) {}

    //to make it clearer make everything have a set func
    void set_type(const string typeG) {type = typeG;}
    void set_distance(const int distanceG) {distance = distanceG;}
    void set_position(const Vector2 positionG) {position = positionG;}
    void set_ID(const string IDG) {ID = IDG;}
    void set_confidence(const int confidenceG) {confidence = confidenceG;}
    void set_speed(const int speedG) {speed = speedG;};
    void set_direction(const Vector2 directionG) {direction = directionG;}
    void set_signText(const string signTextG) {signText = signTextG;}
    void set_TLC(const char TLCG) {TrafficLightColor = TLCG;}

    const string get_type() {return type;}
    const int get_distance() {return distance;}
    const Vector2 get_position() {return position;}
    const string get_ID() {return ID;}
    const int get_confidence() {return confidence;}
    const int get_speed() {return speed;}
    const Vector2 get_direction() {return direction;}
    const string get_signText() {return signText;}
    const char get_TLC() {return TrafficLightColor;}





    void print_info() {
        cout << "ID: " << ID << endl;
        cout << '\t' << "type: " << type << endl;
        cout << '\t' << "distance: " << distance << endl;
        cout << '\t' << "pos: ";
        position.print_info();
        cout << '\t' << "conf: " << confidence << endl;
        cout << '\t' << "speed: " << speed << endl;
        cout << '\t' << "dir: ";
        direction.print_info();
        cout << '\t' << "sT: " << signText << endl;
        cout << '\t' << "TLC: " << TrafficLightColor << endl;
    }

};


//I don't think LidarSensor needs anything other than the checking lowkey
//9x9
//sees moving-static but only returns type and distance 
class LidarSensor {

    vector<SensorReading> Readings;

public:

    LidarSensor() {}

    void get_Readings(Vector2 CarPosition, World* W) {

        //it checks 9x9 around it so I think
        //first get topleft and bottomright
        Vector2 topleft;
        if (CarPosition.getX()-4<=0) {topleft.setX(1);}
        else {topleft.setX(CarPosition.getX()-4);}
        if (CarPosition.getY()-4<=0) {topleft.setY(1);}
        else {topleft.setY(CarPosition.getY()-4);}
        Vector2 bottomright;
        Vector2 GridSize = W->get_maxsize();
        if (CarPosition.getX()+4>=GridSize.getX()-1) {bottomright.setX(GridSize.getX()-2);}
        else {bottomright.setX(CarPosition.getX()+4);}
        if (CarPosition.getY()+4>=GridSize.getY()-1) {bottomright.setY(GridSize.getY()-2);}
        else {bottomright.setY(CarPosition.getY()+4);}
        

        for (int i=topleft.getY(); i<bottomright.getY(); i++) {
            for (int j=topleft.getX(); j<bottomright.getX(); j++) {
                if (i == CarPosition.getY() && j == CarPosition.getX()) continue;
                WorldObject WO = *(W->get_WObject(i,j));
                if (WO.get_symbol() == '.') continue;
                SensorReading newSense;
                Readings.reserve(Readings.size()+1);
                // cout << "WHERE\n";
                //type
                if (WO.get_symbol() == 'B' || WO.get_symbol() == 'C') {
                    newSense.set_type("Moving");
                } else {newSense.set_type("Static");}
                //distance
                //I don't think using the actual formulas for accuracy is necessary
                //so I'm gonna go with an easier approach
                int dx = abs(CarPosition.getX()-j);
                int dy = abs(CarPosition.getY()-i);
                newSense.set_distance(dx+dy);
                //confidence
                newSense.set_confidence(99); //confidence needs a good rework at the end
                //ID
                newSense.set_ID(WO.get_ID());
                // cout << "Lidar\n";
                Readings.push_back(newSense);
                // cout << "Lidar done\n";
            }
        }

    }

    vector<SensorReading> get_Data() {return Readings;}

};

//12 in front
//sees only moving
class RadarSensor {
    
    vector<SensorReading> Readings;

public:
    RadarSensor() {}


    void get_Readings(Vector2 CarPosition, Vector2 CarDirection, World* W) {

        //gotta get the direction of the car
        Vector2 topleft = CarPosition;
        Vector2 bottomright = CarPosition;
        if (CarDirection.getX() == 1 && CarDirection.getY() == 0) { // right
            if (CarPosition.getX()+12 >= W->get_maxsize().getX()-1) {bottomright.setX(W->get_maxsize().getX()-2);}
            else {bottomright.setX(CarPosition.getX()+12);}
        }
        if (CarDirection.getX() == -1 && CarDirection.getY() == 0) { // left
            if (CarPosition.getX()-12 <=0) {topleft.setX(1);}
            else {topleft.setX(CarPosition.getX()-12);}
        }
        if (CarDirection.getX() == 0 && CarDirection.getY() == 1) { // down
            if (CarPosition.getY()+12 >= W->get_maxsize().getY()-1) {bottomright.setY(W->get_maxsize().getY()-2);}
            else {bottomright.setY(CarPosition.getY()+12);}
        }
        if (CarDirection.getX() == 0 && CarDirection.getY() == -1) { // up
            if (CarPosition.getY()-12 <=0) {topleft.setX(1);}
            else {topleft.setY(CarPosition.getY()-12);}
        }
        // cout << "topleft: (" << topleft.getX() << ", " << topleft.getY() << ")\n";
        // cout << "bottomright: (" << bottomright.getX() << ", " << bottomright.getY() << ")\n";

        
        for (int i=topleft.getY(); i<=bottomright.getY(); i++) {
            // cout << "def not right?\n";
            for (int j=topleft.getX(); j<=bottomright.getX(); j++) {
                // cout << "i: " << i << "\tj: " << j << endl;
                if (i == CarPosition.getY() && j == CarPosition.getX()) continue;
                // cout << "mayhaps?\n";
                WorldObject* WO = W->get_WObject(i,j);
                if (WO->get_ID()[0] == '0') continue; //Every other object other than Moving have 0 as the first char of their ID
                SensorReading newSense;
                Readings.reserve(Readings.size()+1);
                //I know it's not exactly said but since it only gets moving objects might as well set it
                newSense.set_type("Moving");
                //speed
                const int speedToGive = WO->get_speed();
                newSense.set_speed(speedToGive);
                //direction
                const Vector2 dir = WO->get_direction();
                newSense.set_direction(dir);
                //distance
                int dx = abs(CarPosition.getX()-j);
                int dy = abs(CarPosition.getY()-i);
                const int d = dx+dy+1;
                newSense.set_distance(d);
                //confidence
                newSense.set_confidence(55); //random ahh
                //ID
                newSense.set_ID(WO->get_ID());

                //push
                
                Readings.push_back(newSense);
                cout << "perhaps\n";
                // cout << "Nuh uh\n";
                
            }
        }
        // vector<SensorReading>::iterator it;
        // cout << "here?\n";
        // int i=0;
        // for (it = Readings.begin(); it<Readings.end(); ++it) {
        //     cout << i++ << ": ";
        //     it->print_info();
        // }
    }
    
    vector<SensorReading> get_Data() {return Readings;}

};


class CameraSensor {

    vector<SensorReading> Readings;

public:

    CameraSensor() {}

    void get_Readings(Vector2 CarPosition, Vector2 CarDirection, World* W) {

        //7x7 in front so like
        Vector2 topleft;
        Vector2 bottomright;

        //this is gonna be a pain :((((
        int carX = CarPosition.getX();
        int carY = CarPosition.getY();
        if (CarDirection.getX() == 1 && CarDirection.getY() == 0) { //right
            if (carX+1 >= W->get_maxsize().getX()-1) { //car right at the edge (edge case since it would not go right then but whaeva)
                return;
            }
            //width
            topleft.setX(carX+1);
            if (carX+8>=W->get_maxsize().getX()-1) {
                bottomright.setX(W->get_maxsize().getX()-2);
            }
            else {
                bottomright.setX(carX+8);
            }
            // height
            if (carY-3 <= 0) {
                topleft.setY(1);
            } else {topleft.setY(carY-3);}
            if (carY+3 >= W->get_maxsize().getY()-1) {
                bottomright.setY(W->get_maxsize().getY()-2);
            } else {bottomright.setY(carY+3);}
        }

        if (CarDirection.getX() == -1 && CarDirection.getY() == 0) { //left
            if (carX == 1) return; //edge case ig
            //width
            bottomright.setX(carX-1);
            if (carX-8<=0) {
                topleft.setX(1);
            } else {topleft.setX(carX-8);}
            // height
            if (carY-3 <= 0) {
                topleft.setY(1);
            } else {topleft.setY(carY-3);}
            if (carY+3 >= W->get_maxsize().getY()-1) {
                bottomright.setY(W->get_maxsize().getY()-2);
            } else {bottomright.setY(carY+3);}
            

        }  
    
        if (CarDirection.getX() == 0 && CarDirection.getY() == 1) { //down
            if (carY == W->get_maxsize().getY()-1) return;
            //height
            topleft.setY(carY+1);
            if (carY+8>=W->get_maxsize().getY()-1) {
                bottomright.setY(W->get_maxsize().getY()-2);
            } else {bottomright.setY(carY+8);}
            //width
            if (carX-3<=0) {
                topleft.setX(1);
            } else {topleft.setX(carX-3);}
            if (carX+3>=W->get_maxsize().getX()-1) {
                bottomright.setX(W->get_maxsize().getX()-2);
            } else {bottomright.setX(carX+3);}
        }

        if (CarDirection.getX() == 0 && CarDirection.getY() == -1) { //up
            if (carY == 1) return;
            //height
            bottomright.setY(1);
            if (carY-8 <=0 ) {
                topleft.setY(1);
            } else {topleft.setY(carY-8);}
            //width
            if (carX-3<=0) {
                topleft.setX(1);
            } else {topleft.setX(carX-3);}
            if (carX+3>=W->get_maxsize().getX()-1) {
                bottomright.setX(W->get_maxsize().getX()-2);
            } else {bottomright.setX(carX+3);}
        }


        // cout << "Checking from (" << topleft.getX() << ", " << topleft.getY() << ") to (" << bottomright.getX() << ", " << bottomright.getY() << ")" << endl;

        for (int i=topleft.getY(); i<=bottomright.getY(); i++) {
            for (int j = topleft.getX(); j<=bottomright.getX(); j++) {
                // cout << "Now Checking (" << j << ", " << i << ")" << endl;
                WorldObject* WO = (W->get_WObject(i, j));
                if (WO->get_symbol() == '.') continue;

                Readings.reserve(Readings.size()+1);
                SensorReading newSense;

                //type
                if (WO->get_ID()[0] == '1') {newSense.set_type("Moving");}
                else {newSense.set_type("Static");}
                //distance
                int dx = abs(carX-j);
                int dy = abs(carY-i);
                newSense.set_distance(dx+dy+1);
                //position
                newSense.set_position(WO->get_position());
                //ID
                newSense.set_ID(WO->get_ID());
                //confidence
                newSense.set_confidence(67);
                //speed
                newSense.set_speed(WO->get_speed());
                //direction
                newSense.set_direction(WO->get_direction());
                //signText
                newSense.set_signText(WO->get_signText());
                //TLC
                newSense.set_TLC(WO->get_TLC());
                // cout << "Camera\n";
                Readings.push_back(newSense);
                // cout << "Camera done\n";
            }
        }

        // vector<SensorReading>::iterator it;
        // for (it = Readings.begin(); it!=Readings.end(); ++it) {
        //     it->print_info();
        // }

    }

    vector<SensorReading> get_Data() {return Readings;}

};



class AutomaticCar: public WorldObject {    //ACar

    LidarSensor LS;
    RadarSensor RS;
    CameraSensor CS;

    vector<SensorReading> completed_readings;


    int speed;
    Vector2 CarPosition;
    Vector2 CarDirection;
    static char symbol;
    Vector2 final_destination;
    bool done;

public:


    void print() {
        cout << "Position: (" << CarPosition.getX() << ", " << CarPosition.getY() << ")\n";
        cout << "Direction: (" << CarDirection.getX() << ", " << CarDirection.getY() << ")\n";
        cout << "Speed: " << speed << endl;
    }

    AutomaticCar(World* W, Vector2 D)
    :WorldObject("-1", '@', 0, 0) {
        this->final_destination = D;
        this->speed = HALF_SPEED;
        done = false;
        for (int i = 1; i<W->get_maxsize().getY()-1; i++) {
            for (int j=1; j<W->get_maxsize().getX()-1; j++) {
                if (W->get_thing(j, i)->get_ID() == "-1") {
                    CarPosition.setX(j);
                    CarPosition.setY(i);
                    if (j < final_destination.getX()) CarDirection = RIGHT;
                    else CarDirection = LEFT;

                    this->set_position(CarPosition);
                    W->set_thing(this, j, i);
                }
            }
        }



    }

    void accelerate() {
        if (speed >= FULL_SPEED) {speed = FULL_SPEED; return;} 
        speed++;
        return;}
    void decelerate() {
        if (speed <=STOPPED) {speed = STOPPED; return;}
        speed--;
        return;}
    void turn(string turn = "NaN") {
        if (CarDirection.getX() == 1) {
            if (turn == "left"){
                CarDirection = UP;
            }
            else if (turn == "right") {
                CarDirection = DOWN;   
            }
        }
        else if (CarDirection.getX() == -1) {
            if (turn == "left") {
                    CarDirection = DOWN;
                }
            else if (turn == "right") {
                    CarDirection = UP;
                }
            
        }
        else if (CarDirection.getY() == 1) {
                if (turn == "left") {
                    CarDirection = RIGHT;
                    }
                else if (turn == "right"){
                    CarDirection = LEFT;
                    }
            
        }
        else if (CarDirection.getY() == -1) {
            if (turn == "left"){
                    CarDirection = LEFT;
                    }
            else if (turn == "right"){
                    CarDirection = RIGHT;
                    }

        }
        
        return;}
    //
    void turn(Vector2 V) {


        if (CarPosition.getX() == final_destination.getX()) {
            if (CarPosition.getY() < final_destination.getY()) {
                CarDirection = DOWN;
            }
            else {CarDirection = UP;}
            return;
        }
        else if (CarPosition.getY() == final_destination.getY()) {
            if (CarPosition.getX() < final_destination.getX()) {
                CarDirection = RIGHT;
            }
            else {CarDirection = LEFT;}
            return;
        }

        if (CarDirection == RIGHT) {
            if (CarPosition.getY() < V.getY()) {
                turn("right");
            } else {turn("left");}
            return;
        }
        if (CarDirection == LEFT) {
            if (CarPosition.getY() < V.getY()) {
                turn("left");
            } else {turn("right");}
            return;
        }if (CarDirection == UP) {
            if (CarPosition.getX() < V.getX()) {
                turn("right");
            } else {turn("left");}
            return;
        }if (CarDirection == DOWN) {
            if (CarPosition.getX() < V.getX()) {
                turn("left");
            } else {turn("right");}
            return;
        }}
    //
    void turn_to(Vector2 pos) {
        if (CarPosition.getX() == pos.getX()) {
            if (CarPosition.getY() < pos.getY()) CarDirection = DOWN;
            else CarDirection = UP;
        }
        else if (CarPosition.getY() == pos.getY()) {
            if (CarPosition.getX() < pos.getX()) CarDirection = RIGHT;
            else CarDirection = LEFT;
        }
    }
    //
    
    
    void fuseSensorData() {
        //There are defnitely better ways to go about it but I think a triple loop will be my go to
        vector<SensorReading>::iterator it0;  //Camera Sensor
        vector<SensorReading>::iterator it1;  //Lidar Sensor
        vector<SensorReading> CS_Data = CS.get_Data();
        vector<SensorReading> LS_Data = LS.get_Data();
        vector<SensorReading> RS_Data = RS.get_Data();
        
        //first insert the "everything"
        for (it0 = CS_Data.begin(); it0 != CS_Data.end(); ++it0) {
            completed_readings.push_back(*it0);
            // cout << "Found through Camera: ";
            // (--completed_readings.end())->print_info();
        }

        //then add the "most something"
        for (it1 = LS_Data.begin(); it1 != LS_Data.end(); ++it1) {
            bool found = false;
            for (it0 = completed_readings.begin(); it0 != completed_readings.end(); ++it0) {
                if (it0->get_ID() == it1->get_ID()) {found = true; break;}
            }
            if (!found) {completed_readings.push_back(*it1);
                // cout << "Found through Lidar: ";
                // (--completed_readings.end())->print_info();
            }
        }

        //add the "least something"
        for (it1 = RS_Data.begin(); it1 != RS_Data.end(); ++it1) {
            bool found = false;
            for (it0 = completed_readings.begin(); it0 != completed_readings.end(); ++it0) {
                if (it0->get_ID() == it1->get_ID()) {
                    //there's 2 cases
                    // 1] it was seen by the Camera Sensor
                    //        •Everything is already set and needs nothing
                    // 2] it was seen by the Lidar Sensor
                    //        •Lidar and Radar both have distance and confidence, while type is definite
                    //        •So only speed and direction is needed
                    it0->set_speed(it1->get_speed());
                    it0->set_direction(it1->get_direction());
                    found == true;
                }
            
            }
            if (!found) {completed_readings.push_back(*it1);
                // cout << "Found through Radar: ";
                // (--completed_readings.end())->print_info();
            }
        }

        // for (it0 = completed_readings.begin(); it0 != completed_readings.end(); ++it0) {
        //     it0->print_info();
        // }
    }
    

    bool force(Vector2 W_maxsize) {
        
        bool decelerated = false;
        bool forced = false;
        
        vector<SensorReading>::iterator it;
        for (it = completed_readings.begin(); it != completed_readings.end(); ++it) {
            //traffic lights
            if ((it->get_TLC() == 'Y' || it->get_TLC() == 'R') && in_front(CarPosition, CarDirection, it->get_position()) && d(CarPosition, it->get_position()) < 6) {
                if (speed == FULL_SPEED) {decelerate(); decelerated = true; forced = true;
                cout << "forced for light\n";
                }
            }
            if (it->get_TLC() == 'Y' || it->get_TLC() == 'R' && right_in_front(CarPosition, CarDirection, it->get_position()))
            {
                decelerate();
                decelerated = true;
                forced = true;
            }

            //parked vechile
            if (it->get_TLC() == 'X' && it->get_signText() == "EMPTY" && right_in_front(CarPosition,CarDirection, it->get_position())) {
                decelerate();
                decelerated = true;
            }
            if (it->get_TLC() == 'X' && it->get_signText() == "EMPTY" && right_in_front(CarPosition,CarDirection, it->get_position())) {
                speed = STOPPED;
                decelerated = true;
                //edge cases
                if (CarPosition.getX() == 1) {if (CarDirection == UP) {turn("right");} else {turn("left");}}
                else if (CarPosition.getX() == W_maxsize.getX()-1) {if (CarDirection == UP) turn("left"); else {turn("right");}}
                else if (CarPosition.getY() == 1) {if (CarDirection == RIGHT) {turn("right");} else {turn("left");}}
                else if (CarPosition.getX() == W_maxsize.getY()-1) {if (CarDirection == RIGHT) turn("left"); else {turn("right");}}
                else {turn("right"); cout << "turnt right for the right reasons\n";}
                forced = true;
                cout << "forced for parked\n";
            }

            //moving things close
            if (it->get_ID()[0] == '1') {
                Vector2 topleft;
                Vector2 bottomright;
                //setX
                if (CarDirection == UP || CarDirection == DOWN) {
                    if (CarPosition.getX() == 1) {topleft.setX(1); bottomright.setX(CarPosition.getX()+1);}
                    else if (CarPosition.getX() == W_maxsize.getX()-2) {topleft.setX(CarPosition.getX()-1); bottomright.setX(W_maxsize.getX()-2);}
                    else {topleft.setX(CarPosition.getX()-1); bottomright.setX(CarPosition.getX()+1);}
                }
                else if (CarDirection == RIGHT) {
                    if (CarPosition.getX() == W_maxsize.getX()-2) {topleft.setX(-1); bottomright.setX(-2);}
                    else if (CarPosition.getX() == W_maxsize.getX()-3) {topleft.setX(W_maxsize.getX()-2); bottomright.setX(W_maxsize.getX()-2);}
                    else {topleft.setX(CarPosition.getX()+1); bottomright.setX(CarPosition.getX()+2);}
                }
                else if (CarDirection == LEFT) {
                    if (CarPosition.getX() == 1) {topleft.setX(-1); bottomright.setX(-2);}
                    else if (CarPosition.getX() == 2) {topleft.setX(1); bottomright.setX(1);}
                    else {topleft.setX(CarPosition.getX()-2); bottomright.setX(CarPosition.getX()-1);}
                }
                //setY
                if (CarDirection == RIGHT || CarDirection == LEFT) {
                    if (CarPosition.getY() == 1) {topleft.setY(1); bottomright.setY(2);}
                    else if (CarPosition.getY() == W_maxsize.getY()-2) {topleft.setY(W_maxsize.getY()-3); bottomright.setY(W_maxsize.getY()-2);}
                    else {topleft.setY(CarPosition.getY()-1); bottomright.setY(CarPosition.getY()+1);}
                }
                else if (CarDirection == UP) {
                    if (CarPosition.getY() == 1) {topleft.setY(-1); bottomright.setY(-2);}
                    else if (CarPosition.getY() == 2) {topleft.setY(1); bottomright.setY(1);}
                    else {topleft.setY(CarPosition.getY()-2); bottomright.setY(CarPosition.getY()-1);}
                }
                else if (CarDirection == DOWN) {
                    if (CarPosition.getY() == W_maxsize.getY()-2) {topleft.setY(-1); bottomright.setY(-2);}
                    else if (CarPosition.getY() == W_maxsize.getY()-3) {topleft.setY(W_maxsize.getY()-2); bottomright.setY(W_maxsize.getY()-2);}
                    else {topleft.setY(CarPosition.getY()+2); bottomright.setY(CarPosition.getY()+1);}
                }
                cout << "DIR: ";
                CarDirection.print_info();
                cout << "\nfrom:";
                topleft.print_info();
                cout << " to: ";
                bottomright.print_info();
                cout << within(it->get_position(), topleft, bottomright) << endl; 
                if (within(it->get_position(), topleft, bottomright)) {
                    // decelerate();
                    speed = STOPPED;
                    decelerated = true;
                    forced = true;
                    cout << "decelerated for Car/Bike\n";
                }
            }

            //close to gps location whtatever
            if (d(CarPosition, final_destination)<6 && !forced) {speed = HALF_SPEED; forced = true; turn(final_destination);
            cout << "forced for close\n";}

        }
        return forced;
    }


    void execute(World* W) {
        
        
        bool forced = force(W->get_maxsize());
        if (!forced) {
            turn(final_destination);
            accelerate();
        } else {cout << "forced something\n";}
        // cout << "X: " << CarPosition.getX()+CarDirection.getX()*speed << "Y: " << CarPosition.getY()+CarDirection.getY()*speed << endl;
        WorldObject* newroad = new Road(CarPosition.getX(), CarPosition.getY());
        // cout << "CarDir: " << CarDirection.getX() << ", " << CarDirection.getY() << endl;
        int newCX = CarPosition.getX()+CarDirection.getX()*speed;
        int newCY = CarPosition.getY()+CarDirection.getY()*speed;
        // cout << "newC: (" << newCX << ", " << newCY << ")\n"; 
        if (newCX >= W->get_maxsize().getX()-1 || newCY >= W->get_maxsize().getY()-1) {
            // cout << "Do you come in here?\n";
            turn(final_destination);
            newCX = CarPosition.getX()+CarDirection.getX()*speed;
            newCY = CarPosition.getY()+CarDirection.getY()*speed;
            // cout << "newpos: (" << newCX << ", " << newCY << ")\n"; 
            
        }
        CarPosition.setX(newCX);
        CarPosition.setY(newCY);
        set_position(CarPosition);
        // cout << "X: " << CarPosition.getX()+CarDirection.getX()*speed << "\nY: " << CarPosition.getY()+CarDirection.getY()*speed << endl;
        W->set_thing(newroad, newroad->get_position().getX(), newroad->get_position().getY());
        W->set_thing(this, CarPosition.getX(), CarPosition.getY());
    }

    //imagine that the 3 Sensors have done their little update thing to get readings
    void Full_Update(World* W) {
        //assume random world
        // cout << "A\n";
        cout << endl << endl << endl;
        W->Update();
        // cout << "huh???\n";
        LS.get_Readings(CarPosition, W);
        RS.get_Readings(CarPosition, CarDirection, W);
        // cout << "I am confusion\n";
        CS.get_Readings(CarPosition, CarDirection, W);
        // cout << "here\n";
        fuseSensorData();
        execute(W);
        W->print_world();
        completed_readings.clear();
        // print();

        if (CarPosition == final_destination) done = true;
    }

    //ignore
    void set_symbol() {symbol = '@'; return;}
    bool get_done() {return done;}
};

