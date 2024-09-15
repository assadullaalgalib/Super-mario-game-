#include <windows.h>
#include <GL/glut.h>
#include <iostream>
#include <vector>
#include<cmath>
using namespace std;

//........................Global Variable Start.................................

#define charSize 60.0f
#define charInitialX 0.0f
#define charInitialY 100.0f
#define initialSpeed 7.0f
#define initialVelocity  21.0f
#define gravity 1.0f
#define respawnDelay 0
#define windowInitialX 0.0f
#define windowInitialY 0.0f
#define windowFinalX 1280.0f
#define windowFinalY 720.0f
#define coordinateX 1280
#define coordinateY 720
#define PI 3.14159265358979323846

//...........................Global Variable end................................

int score = 0;
int lives = 2;
float deadPosition=0.0;
int obstacleClearCount =3;

bool isStage1 = false;
bool isStage2 = false;
bool isStage3 = false;
bool allStage = false;
bool winner = false;

//.......................................Menu Start.................................

// Function to Use SPACE BAR
void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 13: // Enter
        isStage1 = true;
        glutPostRedisplay(); // Mark the window for a redraw
        break;

    case 50: // Numpad 2 key
        isStage2 = true;
        glutPostRedisplay();
        break;

    case 51: // Numpad 3 key
        isStage3 = true;
        glutPostRedisplay();
        break;

    case 27: // 'Esc' key
        exit(0);
        break;
    }
}

void drawText(const string& text, int x, int y, void* font, float r, float g, float b) {
    glColor3f(r, g, b);
    glRasterPos2i(x, y);

    for (char c : text) {
        glutBitmapCharacter(font, c);
    }
}

void menu(){
    glBegin(GL_QUADS);
    glColor3f(0.686, 0.804, 0.804);
    glVertex2f(500.0f, 483.0f);
    glVertex2f(500.0f, 535.0f);
    glVertex2f(785.0f, 535.0f);
    glVertex2f(785.0f, 483.0f);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glLineWidth(3.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(500.0f, 483.0f);
    glVertex2f(500.0f, 535.0f);
    glVertex2f(785.0f, 535.0f);
    glVertex2f(785.0f, 483.0f);
    glEnd();

    drawText("SUPER MARIO", 560, 500, GLUT_BITMAP_TIMES_ROMAN_24, 0.0f, 0.0f, 1.0f);
    drawText("START NEW GAME", 532, 420, GLUT_BITMAP_TIMES_ROMAN_24,0.337, 0.051, 0.051);
    drawText("Press 'Enter' to Start New Game", 475, 380, GLUT_BITMAP_TIMES_ROMAN_24, 0.580, 0, 0);
    drawText("Press '2' to Start STAGE 02", 500, 350, GLUT_BITMAP_TIMES_ROMAN_24, 0.580, 0, 0);
    drawText("Press '3' to Start STAGE 03", 500, 320, GLUT_BITMAP_TIMES_ROMAN_24, 0.580, 0, 0);
    drawText("ALL RIGHTS RESERVED © 2023", 470, 190, GLUT_BITMAP_TIMES_ROMAN_24, 1, 0, 0);

    drawText("FAYSAL AHMMED(22-47069-1)", 460, 160, GLUT_BITMAP_TIMES_ROMAN_24, 0.380, 0.188, 0.573);
    drawText("ASEF RAHMAN ANTIK (22-47106-1)", 430, 130, GLUT_BITMAP_TIMES_ROMAN_24, 0.380, 0.188, 0.573);
    drawText("ABIR DAS (22-47079-1)", 510, 100, GLUT_BITMAP_TIMES_ROMAN_24, 0.380, 0.188, 0.573);
    drawText("MD. ASSADULLA AL GALIB (22-46697-1)", 400, 70, GLUT_BITMAP_TIMES_ROMAN_24, 0.380, 0.188, 0.573);
}


//.............................................Menu End.................................................

//............................................Character Start.........................................

class Character {
private:
    float x;
    float y;
    float speed;
    float size;


public:
    float jumpVelocity;
    float currentJumpVelocity;
    bool isJumping;
    bool moveLeftFlag;
    bool moveRightFlag;
    bool isDead;


    Character(float initialX = charInitialX, float initialY = charInitialY)
        : x(initialX), y(initialY), speed(initialSpeed), jumpVelocity(initialVelocity), size(charSize), isJumping(false), currentJumpVelocity(0.0f),
          isDead(false), moveLeftFlag(false), moveRightFlag(false) {}

    //Setters
    void setX(float x) { this->x = x; }
    void setY(float y) { this->y = y; }
    void setJumpVelocity (float velocity) { jumpVelocity = velocity; }
    void setCurrentJumpVelocity (float velocity) { currentJumpVelocity = velocity; }
    void setCharacterDead(bool dead) { isDead = dead; }
    void setJumping(bool jump) { isJumping = jump; }
    void setSpeed(float speed) {this->speed = speed;}

    //Getters
    float getJumpVelocity () { return jumpVelocity; }
    float getCurrentJumpVelocity () { return currentJumpVelocity; }
    float getX() { return x; }
    float getY() { return y; }
    bool isCharacterDead() const { return isDead; }
    bool getJumping() { return isJumping; }
    int getRespawnDelay() const { return respawnDelay; }
    float getSize() const { return size; }
    bool getMoveLeftFlag() { return moveLeftFlag; }
    bool getMoveRightFlag() { return moveRightFlag; }
    float getSpeed() { return speed; }
    float getWidth() { return size; }
    float getHeight() { return  size; }


    void draw() {
        glColor3f(0.5f, 0.5f, 0.5f); // Set character color to gray

        glBegin(GL_POLYGON);
        glColor3f(0.212, 0.102, 0.047);  // brown
        glVertex2f(x+5.9f, y+4.5f);
        glVertex2f(x+1.1f, y+4.5f);
        glVertex2f(x+1.1f, y+0.7f);
        glVertex2f(x+19.8f, y+0.7f);
        glVertex2f(x+19.9f, y+7.7f);
        glVertex2f(x+5.9f, y+7.7f);
        glVertex2f(x+5.9f, y+4.5f);
        glEnd();

        glBegin(GL_POLYGON);
        glColor3f(0.212, 0.102, 0.047);  // brown
        glVertex2f(x+53.9f, y+4.5f);
        glVertex2f(x+59.0f, y+4.5f);
        glVertex2f(x+59.0f, y+0.8f);
        glVertex2f(x+40.1f, y+0.7f);
        glVertex2f(x+40.3f, y+7.7f);
        glVertex2f(x+53.9f, y+7.7f);
        glVertex2f(x+53.9f, y+4.5f);
        glEnd();

        glBegin(GL_POLYGON);
        glColor3f(0.58, 0.055, 0.051);  // red
        glVertex2f(x+10.9f, y+7.7f);
        glVertex2f(x+10.9f, y+11.3f);
        glVertex2f(x+24.8f, y+11.3f);
        glVertex2f(x+24.8f, y+7.7f);
        glEnd();

        glBegin(GL_POLYGON);
        glColor3f(0.58, 0.055, 0.051);  // red
        glVertex2f(x+35.4f, y+7.7f);
        glVertex2f(x+35.4f, y+11.3f);
        glVertex2f(x+49.8f, y+11.3f);
        glVertex2f(x+49.8f, y+7.7f);
        glEnd();

        glBegin(GL_POLYGON);
        glColor3f(0.58, 0.055, 0.051);  // red
        glVertex2f(x+10.9f, y+11.3f);
        glVertex2f(x+10.9f, y+14.8f);
        glVertex2f(x+49.8f, y+14.8f);
        glVertex2f(x+49.8f, y+11.3f);
        glEnd();

        glBegin(GL_POLYGON);
        glColor3f(0.784, 0.655, 0.533);  // skin
        glVertex2f(x+49.8f, y+11.3f);
        glVertex2f(x+49.8f, y+22.0f);
        glVertex2f(x+59.0f, y+22.0f);
        glVertex2f(x+59.0f, y+11.3f);
        glEnd();

        glBegin(GL_POLYGON);
        glColor3f(0.784, 0.655, 0.533);  // skin
        glVertex2f(x+0.7f, y+11.3f);
        glVertex2f(x+10.9f, y+11.3f);
        glVertex2f(x+10.9f, y+22.0f);
        glVertex2f(x+0.7f, y+22.0f);
        glEnd();

        glBegin(GL_POLYGON);
        glColor3f(0.58, 0.055, 0.051);  // red
        glVertex2f(x+10.9f, y+14.8f);
        glVertex2f(x+10.9f, y+18.5f);
        glVertex2f(x+49.8f, y+18.5f);
        glVertex2f(x+49.8f, y+14.8f);
        glEnd();

        glBegin(GL_POLYGON);
        glColor3f(0.784, 0.655, 0.533);  // skin
        glVertex2f(x+10.9f, y+14.8f);
        glVertex2f(x+10.9f, y+18.5f);
        glVertex2f(x+15.3f, y+18.5f);
        glVertex2f(x+15.3f, y+14.8f);
        glEnd();

        glBegin(GL_POLYGON);
        glColor3f(0.655, 0.576, 0.055);  // yellow
        glVertex2f(x+20.0f, y+14.8f);
        glVertex2f(x+20.0f, y+18.5f);
        glVertex2f(x+25.0f, y+18.5f);
        glVertex2f(x+25.0f, y+14.8f);
        glEnd();

        glBegin(GL_POLYGON);
        glColor3f(0.655, 0.576, 0.055);  // yellow
        glVertex2f(x+35.0f, y+14.8f);
        glVertex2f(x+35.0f, y+18.5f);
        glVertex2f(x+40.0f, y+18.5f);
        glVertex2f(x+40.0f, y+14.8f);
        glEnd();

        glBegin(GL_POLYGON);
        glColor3f(0.784, 0.655, 0.533);  // skin
        glVertex2f(x+45.0f, y+14.8f);
        glVertex2f(x+45.0f, y+18.5f);
        glVertex2f(x+49.8f, y+18.5f);
        glVertex2f(x+49.8f, y+14.8f);
        glEnd();

        glBegin(GL_POLYGON);
        glColor3f(0.047, 0.043, 0.549);  // blue
        glVertex2f(x+10.8f, y+18.5f);
        glVertex2f(x+10.8f, y+33.3f);
        glVertex2f(x+49.8f, y+33.f);
        glVertex2f(x+49.8f, y+18.5f);
        glEnd();

        glBegin(GL_POLYGON);
        glColor3f(0.58, 0.055, 0.051);  // red
        glVertex2f(x+20.0f, y+18.5f);
        glVertex2f(x+20.0f, y+33.3f);
        glVertex2f(x+25.0f, y+33.3f);
        glVertex2f(x+25.0f, y+18.5f);
        glEnd();

        glBegin(GL_POLYGON);
        glColor3f(0.58, 0.055, 0.051);  // red
        glVertex2f(x+25.0f, y+18.5f);
        glVertex2f(x+25.0f, y+22.0f);
        glVertex2f(x+40.0f, y+22.0f);
        glVertex2f(x+40.0f, y+18.5f);
        glEnd();

        glBegin(GL_POLYGON);
        glColor3f(0.58, 0.055, 0.051);  // red
        glVertex2f(x+35.0f, y+22.0f);
        glVertex2f(x+35.0f, y+33.3f);
        glVertex2f(x+40.0f, y+33.3f);
        glVertex2f(x+40.0f, y+22.0f);
        glEnd();

        glBegin(GL_POLYGON);
        glColor3f(0.047, 0.043, 0.549);  // blue
        glVertex2f(x+5.7f, y+25.7f);
        glVertex2f(x+5.7f, y+29.5f);
        glVertex2f(x+10.9f, y+29.5f);
        glVertex2f(x+10.9f, y+22.0f);
        glVertex2f(x+0.7f, y+22.0f);
        glVertex2f(x+0.7f, y+25.7f);
        glVertex2f(x+5.7f, y+25.7f);
        glEnd();

        glBegin(GL_POLYGON);
        glColor3f(0.047, 0.043, 0.549);  // blue
        glVertex2f(x+54.7f, y+25.7f);
        glVertex2f(x+54.7f, y+29.5f);
        glVertex2f(x+50.0f, y+29.5f);
        glVertex2f(x+50.0f, y+22.0f);
        glVertex2f(x+59.3f, y+22.0f);
        glVertex2f(x+59.3f, y+25.7f);
        glVertex2f(x+54.7f, y+25.7f);
        glEnd();

        glBegin(GL_POLYGON);
        glColor3f(0.784, 0.655, 0.533);  // skin
        glVertex2f(x+15.7f, y+33.3f);
        glVertex2f(x+15.7f, y+52.5f);
        glVertex2f(x+45.0f, y+52.5f);
        glVertex2f(x+45.0f, y+33.3f);
        glEnd();

        glBegin(GL_POLYGON);
        glColor3f(0.58, 0.055, 0.051);  // red
        glVertex2f(x+10.5f, y+52.5f);
        glVertex2f(x+10.5f, y+56.0f);
        glVertex2f(x+15.7f, y+56.0f);
        glVertex2f(x+15.7f, y+52.5f);
        glEnd();

        glBegin(GL_POLYGON);
        glColor3f(0.58, 0.055, 0.051);  // red
        glVertex2f(x+15.7f, y+52.5f);
        glVertex2f(x+15.7f, y+59.0f);
        glVertex2f(x+40.0f, y+59.0f);
        glVertex2f(x+40.0f, y+52.5f);
        glEnd();

        glBegin(GL_POLYGON);
        glColor3f(0.58, 0.055, 0.051);  // red
        glVertex2f(x+40.0f, y+52.5f);
        glVertex2f(x+40.0f, y+56.0f);
        glVertex2f(x+54.0f, y+56.0f);
        glVertex2f(x+54.0f, y+52.5f);
        glEnd();

        glBegin(GL_POLYGON);
        glColor3f(0.212, 0.102, 0.047);  // brown
        glVertex2f(x+10.7f, y+48.5f);
        glVertex2f(x+10.7f, y+52.5f);
        glVertex2f(x+25.0f, y+52.5f);
        glVertex2f(x+25.0f, y+48.5f);
        glEnd();

        glBegin(GL_POLYGON);
        glColor3f(0.212, 0.102, 0.047);  // brown
        glVertex2f(x+20.0f, y+44.5f);
        glVertex2f(x+20.0f, y+48.5f);
        glVertex2f(x+15.7f, y+48.5f);
        glVertex2f(x+15.7f, y+41.0f);
        glVertex2f(x+25.0f, y+41.0f);
        glVertex2f(x+25.0f, y+44.5f);
        glVertex2f(x+20.0f, y+44.5f);
        glEnd();

        glBegin(GL_POLYGON);
        glColor3f(0.784, 0.655, 0.533);  // skin
        glVertex2f(x+10.3f, y+41.0f);
        glVertex2f(x+10.3f, y+48.5f);
        glVertex2f(x+15.7f, y+48.5f);
        glVertex2f(x+15.7f, y+41.0f);
        glEnd();

        glBegin(GL_POLYGON);
        glColor3f(0.212, 0.102, 0.047);  // brown
        glVertex2f(x+10.3f, y+41.0f);
        glVertex2f(x+10.3f, y+48.5f);
        glVertex2f(x+6.0f, y+48.5f);
        glVertex2f(x+6.0f, y+37.4f);
        glVertex2f(x+15.7f, y+37.4f);
        glVertex2f(x+15.7f, y+41.0f);
        glVertex2f(x+10.3f, y+41.0f);
        glEnd();

        glBegin(GL_POLYGON);
        glColor3f(0.043, 0.039, 0.024);  // black
        glVertex2f(x+35.4f, y+44.5f);
        glVertex2f(x+35.4f, y+52.5f);
        glVertex2f(x+40.0f, y+52.5f);
        glVertex2f(x+40.0f, y+44.5f);
        glEnd();

        glBegin(GL_POLYGON);
        glColor3f(0.212, 0.102, 0.047);  // brown
        glVertex2f(x+35.4f, y+37.4f);
        glVertex2f(x+35.4f, y+41.0f);
        glVertex2f(x+54.5f, y+41.0f);
        glVertex2f(x+54.5f, y+37.4f);
        glEnd();

        glBegin(GL_POLYGON);
        glColor3f(0.212, 0.102, 0.047);  // brown
        glVertex2f(x+40.0f, y+41.0f);
        glVertex2f(x+40.0f, y+44.5);
        glVertex2f(x+45.0f, y+44.5f);
        glVertex2f(x+45.0f, y+41.0f);
        glEnd();

        glBegin(GL_POLYGON);
        glColor3f(0.784, 0.655, 0.533);  // skin
        glVertex2f(x+45.0f, y+41.0f);
        glVertex2f(x+45.0f, y+48.3f);
        glVertex2f(x+51.0f, y+48.3f);
        glVertex2f(x+51.0f, y+41.0f);
        glEnd();

        glBegin(GL_POLYGON);
        glColor3f(0.784, 0.655, 0.533);  // skin
        glVertex2f(x+51.0f, y+41.0f);
        glVertex2f(x+51.0f, y+44.3f);
        glVertex2f(x+55.5f, y+44.3f);
        glVertex2f(x+55.5f, y+41.0f);
        glEnd();

    }

    void moveLeft() {
        x -= speed;
        if (x < windowInitialX) {
            x = windowInitialX;
        }
    }

    void moveRight() {
        x += speed;
        if(isStage1){
            if (x > 7300) {
                x = 0.0;
                isStage1= false;
                isStage2= true;
                glutPostRedisplay();
            }
        }
        if(isStage2){
            if (x > 7300) {
                x = 0.0;
                isStage2= false;
                isStage3= true;
                glutPostRedisplay();
            }
        }
        if(isStage3){
            if (x > 7300) {
                x = 0.0;
                isStage3= false;
                winner= true;
                glutPostRedisplay();
            }
        }
    }

    void jump() {
        if (!isJumping) {
            isJumping = true;
            currentJumpVelocity = jumpVelocity;
        }
    }

    // Function to handle arrow key events
    void specialKeyPressed(int key, int x, int y) {
        switch (key) {
            case GLUT_KEY_LEFT:
                moveLeftFlag = true;
                moveRightFlag = false;
                break;
            case GLUT_KEY_RIGHT:
                moveRightFlag = true;
                moveLeftFlag = false;
                break;
            case GLUT_KEY_UP:
                if (!isJumping) {
                    isJumping = true;
                    currentJumpVelocity = jumpVelocity;
                }
                break;
            default:
                break;
        }
    }

    // Function to handle arrow key release events
    void specialKeyReleased(int key, int x, int y) {
        switch (key) {
            case GLUT_KEY_LEFT:
                moveLeftFlag = false;
                break;
            case GLUT_KEY_RIGHT:
                moveRightFlag = false;
                break;
            default:
                break;
        }
    }

};

//.........................END Character........................


//............................Coin Start.........................
class Coin {
private:
    float x;      // X-coordinate of the coin's center
    float y;      // Y-coordinate of the coin's center
    float radius; // Radius of the coin (considering it's a circle)
    float red;    // Red component of the coin's color
    float green;  // Green component of the coin's color
    float blue;   // Blue component of the coin's color

public:
    // Constructor to initialize the coin
    Coin(float x, float y, float radius, float red, float green, float blue)
    : x(x), y(y), radius(radius), red(red), green(green), blue(blue) {}

    bool isActive = true;
    bool isCollected = false;
    bool collectedCoinFlag;
    bool isCollidingWithCoin = false; // Add a flag to track if character is colliding with a coin

    // Getters for the coin properties
    float getX() const { return x; }
    float getY() const { return y; }
    float getRadius() const { return radius; }
    float getRed() const { return red; }
    float getGreen() const { return green; }
    float getBlue() const { return blue; }

    // Setters for the coin properties
    void setX(float newX) { x = newX; }
    void setY(float newY) { y = newY; }
    void setRadius(float newRadius) { radius = newRadius; }
    void setRed(float newRed) { red = newRed; }
    void setGreen(float newGreen) { green = newGreen; }
    void setBlue(float newBlue) { blue = newBlue; }


    // Function to draw the coin using GLUT functions
    void draw() const{

        if(isActive && !isCollected) {

            //outer circle
            GLfloat a= x;
            GLfloat b= y;
            GLfloat radius = 20.0f;
            int triangleAmount = 100; //# of lines used to draw circle

            GLfloat twicePi = 2.0f * PI;

            glColor3ub(255,215,0);
            glBegin(GL_TRIANGLE_FAN);
            glVertex2f(a, b); // center of circle
            for(int i = 0; i <= triangleAmount; i++)
            {
                glVertex2f( a + (radius * cos(i *  twicePi / triangleAmount)),
                            b + (radius * sin(i * twicePi / triangleAmount)) );
            }
            glEnd();

            //inner black line
            glLineWidth(2.0f);
            GLfloat x1=x;
            GLfloat y1 =y;
            GLfloat radius1 =(radius/2);
            int lineAmount = 100; //# of triangles used to draw circle
            GLfloat twicePi1 = 2.0f * PI;

            glColor3f(0.0f, 0.0f, 0.0f);
            glBegin(GL_LINE_LOOP);
            for(int i = 0; i <= lineAmount; i++)
            {
                glVertex2f(
                    x1 + (radius1 * cos(i *  twicePi1 / lineAmount)),
                    y1 + (radius1* sin(i * twicePi1 / lineAmount))
                );
            }
            glEnd();

            //inner circle
            glLineWidth(2.0f);
            GLfloat x2=x;
            GLfloat y2=y;
            GLfloat radius2 = radius;
            int lineAmount2 = 100; //# of triangles used to draw circle
            GLfloat twicePi2 = 2.0f * PI;

            glColor3f(0.0f, 0.0f, 0.0f);
            glBegin(GL_LINE_LOOP);
            for(int i = 0; i <= lineAmount2; i++)
            {
                glVertex2f(
                    x2 + (radius2 * cos(i *  twicePi2 / lineAmount2)),
                    y2 + (radius* sin(i * twicePi2 / lineAmount2))
                );
            }
            glEnd();

            glPopMatrix();
        }
    }

    // Function to check collision with the coin
        bool checkCollision(Character character) const {
            float charLeft = character.getX();
                float charRight = character.getX() + character.getWidth();
                float charTop = character.getY() + character.getHeight();
                float charBottom = character.getY();

                float coinLeft = getX() - getRadius();
                float coinRight = getX() + getRadius();
                float coinTop = getY() + getRadius();
                float coinBottom = getY() - getRadius();

                // Check if the bounding boxes intersect
                if (charRight < coinLeft || charLeft > coinRight || charTop < coinBottom || charBottom > coinTop) {
                    // Bounding boxes do not intersect, so no collision
                    return false;
                } else {
                    // Bounding boxes intersect, so collision
                    return true;
                }
            }

    // Function to collect the coin and update the score
    void collectCoin() {
        if (isActive) {
            isActive = false; // Set isActive to false for the collected coin
            score++;
            collectedCoinFlag = true;
            isCollidingWithCoin = true; // Set the flag to true to prevent further collisions
            isCollected = true; // Set the flag to indicate the coin is collected
        }
    }

    // Function to check collision between character and coins
    void checkCoinCollision(Character character) {
        // Reset the flag before checking collisions with coins
            isCollidingWithCoin = false;
            if (checkCollision(character)) {
                collectCoin();
            }
    }

};

//.....................................Coin End.....................................


//...................................Obstacle Start.................................

class Obstacle {
private:
    float obsX;
    float obsY;
    float obsWidth;
    float obsHeight;
    float colorR; // Red component of color (0.0 to 1.0)
    float colorG; // Green component of color (0.0 to 1.0)
    float colorB; // Blue component of color (0.0 to 1.0)


public:
    Obstacle(float xPos, float yPos, float width, float height, float red, float green, float blue)
        : obsX(xPos), obsY(yPos), obsWidth(width), obsHeight(height), colorR(red), colorG(green), colorB(blue) {}

    void draw() const {
        glColor3f(colorR, colorG, colorB);

        glBegin(GL_QUADS);
        glVertex2f(obsX, obsY);
        glVertex2f(obsX + obsWidth, obsY);
        glVertex2f(obsX + obsWidth, obsY + obsHeight);
        glVertex2f(obsX, obsY + obsHeight);
        glEnd();
    }

    // Add methods to set and get color components
        void setColor(float red, float green, float blue) {
            colorR = red;
            colorG = green;
            colorB = blue;
        }

        bool checkCollision(Character character) const {
        float charLeft = character.getX();
        float charRight = character.getX() + character.getWidth();
        float charTop = character.getY() + character.getHeight();
        float charBottom = character.getY();

        float obstacleLeft = getX();
        float obstacleRight = getX() + getWidth();
        float obstacleTop = getY() + getHeight();
        float obstacleBottom = getY();

        // Check if the character collides with the obstacle
        if (charRight < obstacleLeft || charLeft > obstacleRight || charTop < obstacleBottom || charBottom > obstacleTop) {
            // Bounding boxes do not intersect, so no collision
            return false;
        } else {
            // Bounding boxes intersect, so collision
            return true;
        }

    }

    float getX() const { return obsX; }
    float getY() const { return obsY; }
    float getWidth() const { return obsWidth; }
    float getHeight() const { return obsHeight; }
};

//.................................Obstacle End........................................

//..................................River Start.........................................

class River {
private:
    float rivX;
    float rivY;
    float rivWidth;
    float rivHeight;
    float colorR; // Red component of color (0.0 to 1.0)
    float colorG; // Green component of color (0.0 to 1.0)
    float colorB; // Blue component of color (0.0 to 1.0)

public:
    River(float xPos, float yPos, float width, float height, float red, float green, float blue)
        : rivX(xPos), rivY(yPos), rivWidth(width), rivHeight(height), colorR(red), colorG(green), colorB(blue) {}

    void draw() const {
        glColor3f(colorR, colorG, colorB);

        glBegin(GL_QUADS);
        glVertex2f(rivX, rivY);
        glVertex2f(rivX + rivWidth, rivY);
        glVertex2f(rivX + rivWidth, rivY + rivHeight);
        glVertex2f(rivX, rivY + rivHeight);
        glEnd();
    }

    // Add methods to set and get color components
        void setColor(float red, float green, float blue) {
            colorR = red;
            colorG = green;
            colorB = blue;
        }

    // Function to check if there is a collision between the character and the river
    bool checkRiverCollision(Character character) const{
        // Check if the character's position is within the river area
        if (character.getY() <= (rivY + rivHeight) && character.getX()  >= rivX && character.getX() <= (rivX + rivWidth-character.getSize())){
            return true;
        }
        else {

            return false; // No collision
        }
    }

};

//............................................River End....................................

//.......................................Stage Component Start........................

void drawHill(float x, float hillHeight, float hillWidth) {
    glPushMatrix();
    glTranslatef(x, 101.0f, 0.0f);

    int numPoints = 100;
    float step = 2.0f * hillWidth / numPoints;

    // Draw the black border around the hill
    glBegin(GL_TRIANGLE_STRIP);
    for (int i = 0; i <= numPoints; ++i) {
        float t = (float) (i / numPoints);
        float angle = t * 3.14159f;
        float yOffset = hillHeight * sin(angle);

        // Use black color for the border
        glColor3f(0.0f, 0.0f, 0.0f);

        // Outer border vertices
        glVertex2f(i * step, -yOffset - 3.0f);
        glVertex2f(i * step, yOffset + 3.0f);
    }
    glEnd();

    // Draw the inner hill
    glBegin(GL_TRIANGLE_STRIP);
    for (int i = 0; i <= numPoints; ++i) {
        float t = (float) (i / numPoints);
        float angle = t * 3.14159f;
        float yOffset = hillHeight * sin(angle);

        // Use green color for the hill
        glColor3f(0.5f, 0.7f, 0.3f);

        // Inner hill vertices (adjusted for the border)
        glVertex2f(i * step, -yOffset);
        glVertex2f(i * step, yOffset);
    }
    glEnd();

    glPopMatrix();
}


void drawCloud(float x, float y) {
    int numSegments = 20; // Number of segments for the cloud circle
    float cloudRadius = 30.0f; // Radius of the cloud circle

    glColor3ub(255, 255, 255); // White color for clouds

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y); // Center of the cloud
    for (int j = 0; j <= numSegments; ++j) {
        float angle = j * (360.0f / numSegments);
        float cloudX = x + cloudRadius * cos(angle * PI / 180.0);
        float cloudY = y + cloudRadius * sin(angle * PI / 180.0);
        glVertex2f(cloudX, cloudY);
    }
    glEnd();

    // Draw more circles to create the cloud shape
    // Add more circles and adjust their positions and radii to customize the cloud shape
    numSegments = 20;
    cloudRadius = 50.0f;

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x + 60, y); // Center of the second circle
    for (int j = 0; j <= numSegments; ++j) {
        float angle = j * (360.0f / numSegments);
        float cloudX = x + 60 + cloudRadius * cos(angle * PI / 180.0);
        float cloudY = y + cloudRadius * sin(angle * PI / 180.0);
        glVertex2f(cloudX, cloudY);
    }
    glEnd();

    numSegments = 20;
    cloudRadius = 40.0f;

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x + 120, y); // Center of the third circle
    for (int j = 0; j <= numSegments; ++j) {
        float angle = j * (360.0f / numSegments);
        float cloudX = x + 120 + cloudRadius * cos(angle * PI / 180.0);
        float cloudY = y + cloudRadius * sin(angle * PI / 180.0);
        glVertex2f(cloudX, cloudY);
    }
    glEnd();
}


// Function to draw the ground (bricks and lines)
void drawGround(float maxXPosition) {
    glPushMatrix(); // Save the current matrix state
    glTranslatef(0.0f, 0.0f, 0.0f); // Adjust the translation if needed

    // Draw the bricks (assuming each brick has a width of 50 units)
    glColor3f(0.545f, 0.271f, 0.075f); // Brown color for bricks
    for (float x = 0.0f; x <= maxXPosition; x += 50.0f) {
        glBegin(GL_QUADS);
        glVertex2f(x, 0.0f);
        glVertex2f(x + 50.0f, 0.0f);
        glVertex2f(x + 50.0f, 100.0f);
        glVertex2f(x, 100.0f);
        glEnd();
    }

    // Draw the lines between bricks along x-axis
    glLineWidth(3.0f); // Increase the line width to make it bolder
    glColor3f(0.0f, 0.0f, 0.0f); // Black color for lines
    glBegin(GL_LINES);
    for (float x = 0.0f; x <= maxXPosition; x += 50.0f) {
        glVertex2f(x, 0.0f);
        glVertex2f(x, 100.0f);
    }
    glEnd();

    // Draw the lines between bricks along y-axis
    glBegin(GL_LINES);
    for (float y = 0.0f; y <= 100.0f; y += 50.0f) {
        glVertex2f(0.0f, y);
        glVertex2f(maxXPosition, y);
    }
    glEnd();

    glPopMatrix(); // Restore the previous matrix state
}

void drawGround2(float maxXPosition) {
    glPushMatrix(); // Save the current matrix state
    glTranslatef(0.0f, 0.0f, 0.0f); // Adjust the translation if needed

    // Draw the bricks (assuming each brick has a width of 50 units)
    glColor3f(0.115,0.16,0.20); // Brown color for bricks
    for (float x = 0.0f; x <= maxXPosition; x += 50.0f) {
        glBegin(GL_QUADS);
        glVertex2f(x, 0.0f);
        glVertex2f(x + 50.0f, 0.0f);
        glVertex2f(x + 50.0f, 100.0f);
        glVertex2f(x, 100.0f);
        glEnd();
    }

    // Draw the lines between bricks along x-axis
    glLineWidth(3.0f); // Increase the line width to make it bolder
    glColor3f(0.0f, 0.0f, 0.0f); // Black color for lines
    glBegin(GL_LINES);
    for (float x = 0.0f; x <= maxXPosition; x += 50.0f) {
        glVertex2f(x, 0.0f);
        glVertex2f(x, 100.0f);
    }
    glEnd();

    // Draw the lines between bricks along y-axis
    glBegin(GL_LINES);
    for (float y = 0.0f; y <= 100.0f; y += 50.0f) {
        glVertex2f(0.0f, y);
        glVertex2f(maxXPosition, y);
    }
    glEnd();

    glPopMatrix(); // Restore the previous matrix state
}

void drawBorder(float x1, float y1, float x2, float y2, float borderWidth) {

    // Draw the black border around the obstacle
    glLineWidth(borderWidth); // Set the desired line width for the border
    glColor3f(0.0f, 0.0f, 0.0f); // Black color for the border
    glBegin(GL_LINE_LOOP);
    glVertex2f(x1, y1);
    glVertex2f(x2, y1);
    glVertex2f(x2, y2);
    glVertex2f(x1, y2);
    glEnd();
}

void drawBrickLines(float minX, float minY, float maxX, float maxY, float brickWidth, float brickHeight, float lineWidth) {
    glPushMatrix(); // Save the current matrix state
    glTranslatef(0.0f, 0.0f, 0.0f); // Adjust the translation if needed

    // Draw the lines between bricks along x-axis
    glLineWidth(lineWidth); // Set the desired line width
    glColor3f(0.0f, 0.0f, 0.0f); // Black color for lines
    glBegin(GL_LINES);
    for (float x = minX; x <= maxX; x += brickWidth) {
        glVertex2f(x, minY);
        glVertex2f(x, maxY);
    }
    glEnd();

    // Draw the lines between bricks along y-axis
    glBegin(GL_LINES);
    for (float y = minY; y <= maxY; y += brickHeight) {
        glVertex2f(minX, y);
        glVertex2f(maxX, y);
    }
    glEnd();

    glPopMatrix(); // Restore the previous matrix state
}

void drawArchGate(float x, float y, float width, float height, float r, float g, float b) {
    // Set the color for the gate
    glColor3f(r, g, b);

    // Draw the gate using rectangles and a filled arch
    glBegin(GL_QUADS);
    // Draw the main gate structure
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();

    // Draw the filled arch above the gate
    float centerX = x + width / 2.0f;
    float centerY = y + height;
    float radius = width / 2.0f;

    int numSegments = 100;
    float angleStep = 3.14159f / numSegments;

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(centerX, centerY); // Center vertex of the fan

    for (int i = 0; i <= numSegments; ++i) {
        float angle = i * angleStep;
        float x1 = centerX + radius * cos(angle);
        float y1 = centerY + radius * sin(angle);
        glVertex2f(x1, y1);
    }

    glEnd();
}

//..........................................Stage Component End......................................

// ......................................Creating Necessary Objects................................

    Character character;

    vector<Obstacle> obstacles;
    vector<River> rivers;


    // Stage 01 Coins
    Coin coin1(1050.0f, 437.0f, 20.0f, 1.0f, 0.843f, 0.0f); // Yellow coin
    Coin coin2(1385, 218, 20.0f, 1.0f, 0.843f, 0.0f); // Yellow coin
    Coin coin3(2252, 320, 20.0f, 1.0f, 0.843f, 0.0f); // Yellow coin
    Coin coin4(2252, 380, 20.0f, 1.0f, 0.843f, 0.0f); // Yellow coin
    Coin coin5(4014, 520, 20.0f, 1.0f, 0.843f, 0.0f); // Yellow coin
    Coin coin6(4526, 336, 20.0f, 1.0f, 0.843f, 0.0f); // Yellow coin
    Coin coin7(4525, 560, 20.0f, 1.0f, 0.843f, 0.0f); // Yellow coin
    Coin coin8(5102, 267, 20.0f, 1.0f, 0.843f, 0.0f); // Yellow coin
    Coin coin9(5246, 267, 20.0f, 1.0f, 0.843f, 0.0f); // Yellow coin
    Coin coin10(5390, 267, 20.0f, 1.0f, 0.843f, 0.0f); // Yellow coin
    Coin coin11(5246, 462, 20.0f, 1.0f, 0.843f, 0.0f); // Yellow coin


    // Stage 02 Coins
    Coin coin41(540 , 230, 20.0f, 1.0f, 0.843f, 0.0f);
    Coin coin42(830 , 330, 20.0f, 1.0f, 0.843f, 0.0f);
    Coin coin43(950 , 530, 20.0f, 1.0f, 0.843f, 0.0f);
    Coin coin44(1240 , 190, 20.0f, 1.0f, 0.843f, 0.0f);
    Coin coin45(1430 , 390, 20.0f, 1.0f, 0.843f, 0.0f);
    Coin coin46(1835 , 540, 20.0f, 1.0f, 0.843f, 0.0f);
    Coin coin47(2000 , 540, 20.0f, 1.0f, 0.843f, 0.0f);
    Coin coin48(2445 , 190, 20.0f, 1.0f, 0.843f, 0.0f);
    Coin coin49(2885 , 190, 20.0f, 1.0f, 0.843f, 0.0f);
    Coin coin50(3255 , 335, 20.0f, 1.0f, 0.843f, 0.0f);
    Coin coin51(3670 , 490, 20.0f, 1.0f, 0.843f, 0.0f);
    Coin coin52(3850 , 490, 20.0f, 1.0f, 0.843f, 0.0f);
    Coin coin53(4290 , 440, 20.0f, 1.0f, 0.843f, 0.0f);
    Coin coin54(4865 , 290, 20.0f, 1.0f, 0.843f, 0.0f);
    Coin coin55(5280 , 490, 20.0f, 1.0f, 0.843f, 0.0f);
    Coin coin56(5410 , 490, 20.0f, 1.0f, 0.843f, 0.0f);
    Coin coin57(5555 , 490, 20.0f, 1.0f, 0.843f, 0.0f);
    Coin coin58(5890 , 190, 20.0f, 1.0f, 0.843f, 0.0f);
    Coin coin59(6270 , 330, 20.0f, 1.0f, 0.843f, 0.0f);


    // Stage 03 Coins
    Coin coin14(1230, 335, 20.0f, 1.0f, 0.843f, 0.0f); // Yellow coin
    Coin coin15(1290, 460, 20.0f, 1.0f, 0.843f, 0.0f); // Yellow coin
    Coin coin16(1410, 460, 20.0f, 1.0f, 0.843f, 0.0f); // Yellow coin
    Coin coin17(1470, 335, 20.0f, 1.0f, 0.843f, 0.0f); // Yellow coin
    Coin coin18(1870, 375, 20.0f, 1.0f, 0.843f, 0.0f); // Yellow coin
    Coin coin19(1950, 375, 20.0f, 1.0f, 0.843f, 0.0f); // Yellow coin
    Coin coin20(2020, 375, 20.0f, 1.0f, 0.843f, 0.0f); // Yellow coin
    Coin coin21(2180, 375, 20.0f, 1.0f, 0.843f, 0.0f); // Yellow coin
    Coin coin22(2270 , 375, 20.0f, 1.0f, 0.843f, 0.0f); // Yellow coin
    Coin coin23(2690 , 355, 20.0f, 1.0f, 0.843f, 0.0f); // Yellow coin
    Coin coin24(2900 , 435, 20.0f, 1.0f, 0.843f, 0.0f); // Yellow coin
    Coin coin25(2965 , 435, 20.0f, 1.0f, 0.843f, 0.0f); // Yellow coin
    Coin coin26(3030 , 435, 20.0f, 1.0f, 0.843f, 0.0f); // Yellow coin
    Coin coin27(3260 , 300, 20.0f, 1.0f, 0.843f, 0.0f); // Yellow coin
    Coin coin28(3480 , 300, 20.0f, 1.0f, 0.843f, 0.0f); // Yellow coin
    Coin coin29(4270 , 300, 20.0f, 1.0f, 0.843f, 0.0f); // Yellow coin
    Coin coin30(4410 , 435, 20.0f, 1.0f, 0.843f, 0.0f); // Yellow coin
    Coin coin31(4485 , 435, 20.0f, 1.0f, 0.843f, 0.0f); // Yellow coin
    Coin coin32(4560 , 435, 20.0f, 1.0f, 0.843f, 0.0f); // Yellow coin
    Coin coin33(4690 , 300, 20.0f, 1.0f, 0.843f, 0.0f); // Yellow coin
    Coin coin34(4990 , 340, 20.0f, 1.0f, 0.843f, 0.0f); // Yellow coin
    Coin coin35(5055 , 340, 20.0f, 1.0f, 0.843f, 0.0f); // Yellow coin
    Coin coin36(5300 , 340, 20.0f, 1.0f, 0.843f, 0.0f); // Yellow coin
    Coin coin37(5365 , 340, 20.0f, 1.0f, 0.843f, 0.0f); // Yellow coin
    Coin coin38(5830 , 390, 20.0f, 1.0f, 0.843f, 0.0f); // Yellow coin
    Coin coin39(5905 , 390, 20.0f, 1.0f, 0.843f, 0.0f); // Yellow coin
    Coin coin40(5980 , 390, 20.0f, 1.0f, 0.843f, 0.0f); // Yellow coin

    //..................................Object Creation End............................

    //....................... Gathering every piece together to complete the game................

    void cameraMovement(){

    // Calculate the camera's position to center the stage around the character
        float cameraX = -(character.getX() - windowFinalX / 2);
        float cameraY = 0.0f; // Keep a fixed Y position for the camera
        float cameraZ = 0.0f; // Set a suitable Z position for the camera (you can adjust this based on your scene)

        // Apply the translation transformation
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(cameraX, cameraY, cameraZ);
}


// Function to check collision with all the obstacles and the river
bool checkAllObstacleCollisions(Character character) {
    for (const auto& obstacle : obstacles) {
        if (obstacle.checkCollision(character)) {
            return true;
        }
    }
    return false; // No collision
}


// Function to check collision with all the river
bool checkAllRiverCollisions(Character character) {
    for (const auto& river : rivers){
        if (river.checkRiverCollision(character)){
            return true;
        }
    }
    return false; // No Collision
}


// Function to respawn the character after a delay
void respawnCharacter(int value) {
    lives--;
    deadPosition = character.getX();
    character.setCharacterDead(false);
    // Set the character's position to the starting position
    character.setX(charInitialX);
    character.setY(charInitialY);
    // Reset jump state and velocity
    character.setJumping(false);
    character.setCurrentJumpVelocity(0.0f);
    glutPostRedisplay(); // Mark the window for a redraw
}


//Function to show Scoreboard
void drawScoreboard(int characterX) {
    int scoreboardX = characterX - windowFinalX / 2;
    int scoreboardY = 10; // Adjust this as needed

    // Draw the scoreboard at the calculated position
    drawText("Score: " + to_string(score), scoreboardX, scoreboardY, GLUT_BITMAP_TIMES_ROMAN_24, 1, 1, 1);
}


//Function to show Lives
void drawLives(int characterX)
{
    int livesX = characterX - windowFinalX / 2;
    int livesY = 40; // Adjust this as needed


    drawText("Lives: " + to_string(lives), livesX, livesY, GLUT_BITMAP_TIMES_ROMAN_24, 1, 1, 1);
}


//Update function
void update(int value) {
    // Store the current character position for collision checking
    float prevCharacterX = character.getX();
    float prevCharacterY = character.getY();


    if(isStage1){   // Stage 01 Coins
        coin1.checkCoinCollision(character);
        coin2.checkCoinCollision(character);
        coin3.checkCoinCollision(character);
        coin4.checkCoinCollision(character);
        coin5.checkCoinCollision(character);
        coin6.checkCoinCollision(character);
        coin7.checkCoinCollision(character);
        coin8.checkCoinCollision(character);
        coin9.checkCoinCollision(character);
        coin10.checkCoinCollision(character);
        coin11.checkCoinCollision(character);

    }

    if(isStage2){   // Stage 02 Coins
        coin41.checkCoinCollision(character);
        coin42.checkCoinCollision(character);
        coin43.checkCoinCollision(character);
        coin44.checkCoinCollision(character);
        coin45.checkCoinCollision(character);
        coin46.checkCoinCollision(character);
        coin47.checkCoinCollision(character);
        coin48.checkCoinCollision(character);
        coin49.checkCoinCollision(character);
        coin50.checkCoinCollision(character);
        coin51.checkCoinCollision(character);
        coin52.checkCoinCollision(character);
        coin53.checkCoinCollision(character);
        coin54.checkCoinCollision(character);
        coin55.checkCoinCollision(character);
        coin56.checkCoinCollision(character);
        coin57.checkCoinCollision(character);
        coin58.checkCoinCollision(character);
        coin59.checkCoinCollision(character);

    }

    if(isStage3){   // Stage 03 Coins
        coin14.checkCoinCollision(character);
        coin15.checkCoinCollision(character);
        coin16.checkCoinCollision(character);
        coin17.checkCoinCollision(character);
        coin18.checkCoinCollision(character);
        coin19.checkCoinCollision(character);
        coin20.checkCoinCollision(character);
        coin21.checkCoinCollision(character);
        coin22.checkCoinCollision(character);
        coin23.checkCoinCollision(character);
        coin24.checkCoinCollision(character);
        coin25.checkCoinCollision(character);
        coin26.checkCoinCollision(character);
        coin27.checkCoinCollision(character);
        coin28.checkCoinCollision(character);
        coin29.checkCoinCollision(character);
        coin30.checkCoinCollision(character);
        coin31.checkCoinCollision(character);
        coin32.checkCoinCollision(character);
        coin33.checkCoinCollision(character);
        coin34.checkCoinCollision(character);
        coin35.checkCoinCollision(character);
        coin36.checkCoinCollision(character);
        coin37.checkCoinCollision(character);
        coin38.checkCoinCollision(character);
        coin39.checkCoinCollision(character);
        coin40.checkCoinCollision(character);
    }

    // Move the character based on the current direction
    if (character.getMoveLeftFlag()) {
        character.moveLeft();
    }
    if (character.getMoveRightFlag()) {
        character.moveRight();
    }

    // Check for collision with obstacles and handle accordingly
    bool isObstacleCollision = checkAllObstacleCollisions(character);
    if (isObstacleCollision) {
        // Restore the previous character position
        character.setX(prevCharacterX);
    }

    // Check for collision with the river, and handle accordingly
    bool isRiverColliding = checkAllRiverCollisions(character);
    if (isRiverColliding && !character.isCharacterDead()) {
        character.setY(character.getY() - gravity);
        character.isDead = true;
        glutTimerFunc(10, respawnCharacter, 0);
    }

    // Apply gravity if jumping
    if (character.getJumping()) {
        // Update the vertical position with projectile motion
        character.setY(character.getY() + character.getCurrentJumpVelocity());

        // Check if the character reaches the ground or an obstacle
        if (checkAllObstacleCollisions(character)) {
            // If on top of an obstacle, adjust the vertical position to stay on top
            character.setY(prevCharacterY);
            character.setJumping(false);
            character.setCurrentJumpVelocity(0.0f); // Reset jump velocity
        } else {
            character.setCurrentJumpVelocity(character.getCurrentJumpVelocity() - gravity);

            if (checkAllRiverCollisions(character) && !character.isCharacterDead()){
                character.setY(character.getY()-gravity);
                character.setCharacterDead(true);
                glutTimerFunc(respawnDelay, respawnCharacter, 0);
            }
            else if (character.getY() <= charInitialY){
                character.setY(charInitialY);
                character.setJumping(false);
                character.setCurrentJumpVelocity(0.0f); // Reset jump velocity
            }
        }
    } else {
            // If not jumping, apply continuous gravity to make the character fall
            // Adjust the gravity value to achieve the desired falling speed
            character.setY(character.getY()- gravity * 5);

        // Check for collision with obstacles or River below the character
        bool isOnObstacle = checkAllObstacleCollisions(character);
        bool isOnRiver = checkAllRiverCollisions(character);

        if (isOnObstacle) {
            // If on top of an obstacle, adjust the vertical position to stay on top
            character.setY(prevCharacterY);
        }

        // Check if on top of river
        if (isOnRiver) {
            // Character has died, reset its position to the starting position

            character.setY(character.getY() - 0.5f);
            character.isDead = true;
            glutTimerFunc(respawnDelay, respawnCharacter, 0);
            character.isJumping = false;
            character.currentJumpVelocity = 0.0f;

        }
        // Check if the character reaches the ground level
        if ((!isOnRiver) && character.getY() <= charInitialY) {
            character.setY(charInitialY); // Snap to the ground level
        }
    }

    glutPostRedisplay(); // Mark the window for a redraw
    glutTimerFunc(16, update, 0); // Call moveCharacter function after 16 milliseconds
}


// Function to add a new obstacle
void addObstacle(float xPos, float yPos, float width, float height, float red, float green, float blue) {
    obstacles.emplace_back(xPos, yPos, width, height, red, green, blue);
}


// Function to add a new river
void addRiver(float xPos, float yPos, float width, float height, float red, float green, float blue) {
    rivers.emplace_back(xPos, yPos, width, height, red, green, blue);
}

//Function to draw River
void drawRiver1(){
    addRiver(150.0f, 0.0f, 100.0f, 75.0f, 0.3f, 0.518f, 0.984f);  // River 1
}


//Function to draw obstacles
void drawAllObstacleObjects(){
    // Draw the obstacles
    for (const auto& obstacle : obstacles) {
        obstacle.draw();
    }
}


//Function to draw river objects
void drawAllRiverObjects(){
    // Draw the rivers
    for (const auto& river : rivers) {
        river.draw();
    }
}


//---------------------------- STAGE 01 --------------------------------------------------------


void drawStage01(){

    isStage1 = true;

    drawAllObstacleObjects();  // If you have obstacles call this function

    addRiver(-700, 0, 698, 70, 0.3f, 0.518f, 0.984f);

    drawCloud(450, 540);

    drawHill(0, 200, 230);
    drawHill(760, 90, 200);

    addObstacle(767, 247, 45, 45, 0.69f, 0.263f, 0.043f);
    drawBorder(767, 247, 767+45, 247+45, 3.0f);

    addObstacle(955, 247, 45*4, 45, 0.69f, 0.263f, 0.043f);
    drawBrickLines(955, 247, 1135, 292, 45, 45, 3);

    drawCloud(980, 580);

    addObstacle(1341, 151, 95, 42, 0.59f, 0.863f, 0.039f);
    drawBorder(1341, 151, 1341+95, 151+42, 3);

    addObstacle(1349, 101, 80, 50, 0.59f, 0.863f, 0.039f);
    drawBorder(1349, 101, 1349+80, 101+50, 3);

    drawCloud(1450, 550);
    drawCloud(1820, 590);

    addObstacle(1822, 201, 95, 42, 0.59f, 0.863f, 0.039f);
    drawBorder(1822, 201, 1822+95, 201+42, 3);

    addObstacle(1828, 101, 80, 100, 0.59f, 0.863f, 0.039f);
    drawBorder(1828, 101, 1828+82, 101+100, 3);

    addObstacle(2211, 101, 80, 147, 0.59f, 0.863f, 0.039f);
    drawBorder(2211, 101, 2211+82, 101+147, 3);

    addObstacle(2205, 248, 95, 42, 0.59f, 0.863f, 0.039f);
    drawBorder(2205, 248, 2205+95, 248+42, 3);

    drawHill(2300, 100, 200);

    addObstacle(2733, 248, 95, 42, 0.59f, 0.863f, 0.039f);
    drawBorder(2733, 248, 2733+95, 248+42, 3);

    addObstacle(2738, 101, 80, 147, 0.59f, 0.863f, 0.039f);
    drawBorder(2738, 101, 2738+82, 101+147, 3);

    drawCloud(2755, 540);
    drawCloud(3285, 590);

    drawHill(3060, 50, 70);

    addRiver(3301, 0, 97, 102, 0.55f, 0.53f, 0.94f);

    addObstacle(3690, 247, 45*3, 45, 0.69f, 0.263f, 0.043f);
    drawBorder(3690, 247, 3690+45, 247+45, 3.0f);
    drawBorder(3690+45, 247, 3690+45+45, 247+45, 3.0f);     //*** IF POSSIBLE HIT BRICK TO MAKE INVISIBLE
    drawBorder(3690+45+45, 247, 3690+45+45+45, 247+45, 3.0f);

    drawCloud(3712, 540);

    addObstacle(3834, 440, 45*8, 45, 0.69f, 0.263f, 0.043f);
    drawBrickLines(3834, 440, 4194, 485, 45, 45, 3.0f);

    drawCloud(4120, 595);

    addObstacle(4360, 440, 45*4, 45, 0.69f, 0.263f, 0.043f);
    drawBrickLines(4360, 440, 4360+45*4, 485, 45, 45, 3.0f);

    addObstacle(4503, 247, 45, 45, 0.69f, 0.263f, 0.043f);  //***
    drawBorder(4503, 247, 4503+45, 247+45, 3);

    drawHill(4600, 90, 140);

    addObstacle(4790, 247, 45*2, 45, 0.69f, 0.263f, 0.043f);
    drawBrickLines(4790, 247, 4790+45*2, 247+45, 45, 45, 3.0f);

    drawCloud(5060, 540);
    drawCloud(5584, 586);

    drawHill(5364, 90 , 150);

    addObstacle(5652, 247, 45, 45, 0.69f, 0.263f, 0.043f);
    drawBorder(5652, 247, 5652+45, 247+45, 3);

    addObstacle(5800, 440, 45*3, 45, 0.69f, 0.263f, 0.043f);
    drawBrickLines(5800, 440, 5800+45*3, 440+45, 45, 45, 3.0f);

    drawCloud(6011, 545);

    addObstacle(6135, 440, 45*4, 45, 0.69f, 0.263f, 0.043f);
    drawBrickLines(6135, 440, 6135+45*4, 440+45, 45, 45, 3.0f);

    addObstacle(6181, 247, 45*2, 45, 0.69f, 0.263f, 0.043f);
    drawBrickLines(6181, 247, 6181+45*2, 247+45, 45, 45, 3.0f);

    addObstacle(6421, 101, 45*9, 45, 0.69f, 0.263f, 0.043f);
    drawBrickLines(6421, 101, 6421+45*9, 101+45, 45, 45, 3.0f);

    addObstacle(6421+45, 101+45, 45*8, 45, 0.69f, 0.263f, 0.043f);
    drawBrickLines(6421+45, 101+45, 6421+45*9, 101+45+45, 45, 45, 3.0f);

    addObstacle(6421+45+45, 101+45+45, 45*7, 45, 0.69f, 0.263f, 0.043f);
    drawBrickLines(6421+45+45, 101+45+45, 6421+45*9, 101+45+45+45, 45, 45, 3.0f);

    addObstacle(6421+45+45+45, 101+45+45+45, 45*6, 45, 0.69f, 0.263f, 0.043f);
    drawBrickLines(6421+45+45+45, 101+45+45+45, 6421+45*9, 101+45+45+45+45, 45, 45, 3.0f);

    addObstacle(6421+45+45+45+45, 101+45+45+45+45, 45*5, 45, 0.69f, 0.263f, 0.043f);
    drawBrickLines(6421+45+45+45+45, 101+45+45+45+45, 6421+45*9, 101+45+45+45+45+45, 45, 45, 3.0f);

    addObstacle(6421+45+45+45+45+45, 101+45+45+45+45+45, 45*4, 45, 0.69f, 0.263f, 0.043f);
    drawBrickLines(6421+45+45+45+45+45, 101+45+45+45+45+45, 6421+45*9, 101+45+45+45+45+45+45, 45, 45, 3.0f);

    addObstacle(6421+45+45+45+45+45+45, 101+45+45+45+45+45+45, 45*3, 45, 0.69f, 0.263f, 0.043f);
    drawBrickLines(6421+45+45+45+45+45+45, 101+45+45+45+45+45+45, 6421+45*9, 101+45+45+45+45+45+45+45, 45, 45, 3.0f);

    addObstacle(6421+45+45+45+45+45+45+45, 101+45+45+45+45+45+45+45, 45*2, 45, 0.69f, 0.263f, 0.043f);
    drawBrickLines(6421+45+45+45+45+45+45+45, 101+45+45+45+45+45+45+45, 6421+45*9, 101+45+45+45+45+45+45+45+45, 45, 45, 3.0f);

    addObstacle(6421+45+45+45+45+45+45+45+45, 101+45+45+45+45+45+45+45+45, 45, 45, 0.69f, 0.263f, 0.043f);
    drawBrickLines(6421+45+45+45+45+45+45+45+45, 101+45+45+45+45+45+45+45+45, 6421+45*9, 101+45+45+45+45+45+45+45+45+45, 45, 45, 3.0f);

    drawCloud(6427, 590);

    glBegin(GL_LINE_STRIP);
    glLineWidth(50.0f);
    glColor3f(0.616f, 0.992f, 0.094f);
    glVertex2f(7048.5f, 147.0f);
    glVertex2f(7048.5f, 570.0f);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(0.9f, 0.9f, 0.9f);
    glVertex2f(7048.5f, 570.0f);
    glVertex2f(6998.5f, 570.0f);
    glVertex2f(7048.5f, 520.0f);
    glEnd();

    addObstacle(7026, 101, 45, 45, 0.69f, 0.263f, 0.043f);
    drawBrickLines(7026, 101, 7026+45, 101+45, 45, 45, 3.0f);

    drawArchGate(7220, 101, 150, 100, 0, 0, 0);

    drawGround(8000.0f);

    drawAllRiverObjects();     //MUST BE CALLED AFTER CREATING GROUND If  you have rivers call this function

    coin1.draw();
    coin2.draw();
    coin3.draw();
    coin4.draw();
    coin5.draw();
    coin6.draw();
    coin7.draw();
    coin8.draw();
    coin9.draw();
    coin10.draw();
    coin11.draw();

    character.draw();

    drawScoreboard(character.getX());
    drawLives(character.getX());
}


//---------------------------- STAGE 02 --------------------------------------------------------


void drawStage02()
{
    isStage2 = true;

    drawAllObstacleObjects();  // If you have obstacles call this function

    drawCloud(0,640);
    drawCloud(750, 640);

    // ------------ GROUND --------------
    addObstacle(0, 0, 300 ,100, 0.69f, 0.263f, 0.043f);
    drawBrickLines(0, 0, 300, 100, 50, 50, 3);

    addRiver(300, 0, 200, 100, 0.55f, 0.53f, 0.94f);

    addObstacle(500, 0, 100 , 150, 0.69f, 0.263f, 0.043f); // 1st block
    drawBorder(500, 0, 500+100, 150,3.0f);
    drawBrickLines(500, 0, 500+100, 150, 50, 50, 3);
    addObstacle(450, 150, 200 , 50, 0.f, 1.0f, 0.0f);
    drawBorder(450, 150, 450+200 , 150+50, 3.0f);

    addRiver(600, 0, 200, 100,0.55f, 0.53f, 0.94f);

    addObstacle(800, 0, 300 , 250, 0.69f, 0.263f, 0.043f); //2nd block
    drawBorder(800, 0, 800+300, 250,3.0f);
    drawBrickLines(800, 0, 800+300, 250, 50, 50, 3);
    addObstacle(750, 250, 400 , 50, 0.f, 1.0f, 0.0f);
    drawBorder(750, 250, 750+400 , 250+50, 3.0f);

    addObstacle(900, 300, 150 , 150, 0.69f, 0.263f, 0.043f); //2nd block upper
    drawBorder(900, 300, 900+150, 300+150,3.0f);
    drawBrickLines(900, 300, 900+150, 300+150, 50, 50, 3);
    addObstacle(850, 450, 250 , 50, 0.f, 1.0f, 0.0f);
    drawBorder(850, 450, 850+250 , 450+50, 3.0f);

    addRiver(1100, 0, 100, 100, 0.55f, 0.53f, 0.94f);

    addObstacle(1200, 0, 50 , 100, 0.69f, 0.263f, 0.043f); //3rd block
    drawBorder(1200, 0, 1200+50, 100,3.0f);
    drawBrickLines(1200, 0, 1200+50, 100, 50, 50, 3);
    addObstacle(1150, 100, 150 , 50, 0.f, 1.0f, 0.0f);
    drawBorder(1150, 100, 1150+150 , 100+50, 3.0f);

    addRiver(1250, 0, 100, 100, 0.55f, 0.53f, 0.94f);

    addObstacle(1350, 0, 150 , 300, 0.69f, 0.263f, 0.043f); //4th block
    drawBorder(1350, 0, 1350+150, 300,3.0f);
    drawBrickLines(1350, 0, 1350+150, 300, 50,50, 3);
    addObstacle(1300, 300, 250 , 50, 0.f, 1.0f, 0.0f);
    drawBorder(1300, 300, 1300+250 , 300+50, 3.0f);

    addRiver(1500, 0, 300, 100, 0.55f, 0.53f, 0.94f);

    addObstacle(1800, 0, 300 , 450, 0.69f, 0.263f, 0.043f); //5th block
    drawBorder(1800, 0, 1800+300, 450,3.0f);
    drawBrickLines(1800, 0, 1800+300, 450, 50,50, 3);
    addObstacle(1750, 450, 400 , 50, 0.f, 1.0f, 0.0f);
    drawBorder(1750, 450, 1750+400 , 450+50, 3.0f);

    addRiver(2100, 0, 300, 100,0.55f, 0.53f, 0.94f);

    addObstacle(2400, 0, 150 , 100, 0.69f, 0.263f, 0.043f); //6th block
    drawBorder(2400, 0, 2400+150, 100,3.0f);
    drawBrickLines(2400, 0, 2400+150, 100, 50,50, 3);
    addObstacle(2350, 100, 250 , 50, 0.f, 1.0f, 0.0f);
    drawBorder(2350, 100, 2350+250 , 100+50, 3.0f);

    addRiver(2550, 0, 300, 100, 0.55f, 0.53f, 0.94f);

    drawCloud(2700, 550);

    addObstacle(2850, 0, 150 , 100, 0.69f, 0.263f, 0.043f); //7th block
    drawBorder(2850, 0, 2850+150, 100,3.0f);
    drawBrickLines(2850, 0, 2850+150, 100, 50,50, 3);
    addObstacle(2800, 100, 250 , 50, 0.f, 1.0f, 0.0f);
    drawBorder(2800, 100, 2800+250 , 100+50, 3.0f);

    addRiver(3000, 0, 200, 100,0.55f, 0.53f, 0.94f);

    addObstacle(3200, 0, 80 , 250, 0.69f, 0.263f, 0.043f); //8th block
    drawBorder(3200, 0, 3200+80, 250,3.0f);
    drawBrickLines(3200, 0, 3200+80, 250, 50,50, 3);
    addObstacle(3150, 250, 180 , 50, 0.f, 1.0f, 0.0f);
    drawBorder(3150, 250, 3150+180 , 250+50, 3.0f);

    addRiver(3280, 0, 370, 100, 0.55f, 0.53f, 0.94f);

    addObstacle(3650, 0, 200 , 400, 0.69f, 0.263f, 0.043f); //9th block
    drawBorder(3650, 0, 3650+200, 400,3.0f);
    drawBrickLines(3650, 0, 3650+200, 400, 50, 50, 3);
    addObstacle(3600, 400, 300 , 50, 0.f, 1.0f, 0.0f);
    drawBorder(3600, 400, 3600+300 , 400+50, 3.0f);

    addRiver(3850, 0, 950, 100, 0.55f, 0.53f, 0.94f);

    addObstacle(4200, 350, 200 , 50, 0.69f, 0.263f, 0.043f); //floating  block
    drawBorder(4200, 350, 4200+200, 350+50,3.0f);
    drawBrickLines(4200, 350, 4200+200, 350+50, 50,50, 3);

    drawCloud(4500, 640);

    addObstacle(4800, 0, 150 , 200, 0.69f, 0.263f, 0.043f); //10th block
    drawBorder(4800, 0, 4800+150, 200,3.0f);
    drawBrickLines(4800, 0, 4800+150, 200, 50,50, 3);
    addObstacle(4750, 200, 250 , 50, 0.f, 1.0f, 0.0f);
    drawBorder(4750, 200, 4750+250 , 200+50, 3.0f);

    addRiver(4950, 0, 300, 100,0.55f, 0.53f, 0.94f);

    addObstacle(5250, 0, 350 , 400, 0.69f, 0.263f, 0.043f); //11th block
    drawBorder(5250, 0, 5250+350, 400,3.0f);
    drawBrickLines(5250, 0, 5250+350, 400, 50,50, 3);
    addObstacle(5200, 400, 450 , 50, 0.f, 1.0f, 0.0f);
    drawBorder(5200, 400, 5200+450 , 400+50, 3.0f);

    addRiver(5600, 0, 250, 100,0.55f, 0.53f, 0.94f);

    addObstacle(5850, 0, 100 , 100, 0.69f, 0.263f, 0.043f); //12th block
    drawBorder(5850, 0, 5850+100, 100,3.0f);
    drawBrickLines(5850, 0, 5850+100, 100,50,50, 3);
    addObstacle(5800, 100, 200 , 50, 0.f, 1.0f, 0.0f);
    drawBorder(5800, 100, 5800+200 , 100+50, 3.0f);

    addRiver(5950, 0, 250, 100, 0.55f, 0.53f, 0.94f);

    addObstacle(6200, 0, 150 , 250, 0.69f, 0.263f, 0.043f); //13th block
    drawBorder(6200, 0, 6200+150, 250,3.0f);
    drawBrickLines(6200, 0, 6200+150, 250, 50,50, 3);
    addObstacle(6150, 250, 250 , 50, 0.f, 1.0f, 0.0f);
    drawBorder(6150, 250, 6150+250 , 250+50, 3.0f);

    addRiver(6350, 0, 150, 100,0.55f, 0.53f, 0.94f);

    // ------------ GROUND --------------
    addObstacle(6500, 0, 2000 ,100, 0.69f, 0.263f, 0.043f);
    drawBrickLines(6500, 0, 6450+2000, 100, 50, 50, 3);

    drawCloud(6500, 640);

    glBegin(GL_LINE_STRIP);
    glLineWidth(50.0f);
    glColor3f(0.616f, 0.992f, 0.094f);
    glVertex2f(7048.5f, 147.0f);
    glVertex2f(7048.5f, 570.0f);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(0.9f, 0.9f, 0.9f);
    glVertex2f(7048.5f, 570.0f);
    glVertex2f(6998.5f, 570.0f);
    glVertex2f(7048.5f, 520.0f);
    glEnd();

    addObstacle(7026, 101, 45, 45, 0.69f, 0.263f, 0.043f);
    drawBrickLines(7026, 101, 7026+45, 101+45, 45, 45, 3.0f);

    drawArchGate(7220, 101, 150, 100, 0, 0, 0);

    drawAllRiverObjects();

    coin41.draw();
    coin42.draw();
    coin43.draw();
    coin44.draw();
    coin45.draw();
    coin46.draw();
    coin47.draw();
    coin48.draw();
    coin49.draw();
    coin50.draw();
    coin51.draw();
    coin52.draw();
    coin53.draw();
    coin54.draw();
    coin55.draw();
    coin56.draw();
    coin57.draw();
    coin58.draw();
    coin59.draw();

    character.draw();

    drawScoreboard(character.getX());
    drawLives(character.getX());
}


//---------------------------- STAGE 03 --------------------------------------------------------


void drawStage03 ()
{
    isStage3 = true;
    drawAllObstacleObjects();  // If you have obstacles call this function

    addRiver(-700, 0, 698, 70, 0.3f, 0.518f, 0.984f);

    addObstacle(400, 100, 60, 60, 0.22,0.34,0.41);// 1st block
    drawBorder(400, 100,460 , 160, 3.5f);

    addObstacle(550, 100, 60, 120, 0.22,0.34,0.41);// 2nd block
    drawBorder(550, 100,610 , 220, 3.5f);
    drawBrickLines(550, 100,610, 220, 60, 60, 4.0f);

    addObstacle(700, 100, 60, 180, 0.22,0.34,0.41);// 3rd block
    drawBorder(700, 100,760 , 280, 3.5f);
    drawBrickLines(700, 100,760, 280, 60, 60, 4.0f);

    addObstacle(850, 100, 60, 240, 0.22,0.34,0.41);// 4th block
    drawBorder(850, 100,910 , 340, 3.5f);
    drawBrickLines(850, 100,910, 340, 60, 60, 4.0f);

    addObstacle(1000, 100, 60, 180, 0.22,0.34,0.41);// 5th block
    drawBorder(1000, 100,1060 , 280, 3.5f);
    drawBrickLines(1000, 100,1060, 280, 60, 60, 4.0f);

    //S shape start
    addObstacle(1150, 265, 40, 160, 0.22,0.34,0.41);// 6th block
    drawBorder(1150, 265,1190 , 425, 3.5f);
    drawBrickLines(1150, 265,1190, 425, 40, 40, 4.0f);

    addObstacle(1150, 265, 120, 40, 0.22,0.34,0.41);// 7th block
    drawBorder(1150, 265,1270 , 305, 3.5f);
    drawBrickLines(1150, 265,1270, 305, 40, 40, 4.0f);

    addObstacle(1270, 265, 40, 160, 0.22,0.34,0.41);// 8th block
    drawBorder(1270, 265,1310 , 425, 3.5f);
    drawBrickLines(1270, 265,1310, 425, 40, 40, 4.0f);

    addObstacle(1270, 385, 160, 40, 0.22,0.34,0.41);// 9th block
    drawBorder(1270, 385,1430 , 425, 3.5f);
    drawBrickLines(1270, 385,1430, 425, 40, 40, 4.0f);

    addObstacle(1390, 265, 40, 160, 0.22,0.34,0.41);// 10th block
    drawBorder(1390, 265,1430 , 425, 3.5f);
    drawBrickLines(1390, 265,1430, 425, 40, 40, 4.0f);

    addObstacle(1390, 265, 120, 40, 0.22,0.34,0.41);// 11th block
    drawBorder(1390, 265,1510 , 305, 3.5f);
    drawBrickLines(1390, 265,1510, 305, 40, 40, 4.0f);

    addObstacle(1510, 265, 40, 160, 0.22,0.34,0.41);// 12th block
    drawBorder(1510, 265,1550 , 425, 3.5f);
    drawBrickLines(1510, 265,1550, 425, 40, 40, 4.0f);
    // S shape end

    addObstacle(1680, 310, 70, 210, 0.22,0.34,0.41);// 13th block
    drawBorder(1680, 310,1750 , 520, 3.5f);
    drawBrickLines(1680, 310,1750, 520, 70, 70, 4.0f);

    addObstacle(1850, 300, 200, 40, 0.22,0.34,0.41);// 14th block
    drawBorder(1850, 300,2050 , 340, 3.5f);
    drawBrickLines(1850, 300,2050, 340, 40, 40, 4.0f);

    addObstacle(2150, 300, 160, 40, 0.22,0.34,0.41);// 15th block
    drawBorder(2150, 300,2310 , 340, 3.5f);
    drawBrickLines(2150, 300,2310, 340, 40, 40, 4.0f);

    addObstacle(2430, 250, 70, 210, 0.22,0.34,0.41);// 16th block
    drawBorder(2430, 250,2500 , 460, 3.5f);
    drawBrickLines(2430, 250,2500, 460, 70, 70, 4.0f);

    addObstacle(2610, 280, 160, 40, 0.22,0.34,0.41);// 17th block
    drawBorder(2610, 280,2770 , 320, 3.5f);
    drawBrickLines(2610, 280,2770, 320, 40, 40, 4.0f);

    addRiver(1065, 0, 1700, 240, 0.55f, 0.53f, 0.94f);

    addObstacle(2900, 100, 124, 248, 0.22,0.34,0.41);// 18th block
    drawBorder(2900, 100,3024 , 348, 3.5f);
    drawBrickLines(2900, 100,3024, 348, 124, 124 , 4.0f);

    addObstacle(2870, 348, 184, 45, 0.22,0.34,0.41);// 19th block
    drawBorder(2870, 348,3054 , 393, 3.5f);

    addObstacle(3140, 200, 100, 35, 0.59f, 0.863f, 0.039f);//pipe upper
    drawBorder(3140, 200,3240 , 235, 3.5f);

    addObstacle(3145, 100, 90, 100, 0.59f, 0.863f, 0.039f);// pipe lower
    drawBorder(3145, 100,3235 , 200, 3.5f);

    addObstacle(3320, 300, 100, 35, 0.59f, 0.863f, 0.039f);//pipe upper
    drawBorder(3320, 300,3420 , 335, 3.5f);

    addObstacle(3325, 100, 90, 200, 0.59f, 0.863f, 0.039f);// pipe lower
    drawBorder(3325, 100,3415 , 300, 3.5f);

    addObstacle(3500, 200, 100, 35, 0.59f, 0.863f, 0.039f);//pipe upper
    drawBorder(3500, 200,3600 , 235, 3.5f);

    addObstacle(3505, 100, 90, 100, 0.59f, 0.863f, 0.039f);// pipe lower
    drawBorder(3505, 100,3595 , 200, 3.5f);

    addRiver(3605 , 0, 200, 102, 0.55f, 0.53f, 0.94f);

    addObstacle(3675, 150, 70, 280, 0.22,0.34,0.41);// 20th block
    drawBorder(3675, 150,3745 , 430, 3.5f);
    drawBrickLines(3675, 150,3745, 430, 70, 70 , 4.0f);

    addObstacle(3945, 100, 180, 45.0f, 0.22,0.34,0.41);
    drawBrickLines(3945, 100,4125 , 145, 45, 45 , 4.0f);
    addObstacle(3990, 145, 135, 45.0f, 0.22,0.34,0.41);
    drawBrickLines(3990, 145,4125, 190, 45, 45 , 4.0f);
    addObstacle(4035, 190, 90, 45.0f, 0.22,0.34,0.41);
    drawBrickLines(4035, 190,4125, 235, 45, 45 , 4.0f);
    addObstacle(4080, 235, 45, 45.0f, 0.22,0.34,0.41);
    drawBrickLines(4080, 235,4125, 280, 45, 45 , 4.0f);

    addRiver(4165  , 0, 680, 102, 0.55f, 0.53f, 0.94f);

    addObstacle(4215, 235, 120, 40, 0.22,0.34,0.41);// 21th block
    drawBorder(4215, 235,4335 , 275, 3.5f);
    drawBrickLines(4215, 235,4335, 275, 40, 40, 4.0f);

    addObstacle(4385, 350, 200, 50, 0.22,0.34,0.41);// 22th block
    drawBorder(4385, 350,4585 , 400, 3.5f);
    drawBrickLines(4385, 350,4585, 400, 50, 50, 4.0f);

    addObstacle(4635, 235, 120, 40, 0.22,0.34,0.41);// 23th block
    drawBorder(4635, 235,4755 , 275, 3.5f);
    drawBrickLines(4635, 235,4755, 275, 40, 40, 4.0f);

    addObstacle(4850, 100, 50, 300, 0.22,0.34,0.41);// 24th block
    drawBorder(4850, 100,4900,400, 3.5f);
    drawBrickLines(4850, 100,4900,400,50,50, 4.0f);

    addRiver(4905  , 0, 500, 102, 0.55f, 0.53f, 0.94f);

    addObstacle(4960, 250, 135, 45, 0.22,0.34,0.41);// 25th block
    drawBorder(4960, 250,5095,295, 3.5f);
    drawBrickLines(4960, 250,5095,295,45,45, 4.0f);

    addObstacle(5095, 250, 45, 225, 0.22,0.34,0.41);// 26th block
    drawBorder(5095, 250,5140,475, 3.5f);
    drawBrickLines(5095, 250,5140,475,45,45, 4.0f);

    addObstacle(5220, 250, 45, 225, 0.22,0.34,0.41);// 27th block
    drawBorder(5220, 250,5265,475, 3.5f);
    drawBrickLines(5220, 250,5265,475,45,45, 4.0f);

    addObstacle(5265, 250, 135, 45, 0.22,0.34,0.41);// 28th block
    drawBorder(5265, 250,5400,295, 3.5f);
    drawBrickLines(5265, 250,5400,295,45,45, 4.0f);

    addObstacle(5650, 250, 100, 35, 0.59f, 0.863f, 0.039f);//pipe upper
    drawBorder(5650, 250,5750 , 285, 3.5f);

    addObstacle(5655, 100, 90, 150, 0.59f, 0.863f, 0.039f);// pipe lower
    drawBorder(5655, 100,5745 , 250, 3.5f);

    addObstacle(5800, 290, 225, 45, 0.22,0.34,0.41);// 27th block
    drawBorder(5800, 290,6025,335, 3.5f);
    drawBrickLines(5800, 290,6025,335,45,45, 4.0f);


    addObstacle(6250, 100, 405, 45.0f, 0.22,0.34,0.41);
    drawBrickLines(6250, 100,6655 , 145, 45, 45 , 4.0f);
    addObstacle(6295, 145, 360, 45.0f, 0.22,0.34,0.41);
    drawBrickLines(6295, 145,6655, 190, 45, 45 , 4.0f);
    addObstacle(6340, 190, 315, 45.0f, 0.22,0.34,0.41);
    drawBrickLines(6340, 190,6655, 235, 45, 45 , 4.0f);
    addObstacle(6385, 235, 270, 45.0f, 0.22,0.34,0.41);
    drawBrickLines(6385, 235,6655, 280, 45, 45 , 4.0f);
    addObstacle(6430, 280, 225, 45.0f, 0.22,0.34,0.41);
    drawBrickLines(6430, 280,6655, 325, 45, 45 , 4.0f);
    addObstacle(6475, 325, 180, 45.0f, 0.22,0.34,0.41);
    drawBrickLines(6475, 325,6655, 370, 45, 45 , 4.0f);
    addObstacle(6520, 370, 135, 45.0f, 0.22,0.34,0.41);
    drawBrickLines(6520, 370,6655, 415, 45, 45 , 4.0f);
    addObstacle(6565, 415, 90, 45.0f, 0.22,0.34,0.41);
    drawBrickLines(6565, 415,6655, 460, 45, 45 , 4.0f);
    addObstacle(6610, 460, 45, 45.0f, 0.22,0.34,0.41);
    drawBrickLines(6610, 460,6655, 505, 45, 45 , 4.0f);

    addObstacle(100, 670, 8000, 50, 0.22,0.34,0.41);// upper ground shape
    drawBorder(100, 670,8000,720, 3.5f);
    drawBrickLines(100, 670,8000,720,50,50, 4.0f);

    addObstacle(100, 470, 50, 300, 0.22,0.34,0.41);// 1stup shape
    drawBorder(100, 470,150,670, 3.5f);
    drawBrickLines(100, 470,150,670,50,50, 4.0f);

    addObstacle(100, 100, 50, 150, 0.22,0.34,0.41);// 1st low shape
    drawBorder(100, 100,150,250, 3.5f);
    drawBrickLines(100, 100,150,250,50,50, 4.0f);

    glBegin(GL_LINE_STRIP);
    glLineWidth(50.0f);
    glColor3f(0.616f, 0.992f, 0.094f);
    glVertex2f(7048.5f, 147.0f);
    glVertex2f(7048.5f, 570.0f);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(0.9f, 0.9f, 0.9f);
    glVertex2f(7048.5f, 570.0f);
    glVertex2f(6998.5f, 570.0f);
    glVertex2f(7048.5f, 520.0f);
    glEnd();

    addObstacle(7026, 101, 45, 45, 0.22,0.34,0.41);
    drawBrickLines(7026, 101, 7026+45, 101+45, 45, 45, 3.0f);

    drawArchGate(7220, 101, 150, 100, 0, 0, 0);

    drawGround2(8000.0f);

    drawAllRiverObjects();

    coin14.draw();
    coin15.draw();
    coin16.draw();
    coin17.draw();
    coin18.draw();
    coin19.draw();
    coin20.draw();
    coin21.draw();
    coin22.draw();
    coin23.draw();
    coin24.draw();
    coin25.draw();
    coin26.draw();
    coin27.draw();
    coin28.draw();
    coin29.draw();
    coin30.draw();
    coin31.draw();
    coin32.draw();
    coin33.draw();
    coin34.draw();
    coin35.draw();
    coin36.draw();
    coin37.draw();
    coin38.draw();
    coin39.draw();
    coin40.draw();

    character.draw();

    drawScoreboard(character.getX());
    drawLives(character.getX());
}


/* Handler for window-repaint event. Call back when the window first appears and
whenever the window needs to be re-painted. */
void renderBitmapString(float x, float y, float z, void *font, char *string)
{
    char *c;
    glRasterPos3f(x, y,z);
    for (c=string; *c != '\0'; c++)
    {
        glutBitmapCharacter(font, *c);
    }
}


//Exit Function
void exit1(int value)
{
    exit(0);
    glutTimerFunc(10,exit1,0);
}

// Function to handle the display
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    if(lives >0){

        if(isStage1){
            cameraMovement();
            drawStage01();
        }

        else if(isStage2){
             if (obstacleClearCount==3){
                obstacles.clear();
                rivers.clear();
                obstacleClearCount--;
             }
            cameraMovement();
            drawStage02();
        }

        else if(isStage3){
            if (obstacleClearCount==2){
            obstacles.clear();
            rivers.clear();
            obstacleClearCount--;
            }
            cameraMovement();
            drawStage03();
        }
        else if(winner){
            glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Set background color to black
            glClear(GL_COLOR_BUFFER_BIT); // Clear the color buffer (background)

            glColor3f(1.0,0.0,0.0);
            renderBitmapString(7100, 400.0f, 0.0f, GLUT_BITMAP_TIMES_ROMAN_24, "Congratulation! You Won");
            drawText("Score: " + to_string(score), 7100, 360.0f, GLUT_BITMAP_TIMES_ROMAN_24, 1, 1, 1);
            glFlush(); // Render now

            glutTimerFunc(9000,exit1,0);
        }
        else {
            menu();
        }
    }
    else{
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Set background color to black
        glClear(GL_COLOR_BUFFER_BIT); // Clear the color buffer (background)

        glColor3f(1.0,0.0,0.0);
        renderBitmapString(deadPosition, 400.0f, 0.0f, GLUT_BITMAP_TIMES_ROMAN_24, "GAME OVER");
        drawText("Score: " + to_string(score), deadPosition, 360.0f, GLUT_BITMAP_TIMES_ROMAN_24, 1, 1, 1);
        glFlush(); // Render now

        glutTimerFunc(9000,exit1,0);
    }
    glutSwapBuffers();
}


//Play Sound In The Background
void sound()    {
    PlaySound("mario.wav", NULL,SND_ASYNC|SND_FILENAME|SND_LOOP);
}


//Function to handle key presses
void specialKeyPressed (int key, int x, int y){
    character.specialKeyPressed(key, x, y);
}


//Function to handle key releases
void specialKeyReleased (int key, int x, int y){
    character.specialKeyReleased(key, x, y);
}


// Function to set up the OpenGL environment
void init() {
    glClearColor(0.529, 0.808, 0.980, 1.0f); // Set the background color to white
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, coordinateX, 0, coordinateY); // Set the coordinate system
}


// Function to handle window resizing
void reshape(int width, int height) {
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
}

//........................Drawing Complete.....................

//.............................Driver code or main function Start.........................

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(windowFinalX, windowFinalY);
    glutCreateWindow("SUPER MARIO GAME");
    init();
    sound();
    glutKeyboardFunc(keyboard); //Set Menu Keyboard Function
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(0, update, 0); // Start the character movement timer
    glutSpecialFunc(specialKeyPressed); // Set the special key callback function
    glutSpecialUpFunc(specialKeyReleased); // Set the special key release callback function
    glutMainLoop();
    return 0;
}

//.......................................Main Function End...............................
