/* *
 * @project nbodies
 * @file graphics.cpp
 *
 * @author Braden Nicholson
 * @date 3/8/21, 4/28/22
 */

#define SDL_MAIN_HANDLED

#include <SDL.h>


#include <iostream>
#include <utility>
#include <SDL_opengl.h>
#include <OpenGL/glu.h>

using std::string;
#define CHAR_WIDTH 7
#define CHAR_HEIGHT 8

#include "pixelfont.h"
#include "graphics.h"


Graphics::Graphics(int w, int h) : vw(w), vh(h) {
    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Init(SDL_INIT_VIDEO);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);


    window = SDL_CreateWindow("N-Bodies", SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED, w + 1, h + 1,
                              SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI);

    glContext = SDL_GL_CreateContext(window);
    camera = new Camera{0, 0, 0, 0, 0, 0, 0.75, 1024};


    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    SDL_GL_SetSwapInterval(-1);

    quad = gluNewQuadric();
    glMatrixMode(GL_MATRIX_MODE);
    glLoadIdentity();

    glOrtho(0, vw, vh, 0, -2048, 2048);
    glMatrixMode(GL_MODELVIEW);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    camera->scale = 0.75;
    SDL_SetWindowInputFocus(window);
}


Graphics::~Graphics() {
    gluDeleteQuadric(quad);
    SDL_DestroyRenderer(renderer);
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Graphics::clear() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

void Graphics::render() {
    SDL_GL_SwapWindow(window);
}

void Graphics::beginSimulationFrame() {
    glPushMatrix();


    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);

    glOrtho(-1024, 1024, 1024, -1024, -2048, 2048);

    glMatrixMode(GL_MODELVIEW);
    glTranslatef(-200, 0, 0);
    glScalef(camera->scale, camera->scale, camera->scale);

    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glEnable(GL_COLOR_MATERIAL);
    GLfloat ambientLight[] = {0.4f, 0.4f, 0.4f, 1.0f};
    GLfloat diffuseLight[] = {0.5f, 0.5f, 0.5f, 1.0f};
    GLfloat specularLight[] = {0.8f, 0.8f, 0.8f, 1.0f};
    GLfloat position[] = {0.0f, 0.0f, -1020.0f, 1.0f};

    glRotatef(camera->pitch, 1, 0, 0);
    glRotatef(camera->yaw, 0, 1, 0);

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
    glLightfv(GL_LIGHT0, GL_POSITION, position);


}

void Graphics::endSimulationFrame() {
    glDisable(GL_COLOR_MATERIAL);
    glDisable(GL_LIGHTING);
    glDisable(GL_NORMALIZE);
    glDisable(GL_LIGHT0);
    glDisable(GL_BLEND);
    glPopMatrix();
}

void Graphics::strokeRect(float x, float y, float w, float h) {
    glBegin(GL_POLYGON);
    glVertex2f(x, y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x, y + h);
    glEnd();
}

void Graphics::fillRect(float x, float y, float w, float h) {
    glBegin(GL_POLYGON);
    glVertex2f(x, y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x, y + h);
    glEnd();
}


void Graphics::setColor(int r, int g, int b) {
    GLfloat div = 255.0;
    glColor3f((GLfloat) r / div, (GLfloat) g / div, (GLfloat) b / div);
}

void Graphics::setAlphaColor(int r, int g, int b, int a) {
    GLfloat div = 255.0;
    glColor4f((GLfloat) r / div, (GLfloat) g / div, (GLfloat) b / div, (GLfloat) a / div);
}

void Graphics::drawMeter(float x, float y, float w, float h, float p) {
    setColor(128,128,128);
    strokeRect(x, y, w, h);
    setAlphaColor(32, 32, 32, 128);
    fillRect(x + 2, y + 2, (w - 4) * p, h - 4);
}

void Graphics::drawLine(float x1, float y1, float z1, float x2, float y2, float z2) {
    glBegin(GL_LINES);
    glVertex3f(x1, y1, z1);
    glVertex3f(x2, y2, z2);
    glEnd();
}

void Graphics::drawString(string s, float size, float x, float y) {
    float pos = x;

    for (char c: s) {
        int w = drawChar(c, size, pos, y);
        pos += w * size + (4 * log(size));
    }
}


void Graphics::drawString3D(string s, float size, float x, float y, float z) {
    glPushMatrix();
    glTranslatef(x, y, z);
    drawString(std::move(s), size, 0, 0);
    glPopMatrix();
}

void Graphics::drawString3DFace(string s, float size, float x, float y, float z) {
    glPushMatrix();
    glTranslatef(x, y, z);

    glRotatef(-camera->yaw, 0, 1, 0);
    glRotatef(-camera->pitch, 1, 0, 0);


    drawString3D(std::move(s), size, -(float) CHAR_WIDTH * size / 2, -CHAR_HEIGHT * 3, 0);

    glPopMatrix();
}

float Graphics::drawStringGetLength(string s, float size, float x, float y) {
    float pos = x;

    for (char c: s) {
        int w = drawChar(c, size, pos, y);
        pos += (float) w * size + (4 * log(size));
    }
    return pos - x + 16;
}

int Graphics::drawChar(char c, float size, float x, float y) {
    char ch = c & 0x7F;
    if (ch < ' ') {
        ch = 0;
    } else {
        ch -= ' ';
    }
    int aWidth = 0;

    // 'font' is a multidimensional array of [96][char_width]
    // which is really just a 1D array of size 96*char_width.
    const uint8_t *chr = font[ch];
    // Draw pixels
    for (int j = 0; j < CHAR_WIDTH; j++) {
        if (chr[j] == 0x00) continue;
        aWidth++;
        for (int i = 0; i < CHAR_HEIGHT; i++) {
            if (chr[j] & (1 << i)) {
                fillRect(x + j * size, y + i * size, size, size);
            }
        }
    }
    return aWidth;
}


void Graphics::drawOrigin(float x, float y, float z, float w, float h, float d) {


    glBegin(GL_LINES);
    glColor3f(0.5, 0.5, 1);
    glVertex3f(x, y, z - d);
    glVertex3f(x, y, z + d);

    glColor3f(0.5, 1, 0.5);
    glVertex3f(x - w, y, z);
    glVertex3f(x + w, y, z);

    glColor3f(1, 0.5, 0.5);
    glVertex3f(x, y - h, z);
    glVertex3f(x, y + h, z);
    glEnd();


    glPushMatrix();
    glColor3f(0.5, 1, 0.5);
    drawString3DFace("- X", 6, x - w, y, z);
    drawString3DFace("+ X", 6, x + w, y, z);
    glPopMatrix();

    glPushMatrix();
    glColor3f(1, 0.5, 0.5);
    drawString3DFace("- Y", 6, x, y - h, z);
    drawString3DFace("+ Y", 6, x, y + h, z);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.5, 0.5, 1);
    drawString3DFace("- Z", 6, x, y, z - d);
    drawString3DFace("+ Z", 6, x, y, z + d);
    glPopMatrix();


}


void Graphics::drawCube(float x, float y, float z, float w, float h, float
d) {


    glBegin(GL_LINES);
    glColor3f(0.5, 0.5, 0.5);
    glVertex3f(x, y, z + d);

    glVertex3f(x + w, y, z + d);

    glVertex3f(x, y, z);

    glVertex3f(x + w, y, z);

    glVertex3f(x, y + h, z);

    glVertex3f(x + w, y + h, z);

    glVertex3f(x + w, y + h, z + d);

    glVertex3f(x, y + h, z + d);

    // Next

    glVertex3f(x, y, z + d);

    glVertex3f(x, y, z);

    glVertex3f(x + w, y, z + d);

    glVertex3f(x + w, y, z);


    glVertex3f(x, y + h, z + d);

    glVertex3f(x, y + h, z);

    glVertex3f(x + w, y + h, z + d);

    glVertex3f(x + w, y + h, z);


    glVertex3f(x, y + h, z);
    glVertex3f(x, y, z);

    glVertex3f(x + w, y + h, z);
    glVertex3f(x + w, y, z);

    glVertex3f(x + w, y + h, z + d);
    glVertex3f(x + w, y, z + d);

    glVertex3f(x, y + h, z + d);
    glVertex3f(x, y, z + d);


    glEnd();

}


void Graphics::drawSphere(float x, float y, float z, float r) {
    glPushMatrix();
    glTranslatef(x, y, z);
    gluSphere(quad, r, 12, 8);
    glPopMatrix();
}

void Graphics::scaleCamera(float s) {
    camera->scale = s;
}

void Graphics::centerFull(float s) {
    float delta = s / (camera->radius * camera->scale);
    camera->scale *= delta;
}

void Graphics::setRadius(float r) {
    // Maintain scale with expansion
    float delta = camera->radius / r;
    camera->scale *= delta;
    camera->radius = r;
}

void Graphics::rotate(float y, float r, float p) {
    camera->yaw = y;
    camera->pitch = r;
}

void Graphics::drawCircle(float x, float y, float r) {
    float points = r;
    glBegin(GL_LINES);
    for (int i = 0; i < (int) 20; i++) {
        float phiDx = (float) (M_PI * 2) / points;
        float phi = phiDx * (float) i;
        glVertex3f(x + r * cos(phi), y + r * sin(phi), 0);
    }
    glEnd();

}

void Graphics::materialEmit(float r, float g, float b, float f) {
    GLfloat emit[] = {(GLfloat) r, (GLfloat) g, (GLfloat) b, (GLfloat) f};
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emit);
}

void Graphics::materialColor(float r, float g, float b) {

    GLfloat ambientLight[] = {(GLfloat) r, (GLfloat) g, (GLfloat) b, 1.0f};
    GLfloat diffuseLight[] = {(GLfloat) r, (GLfloat) g, (GLfloat) b, 1};
    glColor3f((GLfloat) r, (GLfloat) g, (GLfloat) b);
// Assign created components to GL_LIGHT0.
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambientLight);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuseLight);
//    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
}




