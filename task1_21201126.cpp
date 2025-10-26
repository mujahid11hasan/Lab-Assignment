#include <GL/glut.h>
#include <cmath>
#include <iostream>
using namespace std;

int x1_in, y1_in, x2_in, y2_in, thickness;
int mode = 1; // 1=Standard, 2=Thick (4-way)

void plotPixel(int x, int y) {
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}

// Standard Bresenham Line
void bresenhamLine(int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x2 < x1) ? -1 : 1;
    int sy = (y2 < y1) ? -1 : 1;
    int err = dx - dy;

    while (true) {
        plotPixel(x1, y1);
        if (x1 == x2 && y1 == y2) break;
        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x1 += sx; }
        if (e2 < dx)  { err += dx; y1 += sy; }
    }
}

// Thick Bresenham Line (4-way symmetry)
void bresenhamThickLine4Way(int x1, int y1, int x2, int y2, int thickness) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x2 < x1) ? -1 : 1;
    int sy = (y2 < y1) ? -1 : 1;
    int err = dx - dy;
    int half = thickness / 2;

    while (true) {
        for (int i = -half; i <= half; i++) {
            for (int j = -half; j <= half; j++) {
                // 4-way symmetry: only horizontal or vertical neighbors
                if (i == 0 || j == 0) {
                    plotPixel(x1 + i, y1 + j);
                }
            }
        }
        if (x1 == x2 && y1 == y2) break;
        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x1 += sx; }
        if (e2 < dx)  { err += dx; y1 += sy; }
    }
}

// Draw X and Y Axes
void drawAxes() {
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINES);
    glVertex2i(-500, 0); glVertex2i(500, 0);
    glVertex2i(0, -500); glVertex2i(0, 500);
    glEnd();
}

// Draw Text
void drawText(float x, float y, const char *text) {
    glRasterPos2f(x, y);
    for (int i = 0; text[i] != '\0'; i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
}

// Display
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    drawAxes();

    glColor3f(1, 1, 0);
    drawText(-480, 460, "Press 1 = Standard | 2 = Thick (4-way) | ESC = Exit");

    if (mode == 1) {
        glColor3f(0, 1, 0);
        bresenhamLine(x1_in, y1_in, x2_in, y2_in);
        drawText(-80, 440, "[Standard Line]");
    } else if (mode == 2) {
        glColor3f(1, 0, 0);
        bresenhamThickLine4Way(x1_in, y1_in, x2_in, y2_in, thickness);
        drawText(-70, 440, "[Thick Line - 4-way]");
    }

    glFlush();
}

// Keyboard
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case '1': mode = 1; break;
        case '2': mode = 2; break;
        case 27: exit(0);
    }
    glutPostRedisplay();
}

// Initialization
void init() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    gluOrtho2D(-500, 500, -500, 500);
}

// Main
int main(int argc, char** argv) {
    cout << "Enter x1 y1: "; cin >> x1_in >> y1_in;
    cout << "Enter x2 y2: "; cin >> x2_in >> y2_in;
    cout << "Enter line thickness (e.g. 5): "; cin >> thickness;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Bresenham Line Drawing - 4-way Thick");
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}
