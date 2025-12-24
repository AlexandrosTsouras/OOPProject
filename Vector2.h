
using namespace std;

class Vector2 {
private:    
    int posX; // left-right
    int posY; // up-down
public:

    Vector2(int x, int y)
    :posX(x), posY(y) {}

    Vector2 operator=(const Vector2& V) {
        Vector2 newV(V.posX, V.posY);
        return newV;
    }

    int getX() {return posX;}
    int getY() {return posY;}
    void setX(int x) {posX = x;}
    void setY(int y) {posY = y;}
};