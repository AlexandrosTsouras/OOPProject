
using namespace std;

class Vector2 {
private:    
    int posX; // left-right
    int posY; // up-down
public:

    Vector2(int x = -1, int y = -1)
    :posX(x), posY(y) {}
    

    int getX() {return posX;}
    int getY() {return posY;}
    void setX(int x) {posX = x;}
    void setY(int y) {posY = y;}
};