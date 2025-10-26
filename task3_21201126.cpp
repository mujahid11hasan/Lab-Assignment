#include <GL/glut.h>
#include <iostream>
#include <vector>
using namespace std;

// Structure for line segment
struct Line {
    int x1, y1, x2, y2;
};

vector<Line> lines; // input lines
int xmin, ymin, xmax, ymax; // clipping window

// Plot pixel
void plotPixel(int x, int y) {
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}

// Draw a line using simple Bresenham (for visualization)
void drawLine(int x1, int y1, int x2, int y2) {
    int dx = abs(x2-x1), dy = abs(y2-y1);
    int sx = (x2<x1)?-1:1, sy=(y2<y1)?-1:1;
    int err = dx - dy;

    while(true) {
        plotPixel(x1,y1);
        if(x1==x2 && y1==y2) break;
        int e2 = 2*err;
        if(e2 > -dy){ err -= dy; x1+=sx;}
        if(e2 < dx){ err += dx; y1+=sy;}
    }
}

// Liang-Barsky algorithm
bool liangBarsky(int x0, int y0, int x1, int y1, int &cx0, int &cy0, int &cx1, int &cy1) {
    double u1 = 0.0, u2 = 1.0;
    int dx = x1 - x0;
    int dy = y1 - y0;

    auto cliptest = [&](int p, int q, double &u1, double &u2) -> bool {
        if(p == 0) {
            if(q < 0) return false; // parallel & outside
        } else {
            double r = (double)q/p;
            if(p < 0) {
                if(r > u2) return false;
                else if(r > u1) u1 = r;
            } else if(p > 0) {
                if(r < u1) return false;
                else if(r < u2) u2 = r;
            }
        }
        return true;
    };

    if( cliptest(-dx, x0 - xmin, u1, u2) &&
        cliptest( dx, xmax - x0, u1, u2) &&
        cliptest(-dy, y0 - ymin, u1, u2) &&
        cliptest( dy, ymax - y0, u1, u2) ) {
            cx0 = x0 + u1*dx;
            cy0 = y0 + u1*dy;
            cx1 = x0 + u2*dx;
            cy1 = y0 + u2*dy;
            return true;
    }
    return false; // line is outside
}

// Draw clipping rectangle
void drawClippingWindow() {
    glColor3f(0.5,0.5,0.5); // grey
    glBegin(GL_LINE_LOOP);
    glVertex2i(xmin,ymin);
    glVertex2i(xmax,ymin);
    glVertex2i(xmax,ymax);
    glVertex2i(xmin,ymax);
    glEnd();
}

// Display
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    drawClippingWindow();

    // Draw original lines (blue)
    glColor3f(0,0,1);
    for(auto l: lines) drawLine(l.x1,l.y1,l.x2,l.y2);

    // Draw clipped lines (red)
    glColor3f(1,0,0);
    for(auto l: lines) {
        int cx0, cy0, cx1, cy1;
        if(liangBarsky(l.x1,l.y1,l.x2,l.y2,cx0,cy0,cx1,cy1))
            drawLine(cx0,cy0,cx1,cy1);
    }

    glFlush();
}

// Init
void init() {
    glClearColor(0,0,0,1);
    gluOrtho2D(-500,500,-500,500);
}

// Main
int main(int argc, char** argv) {
    int n;
    cout << "Enter clipping window (xmin ymin xmax ymax): ";
    cin >> xmin >> ymin >> xmax >> ymax;
    cout << "Enter number of lines: ";
    cin >> n;
    for(int i=0;i<n;i++) {
        Line l;
        cout << "Enter x1 y1 x2 y2 for line " << i+1 << ": ";
        cin >> l.x1 >> l.y1 >> l.x2 >> l.y2;
        lines.push_back(l);
    }

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800,800);
    glutInitWindowPosition(100,100);
    glutCreateWindow("Liang-Barsky Line Clipping");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
