#include <GL/glut.h>
#include <cmath>
#include <iostream>
using namespace std;

int xc = 0, yc = 0;        // Center
int startRadius = 50;       // Innermost circle radius
int numCircles = 10;        // Number of circles
int radiusStep = 20;        // Increase in radius per circle
int thicknessStep = 2;      // Thickness increment per circle

// Plot single pixel
void plotPixel(int x, int y) {
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}

// Midpoint Circle Algorithm (single circle)
void drawCircle(int xc, int yc, int r) {
    int x = 0;
    int y = r;
    int d = 1 - r;

    while (x <= y) {
        plotPixel(xc + x, yc + y);
        plotPixel(xc - x, yc + y);
        plotPixel(xc + x, yc - y);
        plotPixel(xc - x, yc - y);
        plotPixel(xc + y, yc + x);
        plotPixel(xc - y, yc + x);
        plotPixel(xc + y, yc - x);
        plotPixel(xc - y, yc - x);

        if (d < 0) {
            d += 2 * x + 3;
        } else {
            d += 2 * (x - y) + 5;
            y--;
        }
        x++;
    }
}

// Draw concentric circles with thickness & gradient
void drawConcentricCircles() {
    for (int i = 0; i < numCircles; i++) {
        int r = startRadius + i * radiusStep;
        int thickness = 1 + i * thicknessStep;

        // Color gradient (from red to blue for example)
        float t = (float)i / numCircles; // 0 to 1
        glColor3f(1.0 - t, 0.0, t); // Red→Blue gradient

        // Draw thick circle using multiple thin circles
        for (int tIndex = 0; tIndex < thickness; tIndex++) {
            drawCircle(xc, yc, r + tIndex);
        }
    }
}

// Draw X/Y axes
void drawAxes() {
    glColor3f(1,1,1);
    glBegin(GL_LINES);
    glVertex2i(-500,0); glVertex2i(500,0);
    glVertex2i(0,-500); glVertex2i(0,500);
    glEnd();
}

// Display
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    drawAxes();
    drawConcentricCircles();
    glFlush();
}

// Init
void init() {
    glClearColor(0,0,0,1);
    gluOrtho2D(-500,500,-500,500);
}

// Main
int main(int argc, char** argv) {
    cout << "Enter center xc yc: ";
    cin >> xc >> yc;
    cout << "Enter start radius: "; cin >> startRadius;
    cout << "Enter number of circles: "; cin >> numCircles;
    cout << "Enter radius step: "; cin >> radiusStep;
    cout << "Enter thickness step: "; cin >> thicknessStep;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800,800);
    glutInitWindowPosition(100,100);
    glutCreateWindow("Concentric Circles with Gradient");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
