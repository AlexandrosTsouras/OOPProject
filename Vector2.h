#include <iostream>
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

    int getX() {return X;}
    int getY() {return Y;}
    void setX(int x) {X = x;}
    void setY(int y) {Y = y;}

    void print_info() {
        cout << "(" << X << ", " << Y << ")" << endl;
    }

};