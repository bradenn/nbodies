/* *
 * @project nbodies
 * @file graphics.h
 *
 * @author Braden Nicholson
 * @date 3/8/21, 5/10/22
 */

#ifndef GRAPHICS_H
#define GRAPHICS_H
#define SDL_MAIN_HANDLED

#include <SDL.h>
#include <OpenGL/glu.h>

#include <iostream>

using std::string;

struct Camera {
    float x{}, y{}, z{};
    float pitch = -180, roll = 0, yaw = 0;
    float scale = 1;
    float radius = 0;
};

class Graphics {
private:
    int vw, vh;
    SDL_Window *window{};
    SDL_Renderer *renderer{};
    SDL_GLContext glContext{};
    GLUquadric *quad{};
    Camera *camera{};
    bool state{};
public:
    Graphics(int w, int h);

    ~Graphics();

    void clear();

    void render();


    int drawChar(char c, float size, float x, float y);

    void fillRect(float x, float y, float w, float h);

    void setColor(int r, int g, int b);

    void setAlphaColor(int r, int g, int b, int a);

    void drawCube(float x, float y, float z, float w, float h, float d);

    void drawOrigin(float x, float y, float z, float w, float h, float d);

    Camera getCamera() {
        return *camera;
    }

    void rotate(float y, float p, float r);

    void scaleCamera(float s);

    void drawMeter(float x, float y, float w, float h, float p);

    void setRadius(float r);

    void drawString(string s, float size, float x, float y);

    void drawString3D(string s, float size, float x, float y, float z);

    void centerFull(float s);

    void drawCircle(float x, float y, float r);

    float drawStringGetLength(string s, float size, float x, float y);

    void strokeRect(float x, float y, float w, float h);

    void drawSphere(float x, float y, float z, float r);

    void beginSimulationFrame();

    void endSimulationFrame();

    void materialColor(float r, float g, float b);

    void materialEmit(float r, float g, float b, float f);

    void drawString3DFace(string s, float size, float x, float y, float z);

    void drawLine(float x1, float y1, float z1, float x2, float y2, float z2);
};


#endif //BARNESHUT_GRAPHICS_H
