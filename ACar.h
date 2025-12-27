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
    :type(typeG), distance(distanceG), position(0,0), ID(IDG), confidence(confidenceG), speed(speedG), direction(0,0), signText(signTextG), TrafficLightColor(TLCG) {}

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

};


//I don't think LidarSensor needs anything other than the checking lowkey
class LidarSensor {

    vector<SensorReading> Readings;

public:

    LidarSensor() {}

    void get_Readings(Vector2 CarPosition, World W) {

        //it checks 9x9 around it so I think
        //first get topleft and bottomright
        Vector2 topleft;
        if (CarPosition.getX()-4<=0) {topleft.setX(1);}
        else {topleft.setX(CarPosition.getX()-4);}
        if (CarPosition.getY()-4<=0) {topleft.setY(1);}
        else {topleft.setY(CarPosition.getY()-4);}
        
        Vector2 bottomright;
        Vector2 GridSize = W.get_maxsize();
        if (CarPosition.getX()+4<=0) {bottomright.setX(GridSize.getX()-1);}
        else {bottomright.setX(CarPosition.getX()+4);}
        if (CarPosition.getY()+4<=0) {bottomright.setY(GridSize.getY()-1);}
        else {bottomright.setY(CarPosition.getY()+4);}
        

        for (int i=topleft.getY(); i<bottomright.getY(); i++) {
            for (int j=topleft.getX(); j<bottomright.getX(); j++) {
                if (i == CarPosition.getY() && j == CarPosition.getX()) continue;
                WorldObject WO = W.get_WObject(i,j);
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

                Readings.push_back(newSense);
            }
        }

    }

};

class RadarSensor {
    
    vector<SensorReading> Readings;

public:
    RadarSensor() {}


    void get_Readings(Vector2 CarPosition, Vector2 CarDirection, World W) {

        //gotta get the direction of the car
        Vector2 topleft = CarPosition;
        Vector2 bottomright = CarPosition;
        if (CarDirection.getX() == 1 && CarDirection.getY() == 0) { // right
            if (CarPosition.getX()+12 >= W.get_maxsize().getX()) {bottomright.setX(W.get_maxsize().getX()-1);}
            else {bottomright.setX(CarPosition.getX()+12);}
        }
        if (CarDirection.getX() == -1 && CarDirection.getY() == 0) { // left
            if (CarPosition.getX()-12 <=0) {topleft.setX(1);}
            else {topleft.setX(CarPosition.getX()-12);}
        }
        if (CarDirection.getX() == 0 && CarDirection.getY() == 1) { // down
            if (CarPosition.getY()+12 >= W.get_maxsize().getY()) {bottomright.setY(W.get_maxsize().getY()-1);}
            else {bottomright.setY(CarPosition.getY()+12);}
        }
        if (CarDirection.getX() == 0 && CarDirection.getY() == -1) { // up
            if (CarPosition.getY()-12 <=0) {topleft.setX(1);}
            else {topleft.setY(CarPosition.getY()-12);}
        }
        
        
        for (int i=topleft.getY(); i<=bottomright.getY(); i++) {
            for (int j=topleft.getX(); j<=bottomright.getX(); j++) {
                if (i == CarPosition.getY() && j == CarPosition.getX()) continue;
                cout << "it's here right?\n";
                WorldObject WO = W.get_WObject(i,j);
                cout << "Hello?" << endl;
                if (WO.get_ID()[0] == '0') continue; //Every other object other than Moving have 0 as the first char of their ID
                SensorReading newSense;
                Readings.reserve(Readings.size()+1);
                //speed
                int speedToGive = WO.get_speed();
                
                cout << speedToGive << endl;
                Readings.push_back(newSense);
            }
        }

    }
};