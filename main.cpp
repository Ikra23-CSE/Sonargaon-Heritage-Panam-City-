#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>
#include <iostream>
#include <vector>
#pragma comment(lib, "winmm.lib")

#define PI 3.14159265358979323846

// SECTION 0


bool isHighlightOn = false;   // 'H' key
bool isLampOn = true;         // 'L' key
bool isNight = false;         // 'N' key (Day/Night)
bool isRippleActive = true;   // 'W' key
bool isMusicPlaying = false;  // 'M' key for Music
bool moveCar = true;          // 'C' key to freeze
float zoomScale = 1.0f;

// Boat (X and Y)
float userBoatX = 200.0f;
float userBoatY = 150.0f;

// Animation Variables
float carPosition1 = 500.0f;
float carPosition2 = 900.0f;
float busPosition = -200.0f;
float rickshawPosition = -100.0f;
float birdPosition = 0.0f;

// Automatic boat
float autoBoatX = 800.0f;

// Sky Animation Variables
float sunX = 1050.0f;
float cloudOffset = 0.0f;

// Water Ripple
float rippleOffset = 0.0f;

// Leaf Animation
struct Leaf {
    float x;
    float y;
    float startX;
    float speed;
    float swayOffset;
    bool active;
};

struct Star {
    float x, y, size;
};
std::vector<Star> stars;

const int NUM_LEAVES = 30;
Leaf leaves[NUM_LEAVES];



// SECTION 1

void drawCircle(float cx, float cy, float r, int segments, float red, float green, float blue) {
    glColor3f(red, green, blue);
    glBegin(GL_POLYGON);
    for (int i = 0; i < segments; i++) {
        float theta = 2.0f * PI * float(i) / float(segments);
        float x = r * cosf(theta);
        float y = r * sinf(theta);
        glVertex2f(x + cx, y + cy);
    }
    glEnd();
}

void drawRing(float cx, float cy, float r, float thickness, float red, float green, float blue) {
    glColor3f(red, green, blue);
    glBegin(GL_TRIANGLE_STRIP);
    for (int i = 0; i <= 30; i++) {
        float theta = 2.0f * PI * float(i) / 30.0f;
        float xOut = r * cosf(theta);
        float yOut = r * sinf(theta);
        float xIn = (r - thickness) * cosf(theta);
        float yIn = (r - thickness) * sinf(theta);
        glVertex2f(cx + xOut, cy + yOut);
        glVertex2f(cx + xIn, cy + yIn);
    }
    glEnd();
}

void drawRect(float x, float y, float width, float height, float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
}

// 'H' key (Highlighting)
void drawRectOutline(float x, float y, float width, float height) {
    glBegin(GL_LINE_LOOP);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
}

void drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3, float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_TRIANGLES);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glVertex2f(x3, y3);
    glEnd();
}

void drawDome(float cx, float cy, float r, float red, float green, float blue) {
    glColor3f(red, green, blue);
    glBegin(GL_POLYGON);
    for (int i = 0; i <= 180; i++) {
        float theta = PI * float(i) / 180.0f;
        float x = r * cosf(theta);
        float y = r * sinf(theta);
        glVertex2f(x + cx, y + cy);
    }
    glEnd();
}

void drawArch(float x, float y, float w, float h, float r, float g, float b) {
    drawRect(x, y, w, h - (w/2), r, g, b);
    drawDome(x + w/2, y + h - (w/2), w/2, r, g, b);
}

void drawLeaf(float x, float y) {
    // Leaves at night
    float dim = isNight ? 0.6f : 1.0f;
    glColor3f(0.5f * dim, 0.9f * dim, 0.3f * dim);
    glBegin(GL_POLYGON);
        glVertex2f(x, y);
        glVertex2f(x + 5, y + 3);
        glVertex2f(x, y + 8);
        glVertex2f(x - 5, y + 3);
    glEnd();
}

// SECTION 2

void drawCityscape() {
    float baseY = 280.0f;
    float dim = isNight ? 0.4f : 1.0f;

    // Window colors
    float winR = isNight ? 1.0f : 1.0f;
    float winG = isNight ? 1.0f : 0.9f;
    float winB = isNight ? 0.0f : 0.6f;

    // Building 1
    drawRect(30, baseY, 80, 180, 0.4f * dim, 0.2f * dim, 0.1f * dim);
    drawRect(45, baseY + 180, 50, 40, 0.4f * dim, 0.2f * dim, 0.1f * dim);

    for(int wy = 20; wy < 200; wy += 30) {
        drawRect(40, baseY + wy, 15, 15, winR, winG, winB);
        drawRect(85, baseY + wy, 15, 15, winR, winG, winB);
    }

    // Building 2
    drawRect(150, baseY, 100, 220, 0.5f * dim, 0.3f * dim, 0.2f * dim);
    drawRect(150, baseY + 220, 100, 5, 0.2f * dim, 0.1f * dim, 0.05f * dim);
    drawRect(190, baseY + 20, 20, 180, 0.1f * dim, 0.1f * dim, 0.1f * dim);

    // Building 3
    drawRect(920, baseY, 90, 160, 0.3f * dim, 0.15f * dim, 0.05f * dim);
    drawRect(920, baseY + 160, 40, 40, 0.3f * dim, 0.15f * dim, 0.05f * dim);
    drawRect(970, baseY + 160, 40, 40, 0.3f * dim, 0.15f * dim, 0.05f * dim);
    for(int wy = 20; wy < 150; wy += 20) {
        drawRect(935, baseY + wy, 10, 8, winR, winG, winB);
        drawRect(985, baseY + wy, 10, 8, winR, winG, winB);
    }

    // Building 4
    drawRect(1030, baseY, 60, 280, 0.55f * dim, 0.35f * dim, 0.15f * dim);
    drawRect(1025, baseY + 280, 70, 8, 0.2f * dim, 0.1f * dim, 0.0f);
    for(int wy = 40; wy < 260; wy += 40) {
       drawRect(1040, baseY + wy, 40, 10, winR, winG, 1.0f); // Slight blue tint for glass
    }

    // Building 5
    drawRect(1110, baseY, 80, 200, 0.45f * dim, 0.25f * dim, 0.15f * dim);
    drawRect(1090, baseY, 30, 100, 0.45f * dim, 0.25f * dim, 0.15f * dim);
    drawRect(1150, baseY + 200, 2, 30, 0.2f * dim, 0.2f * dim, 0.2f * dim);
}

void drawCityscapeHighlights() {
    float baseY = 280.0f;
    glLineWidth(3.0f);
    glColor3f(1.0f, 1.0f, 0.0f);

    drawRectOutline(30, baseY, 80, 180);
    drawRectOutline(45, baseY + 180, 50, 40);
    drawRectOutline(150, baseY, 100, 220);
    drawRectOutline(920, baseY, 90, 160);
    drawRectOutline(920, baseY + 160, 40, 40);
    drawRectOutline(970, baseY + 160, 40, 40);
    drawRectOutline(1030, baseY, 60, 280);
    drawRectOutline(1110, baseY, 80, 200);
    drawRectOutline(1090, baseY, 30, 100);

    glLineWidth(1.0f);
}

void drawRickshaw(float x, float y) {
    drawRing(x + 15, y + 15, 15, 2, 0.1f, 0.1f, 0.1f);
    drawRing(x + 65, y + 15, 15, 2, 0.1f, 0.1f, 0.1f);
    drawRect(x + 15, y + 15, 50, 5, 0.2f, 0.2f, 0.2f);
    drawRect(x + 25, y + 20, 45, 20, 0.0f, 0.5f, 0.2f);
    drawRect(x + 25, y + 35, 45, 5, 1.0f, 0.8f, 0.0f);
    drawDome(x + 47, y + 40, 25, 0.1f, 0.1f, 0.1f);
    glLineWidth(2.0f);
    glColor3f(0.3f, 0.2f, 0.1f);
    glBegin(GL_LINES);
        glVertex2f(x + 15, y + 20);
        glVertex2f(x - 10, y + 25);
    glEnd();
    drawCircle(x - 12, y + 40, 6, 10, 0.6f, 0.4f, 0.3f);
    drawRect(x - 15, y + 20, 8, 20, 0.8f, 0.8f, 0.8f);
}

void drawBigTree(float x, float y) {
    float dim = isNight ? 0.5f : 1.0f;
    drawRect(x, y, 40, 140, 0.35f * dim, 0.2f * dim, 0.1f * dim);
    drawRect(x, y, 10, 140, 0.3f * dim, 0.15f * dim, 0.05f * dim);
    drawCircle(x - 30, y + 110, 50, 20, 0.0f * dim, 0.4f * dim, 0.0f * dim);
    drawCircle(x + 70, y + 110, 50, 20, 0.0f * dim, 0.4f * dim, 0.0f * dim);
    drawCircle(x - 10, y + 130, 60, 20, 0.0f * dim, 0.5f * dim, 0.0f * dim);
    drawCircle(x + 50, y + 130, 60, 20, 0.0f * dim, 0.5f * dim, 0.0f * dim);
    drawCircle(x + 20, y + 120, 55, 20, 0.0f * dim, 0.45f * dim, 0.0f * dim);
    drawCircle(x + 20, y + 180, 60, 20, 0.0f * dim, 0.6f * dim, 0.0f * dim);
    drawCircle(x - 15, y + 160, 45, 20, 0.1f * dim, 0.7f * dim, 0.1f * dim);
    drawCircle(x + 55, y + 160, 45, 20, 0.1f * dim, 0.7f * dim, 0.1f * dim);
}

void drawBoat(float x, float y) {
    float dim = isNight ? 0.7f : 1.0f;
    glColor3f(0.4f * dim, 0.2f * dim, 0.1f * dim);
    glBegin(GL_POLYGON);
    glVertex2f(x, y + 20);
    glVertex2f(x + 10, y);
    glVertex2f(x + 90, y);
    glVertex2f(x + 100, y + 20);
    glEnd();
    drawDome(x + 50, y + 20, 25, 0.7f * dim, 0.5f * dim, 0.3f * dim);
    drawRect(x + 50, y + 20, 3, 60, 0.3f * dim, 0.2f * dim, 0.1f * dim);
    drawTriangle(x + 52, y + 70, x + 52, y + 30, x + 90, y + 50, 0.9f * dim, 0.9f * dim, 0.9f * dim);
}

void drawStreetLamp(float x, float y) {
    drawRect(x, y, 5, 100, 0.2f, 0.2f, 0.2f);
    drawRect(x - 15, y + 95, 20, 5, 0.2f, 0.2f, 0.2f);
    drawDome(x - 15, y + 95, 8, 0.2f, 0.2f, 0.2f);

    bool actualLampOn = isNight ? true : isLampOn;

    float r = actualLampOn ? 1.0f : 0.3f;
    float g = actualLampOn ? 1.0f : 0.3f;
    float b = actualLampOn ? 0.8f : 0.3f;

    drawCircle(x - 15, y + 90, 6, 10, r, g, b);

    if (actualLampOn) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(1.0f, 1.0f, 0.6f, 0.4f);
        glBegin(GL_TRIANGLES);
            glVertex2f(x - 15, y + 90);
            glVertex2f(x - 45, y);
            glVertex2f(x + 15, y);
        glEnd();
        glDisable(GL_BLEND);
    }
}

void drawBird(float x, float y) {
    if(isNight) return; // Birds sleep at night
    glLineWidth(2.0f); glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINE_STRIP); glVertex2f(x, y); glVertex2f(x + 10, y - 5); glVertex2f(x + 20, y); glEnd();
}

void drawTrafficLight(float x, float y) {
    drawRect(x, y, 5, 60, 0.3f, 0.3f, 0.3f);
    drawRect(x - 10, y + 60, 25, 50, 0.1f, 0.1f, 0.1f);
    // lights
    static int frame = 0;
    frame++;
    float r = (frame % 100 < 50) ? 1.0f : 0.2f;
    float g = (frame % 100 >= 50) ? 1.0f : 0.2f;

    drawCircle(x + 2, y + 100, 5, 10, r, 0.0f, 0.0f); // Red
    drawCircle(x + 2, y + 85, 5, 10, 1.0f, 1.0f, 0.0f); // Yellow
    drawCircle(x + 2, y + 70, 5, 10, 0.0f, g, 0.0f); // Green
}

void drawBench(float x, float y) {
    drawRect(x, y, 5, 15, 0.3f, 0.2f, 0.1f);
    drawRect(x + 45, y, 5, 15, 0.3f, 0.2f, 0.1f);
    drawRect(x - 5, y + 15, 60, 5, 0.5f, 0.3f, 0.1f);
    drawRect(x - 5, y + 20, 60, 15, 0.5f, 0.3f, 0.1f);
}

void drawFlower(float x, float y) {
    drawRect(x - 1, y, 2, 15, 0.0f, 0.5f, 0.0f);
    drawCircle(x, y + 15, 5, 10, 1.0f, 0.4f, 0.7f);
    drawCircle(x, y + 15, 2, 10, 1.0f, 1.0f, 0.0f);
}

void drawCloud(float x, float y, float s) {
    // Clouds are grey at night
    float col = isNight ? 0.2f : 1.0f;
    drawCircle(x, y, 30 * s, 30, col, col, col);
    drawCircle(x + 25 * s, y + 10 * s, 35 * s, 30, col, col, col);
    drawCircle(x + 55 * s, y, 30 * s, 30, col, col, col);
}

void drawBus(float x, float y) {
    drawRect(x, y, 160, 70, 0.9f, 0.1f, 0.1f);
    drawRect(x + 10, y + 40, 110, 20, 0.8f, 0.9f, 1.0f);
    drawCircle(x + 30, y, 15, 20, 0.1f, 0.1f, 0.1f);
    drawCircle(x + 130, y, 15, 20, 0.1f, 0.1f, 0.1f);
}

void drawCar(float x, float y, float r, float g, float b) {
    drawRect(x, y, 120, 35, r, g, b);
    drawRect(x + 20, y + 35, 70, 25, r, g, b);
    drawRect(x + 25, y + 38, 55, 18, 0.9f, 0.9f, 1.0f);
    drawCircle(x + 25, y, 14, 20, 0.1f, 0.1f, 0.1f);
    drawCircle(x + 95, y, 14, 20, 0.1f, 0.1f, 0.1f);

    // Headlights at night
    if(isNight) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(1.0f, 1.0f, 0.8f, 0.4f);
        // Headlight
        if(r < 0.2f) {
            drawCircle(x+120, y+20, 10, 10, 1.0, 1.0, 0.5);
        } else {
             drawCircle(x, y+20, 10, 10, 1.0, 1.0, 0.5);
        }
        glDisable(GL_BLEND);
    }
}

void drawGrandMosque(float mx, float my) {
    float floorY = my + 40;
    float dim = isNight ? 0.6f : 1.0f; // Dim

    // Ground
    drawRect(mx - 50, my, 600, 20, 0.8f * dim, 0.8f * dim, 0.8f * dim);
    drawRect(mx - 30, my + 20, 560, 20, 0.9f * dim, 0.9f * dim, 0.9f * dim);
    // Main Body
    drawRect(mx, floorY, 500, 200, 0.95f * dim, 0.92f * dim, 0.85f * dim);
    // Entrance
    drawRect(mx + 200, floorY, 100, 150, 0.7f * dim, 0.55f * dim, 0.3f * dim);
    drawArch(mx + 210, floorY, 80, 140, 0.3f * dim, 0.2f * dim, 0.1f * dim);

    for(int i=0; i<3; i++) {
        drawArch(mx + 20 + (i*60), floorY + 40, 40, 80, 0.3f * dim, 0.3f * dim, 0.3f * dim);
        drawArch(mx + 320 + (i*60), floorY + 40, 40, 80, 0.3f * dim, 0.3f * dim, 0.3f * dim);
    }
    // Top Layers
    drawRect(mx - 20, floorY + 200, 540, 15, 0.8f * dim, 0.5f * dim, 0.4f * dim);
    drawRect(mx + 190, floorY + 215, 120, 20, 0.9f * dim, 0.9f * dim, 0.9f * dim);
    // Main Dome
    drawDome(mx + 250, floorY + 235, 90, 1.0f * dim, 0.85f * dim, 0.2f * dim);
    drawRect(mx + 248, floorY + 325, 4, 30, 0.5f * dim, 0.5f * dim, 0.5f * dim);
    drawCircle(mx + 250, floorY + 355, 8, 10, 1.0f, 0.8f, 0.0f); // Spire stays bright

    // Side Domes
    drawDome(mx + 80, floorY + 215, 50, 1.0f * dim, 0.85f * dim, 0.2f * dim);
    drawDome(mx + 420, floorY + 215, 50, 1.0f * dim, 0.85f * dim, 0.2f * dim);

    // Minarets
    drawRect(mx - 60, floorY, 40, 280, 0.95f * dim, 0.92f * dim, 0.85f * dim);
    drawDome(mx - 40, floorY + 280, 25, 1.0f * dim, 0.85f * dim, 0.2f * dim);
    drawRect(mx + 520, floorY, 40, 280, 0.95f * dim, 0.92f * dim, 0.85f * dim);
    drawDome(mx + 540, floorY + 280, 25, 1.0f * dim, 0.85f * dim, 0.2f * dim);
}


// SECTION 3

void drawStars() {
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_POINTS);
    for(size_t i=0; i<stars.size(); i++) {
        glVertex2f(stars[i].x, stars[i].y);
    }
    glEnd();
}

void drawSkyLayer() {
    if(isNight) {
        drawRect(0, 0, 1200, 700, 0.05f, 0.05f, 0.2f); // Dark Night Blue
        drawStars();

        // Moon
        drawCircle(sunX, 620, 50, 50, 0.9f, 0.9f, 0.9f); // White Moon
        // Moon crater
        drawCircle(sunX - 15, 630, 10, 20, 0.8f, 0.8f, 0.8f);
    } else {
        drawRect(0, 0, 1200, 700, 0.53f, 0.81f, 0.98f); // Day Blue

        // Sun
        drawCircle(sunX, 620, 50, 50, 1.0f, 0.9f, 0.0f);
        glColor4f(1.0f, 1.0f, 0.0f, 0.3f);
        drawCircle(sunX, 620, 65, 30, 1.0f, 1.0f, 0.6f);
    }

    float c1 = 200 + cloudOffset;
    float c2 = 500 + cloudOffset;
    float c3 = 800 + cloudOffset;
    if(c1 > 1250) c1 -= 1300;
    if(c2 > 1250) c2 -= 1300;
    if(c3 > 1250) c3 -= 1300;

    drawCloud(c1, 600, 1.0f);
    drawCloud(c2, 630, 0.8f);
    drawCloud(c3, 580, 1.2f);

    float bx = birdPosition;
    drawBird(100 + bx, 650);
    drawBird(130 + bx, 670);
    drawBird(160 + bx, 655);
    drawBird(900 - bx, 680);

    // Horizon mist
    float mistR = isNight ? 0.1f : 0.6f;
    float mistG = isNight ? 0.1f : 0.7f;
    float mistB = isNight ? 0.2f : 0.8f;
    drawRect(0, 250, 1200, 80, mistR, mistG, mistB);
}

void drawGroundLayer() {
    drawCityscape();

    if(isHighlightOn) {
        drawCityscapeHighlights();
    }

    // Grass
    if(isNight) drawRect(0, 250, 1200, 200, 0.1f, 0.3f, 0.1f);
    else drawRect(0, 250, 1200, 200, 0.2f, 0.6f, 0.2f);

    drawGrandMosque(350, 280);

    drawBigTree(120, 260);
    drawBigTree(1000, 260);

    for(int i=0; i<10; i++) drawFlower(50 + i * 25, 260);
    for(int i=0; i<10; i++) drawFlower(900 + i * 25, 260);

    drawBench(100, 260);
    drawBench(1100, 260);

    for(int i = 0; i < NUM_LEAVES; i++) {
        if(leaves[i].active) {
            drawLeaf(leaves[i].x, leaves[i].y);
        }
    }
}

void drawRiverLayer() {
    // Water color
    if(isNight) drawRect(0, 120, 1200, 130, 0.05f, 0.1f, 0.3f);
    else drawRect(0, 120, 1200, 130, 0.2f, 0.5f, 0.9f);

    glLineWidth(1.0f);
    glColor3f(0.3f, 0.6f, 1.0f);

    for(int j = 0; j < 5; j++) {
        float yBase = 130.0f + (j * 25.0f);
        glBegin(GL_LINE_STRIP);
        for(int i = 0; i <= 1200; i += 10) {
            float angle = (float)i * 0.02f + rippleOffset + (j * 1.5f);
            float yOffset = sinf(angle) * 3.0f;
            glVertex2f((float)i, yBase + yOffset);
        }
        glEnd();
    }


    if(isNight) drawRect(sunX - 30, 160, 60, 3, 0.8f, 0.8f, 0.8f);
    else {
        drawRect(sunX - 40, 140, 80, 3, 1.0f, 1.0f, 0.6f);
        drawRect(sunX - 30, 160, 60, 3, 1.0f, 1.0f, 0.6f);
        drawRect(sunX - 20, 180, 40, 3, 1.0f, 1.0f, 0.6f);
    }


    float boatBob = sinf(rippleOffset) * 2.0f;

    drawBoat(userBoatX, userBoatY + boatBob);
    drawBoat(autoBoatX, 160 + boatBob);

    // Pier
    float mx = 350;
    float pierColor = isNight ? 0.4f : 0.7f;
    drawRect(mx + 200, 120, 100, 130, pierColor, pierColor, pierColor);
    drawRect(mx + 190, 120, 10, 130, pierColor-0.2f, pierColor-0.2f, pierColor-0.2f);
    drawRect(mx + 300, 120, 10, 130, pierColor-0.2f, pierColor-0.2f, pierColor-0.2f);
}

void drawRoadLayer() {
    // Road
    if(isNight) drawRect(0, 0, 1200, 120, 0.15f, 0.15f, 0.15f);
    else drawRect(0, 0, 1200, 120, 0.25f, 0.25f, 0.25f);

    for (int i = 0; i < 1200; i += 100) drawRect(i, 55, 60, 10, 1.0f, 1.0f, 1.0f);

    drawTrafficLight(1150, 20);
    drawRickshaw(rickshawPosition, 55);
    drawBus(busPosition, 20);
    drawCar(carPosition1, 35, 0.1f, 0.6f, 0.2f);
    drawCar(carPosition2, 25, 0.2f, 0.4f, 0.8f);

    for (int x = 150; x <= 1050; x += 300) {
        drawStreetLamp(x, 120);
    }
}

// SECTION 4

void handleKeypress(unsigned char key, int x, int y) {
    switch (key) {
        case 'n':
        case 'N':
            isNight = !isNight;
            break;
        case 'c':
        case 'C':
            moveCar = !moveCar;
            break;
        case 'h':
        case 'H':
            isHighlightOn = !isHighlightOn;
            break;
        case 'l':
        case 'L':
            isLampOn = !isLampOn;
            break;
        case 'w':
        case 'W':
            isRippleActive = !isRippleActive;
            break;
        case 'm':
        case 'M':
             if (!isMusicPlaying) {

                isMusicPlaying = true;
            } else {
                PlaySound(NULL, 0, 0); // Stop sound
                isMusicPlaying = false;
            }
            break;
        case ' ': // HORN
            Beep(800, 300);
            break;
        case '+':
            zoomScale += 0.1f;
            break;
        case '-':
            if(zoomScale > 0.2f) zoomScale -= 0.1f;
            break;
        case 27: // Escape Key
            exit(0);
            break;
    }
    glutPostRedisplay();
}

void handleSpecialKeys(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_RIGHT:
            userBoatX += 5.0f;
            break;
        case GLUT_KEY_LEFT:
            userBoatX -= 5.0f;
            break;
        case GLUT_KEY_UP:
            userBoatY += 5.0f;
            break;
        case GLUT_KEY_DOWN:
            userBoatY -= 5.0f;
            break;
    }

    if (userBoatX > 1250) userBoatX = -100;
    if (userBoatX < -150) userBoatX = 1200;
    if (userBoatY > 220) userBoatY = 220;
    if (userBoatY < 120) userBoatY = 120;

    glutPostRedisplay();
}

void update(int value) {

    // moveCar is true
    if (moveCar) {
        busPosition += 1.5f;
        if(busPosition > 1250) busPosition = -200;

        carPosition1 += 3.0f;
        if(carPosition1 > 1250) carPosition1 = -150;

        carPosition2 -= 4.0f;
        if(carPosition2 < -150) carPosition2 = 1250;

        rickshawPosition += 1.0f;
        if(rickshawPosition > 1250) rickshawPosition = -100;
    }

    // Automatic Boat ---
    autoBoatX -= 0.5f;
    if (autoBoatX < -150.0f) autoBoatX = 1250.0f;

    // Birds move
    birdPosition += 1.0f;
    if(birdPosition > 1300) birdPosition = 0;

    // Sky
    cloudOffset += 0.5f;
    if(cloudOffset > 1300) cloudOffset = -300;

    // Sun/Moon
    sunX -= 0.03f;
    if(sunX < -100) sunX = 1300;

    // Ripples
    if (isRippleActive) {
        rippleOffset += 0.1f;
    }

    // Leaves
    for(int i = 0; i < NUM_LEAVES; i++) {
        leaves[i].y -= leaves[i].speed;
        leaves[i].x = leaves[i].startX + (15.0f * sinf(leaves[i].y * 0.02f + leaves[i].swayOffset));

        if(leaves[i].y < 260) {
            leaves[i].y = 400 + (rand() % 50);
            leaves[i].startX = (rand() % 2 == 0) ? (120 + rand()%60 - 30) : (1000 + rand()%60 - 30);
            leaves[i].speed = 0.5f + ((float)(rand() % 10) / 10.0f);
        }
    }

    glutPostRedisplay();
    glutTimerFunc(25, update, 0);
}


// SECTION 5: MAIN DISPLAY


void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw the scene layers
    drawSkyLayer();
    drawGroundLayer();
    drawRiverLayer();
    drawRoadLayer();


    glutSwapBuffers();
}

void init() {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 1200, 0, 700);

    // Leaves
    for(int i = 0; i < NUM_LEAVES; i++) {
        int treeChoice = rand() % 2;
        float treeX = (treeChoice == 0) ? 120.0f : 1000.0f;
        leaves[i].startX = treeX + (rand() % 80 - 40);
        leaves[i].x = leaves[i].startX;
        leaves[i].y = 350.0f + (rand() % 100);
        leaves[i].speed = 0.5f + ((float)(rand() % 10) / 10.0f);
        leaves[i].swayOffset = (float)(rand() % 100) / 10.0f;
        leaves[i].active = true;
    }

    // Initialize Stars
    for(int i=0; i<100; i++) {
        Star s;
        s.x = rand() % 1200;
        s.y = 250 + rand() % 450;
        s.size = 1.0f + (rand() % 20)/10.0f;
        stars.push_back(s);
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(1200, 700);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("Dhaka Scenery");

    init();

    glutDisplayFunc(display);
    glutTimerFunc(25, update, 0);
    glutKeyboardFunc(handleKeypress);
    glutSpecialFunc(handleSpecialKeys);

    glutMainLoop();
    return 0;
}
