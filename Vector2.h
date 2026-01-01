#include <iostream>
#include <cmath>
using namespace std;

class Vector2 {
private:    
    int X; // left-right
    int Y; // up-down
public:

    Vector2(int x = -1, int y = -1)
    :X(x), Y(y) {}
    
    Vector2(const Vector2& given)
    {
        X = given.X;
        Y = given.Y;
    }

    // Vector2(int a):X(a), Y(a) {}

    Vector2 operator=(const Vector2& Vector_Given) {
        
        this->X = Vector_Given.X;
        this->Y = Vector_Given.Y;

        return *this;
    }
    Vector2 operator+(const Vector2& Vector_Given) {
        this->X += Vector_Given.X;
        this->Y += Vector_Given.Y;
        return *this;
    }
    Vector2 operator-(const Vector2& Vector_Given) {
        this->X = this->X - Vector_Given.X;
        this->Y = this->Y - Vector_Given.Y;
        
        return *this;
    }
    Vector2 operator-(const int& n) {
        this->X = this->X - n;
        this->Y = this->Y - n;   
        return *this;
    }
    Vector2 operator*(const Vector2& Vector_Given) {
        this->X = this->X*Vector_Given.X;
        this->Y = this->Y*Vector_Given.Y;
        return *this;
    }
    Vector2 operator*(const int& n) {
        this->X = this->X*n;
        this->Y = this->Y*n;
        return *this;
    }
    bool operator<(const int& n) {
        if (this->X < n || this->Y < n) return true; //edge case equals is impossible at least in theory :(
        else return false;
    }
    bool operator>(const int& n) {
        if (this->X > n || this->Y > n) return true; //edge case equals is impossible at least in theory :(
        else return false;
    }
    bool operator>(const Vector2& V) {
        if (this->X > V.X || this->Y > V.Y) return true; //edge case equals is impossible at least in theory :(
        else return false;
    }
    bool operator==(const Vector2& V) {
        if (this->X == V.X && this->Y == V.Y) return true;
        else return false;
    }
    
    
    int getX() {return X;}
    int getY() {return Y;}
    void setX(int x) {X = x;}
    void setY(int y) {Y = y;}

    void print_info() {
        cout << "(" << X << ", " << Y << ")" << endl;
    }

    
    
};



int dX(Vector2 V1, Vector2 V2) {
    return abs(V1.getX()-V2.getX());
}
int dY(Vector2 V1, Vector2 V2) {
    return abs(V1.getY()-V2.getY());
}
int d(Vector2 V1, Vector2 V2) {
    return (dX(V1,V2)+dY(V1,V2));
}
bool in_front(Vector2 pos1, Vector2 dir, Vector2 pos2) {
    if (pos1.getX() != pos2.getX() && pos1.getY() != pos2.getY()) return false;
    Vector2 temp = (pos1-pos2)*dir;
    cout << "temp:" << temp.getX() << ',' << temp.getY() << endl;
    return temp<0;
}

bool right_in_front(Vector2 pos1, Vector2 dir, Vector2 pos2) {
    return (in_front(pos1, dir, pos2) && d(pos1,pos2) == 1);
}

bool within(Vector2 V, Vector2 lim1, Vector2 lim2) {
    bool rX = V.getX() >= lim1.getX() && V.getX() <= lim2.getX();
    bool rY = V.getY() >= lim1.getY() && V.getY() <= lim2.getY();
    return (rX && rY);
}