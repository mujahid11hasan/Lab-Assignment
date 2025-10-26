#include <GL/glut.h>
#include <cmath>
#include <iostream>
using namespace std;

// ================== Global Variables ==================
int xStart, yStart, xEnd, yEnd;  // user input points
int lineWidth;                   // thickness
int drawMode = 1;                // 1 = Normal, 2 = 8-way Thick

// ================== Pixel Plotter ==================
void putPixel(int px, int py) {
    glBegin(GL_POINTS);
    glVertex2i(px, py);
    glEnd();
}

// ================== Standard Bresenham Algorithm ==================
void drawBresenhamLine(int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x2 < x1) ? -1 : 1;
    int sy = (y2 < y1) ? -1 : 1;
    int err = dx - dy;

    while (true) {
        putPixel(x1, y1);
        if (x1 == x2 && y1 == y2) break;
        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x1 += sx; }
        if (e2 < dx)  { err += dx; y1 += sy; }
    }
}

// ================== 8-Way Symmetric Thick Line ==================
void drawThickLine8Way(int x1, int y1, int x2, int y2, int t) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x2 < x1) ? -1 : 1;
    int sy = (y2 < y1) ? -1 : 1;
    int err = dx - dy;
    int radius = t / 2;

    while (true) {
        // 8-way symmetric pixel fill around center pixel
        for (int i = -radius; i <= radius; i++) {
            for (int j = -radius; j <= radius; j++) {
                if (i * i + j * j <= radius * radius) { // circular region fill
                    putPixel(x1 + i, y1 + j);
                }
            }
        }

        if (x1 == x2 && y1 == y2) break;
        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x1 += sx; }
        if (e2 < dx)  { err += dx; y1 += sy; }
    }
}

// ================== Draw Axes ==================
void drawAxes() {
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINES);
    glVertex2i(-500, 0); glVertex2i(500, 0);
    glVertex2i(0, -500); glVertex2i(0, 500);
    glEnd();
}

// ================== Text Helper ==================
void printText(float x, float y, const char *text) {
    glRasterPos2f(x, y);
    for (int i = 0; text[i] != '\0'; i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
}

// ================== Display ==================
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    drawAxes();

    glColor3f(1, 1, 0);
    printText(-480, 460, "Press 1 = Normal | 2 = Thick (8-way) | ESC = Exit");

    if (drawMode == 1) {
        glColor3f(0.0, 1.0, 0.0);
        drawBresenhamLine(xStart, yStart, xEnd, yEnd);
        printText(-90, 440, "[Normal Bresenham Line]");
    } else if (drawMode == 2) {
        glColor3f(1.0, 0.0, 0.0);
        drawThickLine8Way(xStart, yStart, xEnd, yEnd, lineWidth);
        printText(-80, 440, "[8-Way Symmetric Thick Line]");
    }

    glFlush();
}

// ================== Keyboard Events ==================
void handleKey(unsigned char key, int x, int y) {
    switch (key) {
        case '1': drawMode = 1; break;
        case '2': drawMode = 2; break;
        case 27: exit(0);
    }
    glutPostRedisplay();
}

// ================== Initialization ==================
void initDisplay() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    gluOrtho2D(-500, 500, -500, 500);
    glPointSize(2.5);
}

// ================== Main ==================
int main(int argc, char** argv) {
    cout << "Enter starting point (x1 y1): ";
    cin >> xStart >> yStart;
    cout << "Enter ending point (x2 y2): ";
    cin >> xEnd >> yEnd;
    cout << "Enter line thickness: ";
    cin >> lineWidth;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Bresenham Line - 8-Way Thick Version");
    initDisplay();
    glutDisplayFunc(display);
    glutKeyboardFunc(handleKey);
    glutMainLoop();
    return 0;
}
