#include <iostream>
#include <vector>
#include <cmath>
#include "World.h"

using namespace std;


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
        if (CarPosition.getX()+4<=0) {bottomright.setX(GridSize.getX()-1);}
        else {bottomright.setX(CarPosition.getX()+4);}
        if (CarPosition.getY()+4<=0) {bottomright.setY(GridSize.getY()-1);}
        else {bottomright.setY(CarPosition.getY()+4);}
        

        for (int i=topleft.getY(); i<bottomright.getY(); i++) {
            for (int j=topleft.getX(); j<bottomright.getX(); j++) {
                if (i == CarPosition.getY() && j == CarPosition.getX()) continue;
                WorldObject WO = *(W->get_WObject(i,j));
                if (WO.get_symbol() == '.') continue;
                SensorReading newSense;
                Readings.reserve(Readings.size()+1);
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

                Readings.push_back(newSense);
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
            if (CarPosition.getX()+12 >= W->get_maxsize().getX()) {bottomright.setX(W->get_maxsize().getX()-1);}
            else {bottomright.setX(CarPosition.getX()+12);}
        }
        if (CarDirection.getX() == -1 && CarDirection.getY() == 0) { // left
            if (CarPosition.getX()-12 <=0) {topleft.setX(1);}
            else {topleft.setX(CarPosition.getX()-12);}
        }
        if (CarDirection.getX() == 0 && CarDirection.getY() == 1) { // down
            if (CarPosition.getY()+12 >= W->get_maxsize().getY()) {bottomright.setY(W->get_maxsize().getY()-1);}
            else {bottomright.setY(CarPosition.getY()+12);}
        }
        if (CarDirection.getX() == 0 && CarDirection.getY() == -1) { // up
            if (CarPosition.getY()-12 <=0) {topleft.setX(1);}
            else {topleft.setY(CarPosition.getY()-12);}
        }
        
        
        for (int i=topleft.getY(); i<=bottomright.getY(); i++) {
            for (int j=topleft.getX(); j<=bottomright.getX(); j++) {
                if (i == CarPosition.getY() && j == CarPosition.getX()) continue;
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
                bottomright.setX(W->get_maxsize().getX());
            }
            else {
                bottomright.setX(carX+8);
            }
            // height
            if (carY-3 <= 0) {
                topleft.setY(1);
            } else {topleft.setY(carY-3);}
            if (carY+3 >= W->get_maxsize().getY()) {
                bottomright.setY(W->get_maxsize().getY()-1);
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
            if (carY+3 >= W->get_maxsize().getY()) {
                bottomright.setY(W->get_maxsize().getY()-1);
            } else {bottomright.setY(carY+3);}
            

        }  
    
        if (CarDirection.getX() == 0 && CarDirection.getY() == 1) { //down
            if (carY == W->get_maxsize().getY()-1) return;
            //height
            topleft.setY(carY+1);
            if (carY+8>=W->get_maxsize().getY()) {
                bottomright.setY(W->get_maxsize().getY()-1);
            } else {bottomright.setY(carY+8);}
            //width
            if (carX-3<=0) {
                topleft.setX(1);
            } else {topleft.setX(carX-3);}
            if (carX+3>=W->get_maxsize().getX()) {
                bottomright.setX(W->get_maxsize().getX()-1);
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
            if (carX+3>=W->get_maxsize().getX()) {
                bottomright.setX(W->get_maxsize().getX()-1);
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

                Readings.push_back(newSense);
            }
        }

        // vector<SensorReading>::iterator it;
        // for (it = Readings.begin(); it!=Readings.end(); ++it) {
        //     it->print_info();
        // }

    }

    vector<SensorReading> get_Data() {return Readings;}

};

class AutomaticCar {    //ACar

    LidarSensor LS;
    RadarSensor RS;
    CameraSensor CS;

    vector<SensorReading> completed_readings;

    Vector2 CarPosition;
    Vector2 CarDirection;

public:

    AutomaticCar() {
        Vector2 temp(5,5);
        Vector2 temp1(1,0);
        CarPosition = temp;
        CarDirection = temp1;
    }

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


    //imagine that the 3 Sensors have done their little update thing to get readings
    void AC_Update(World* W) {
        //assume random world
        LS.get_Readings(CarPosition, W);
        RS.get_Readings(CarPosition, CarDirection, W);
        CS.get_Readings(CarPosition, CarDirection, W);
        fuseSensorData();

    }


};