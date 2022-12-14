/* *
 * @project nbodies
 * @file Body.h
 * 
 * @abstract  
 *
 * @author Braden Nicholson
 * @date 3/4/21, 4/22/22-5/17/22
 */

#ifndef NBODIES_BODY_H
#define NBODIES_BODY_H

#include <iostream>
#include <cmath>
#include "graphics.h"

/*
 * Basic Structure inspired by this:
 * ftp://ftp.cs.princeton.edu/pub/cs126/barnes-hut/Body.java
 */

#define G 6.67430E-11f
#define DAMPENING 10E2f

class Body {
private:
    // Position
    float px = 0, py = 0, pz = 0;
    // Velocity
    float vx = 0, vy = 0, vz = 0;
    // Force
    float fx = 0, fy = 0, fz = 0;

    // Metadata
    float mass = 0, radius = 0;
    uint32_t lastCollision = 0;

public:

    void update(float dt) {
        vx += dt * fx / mass;
        vy += dt * fy / mass;
        vz += dt * fz / mass;
        px += dt * vx;
        py += dt * vy;
        pz += dt * vz;
    }

    static float fmap(float value,
                      float istart,
                      float istop,
                      float ostart,
                      float ostop) {
        if (value < istart) return 0;
        if (value > istop) return istop / 2;
        return ostart + (ostop - ostart) * ((value - istart) / (istop - istart));
    }


    // How many milliseconds should a body remain glowing after being struck
    uint32_t maxDelta = 1000 * 8;

    void draw(Graphics *bh, float maxMass) {

        if (lastCollision > 0) {
            uint32_t delta = SDL_GetTicks() - lastCollision;
            if (delta > maxDelta) {
                lastCollision = 0;
            } else if (delta < 1000 * 2) {
                float red = fmap((float) delta, 0, (float) maxDelta, 1, 0.5);
                float green = fmap((float) delta, 0, (float) maxDelta, 0.6, 0.2);
                bh->materialEmit(red, green, 0.2, 0);
                bh->materialColor(red, green, 0.2);
            } else {
                float red = fmap((float) delta, 0, (float) maxDelta, 1, 0.4);
                float green = fmap((float) delta, 0, (float) maxDelta, 0.6, 0.2);
                float brightness = fmap((float) delta, 0, (float) 1000 * 2, 1, 0);
                bh->materialEmit(red, green, 0.2, brightness);
                bh->materialColor(red, green, 0.2);
            }
        } else {
            bh->materialColor(0.4, 0.4, 0.4);
            bh->materialEmit(0, 0, 0, 0);
        }


        bh->drawSphere(px, py, pz, radius);

        radius = fmap(mass, 0, maxMass, 4, 16);

        bh->materialEmit(0, 0, 0, 0);
    }

    float getMass() {
        return mass;
    }

    float distance(Body b) {
        float dx = px - b.px;
        float dy = py - b.py;
        float dz = pz - b.pz;
        return sqrt(dx * dx + dy * dy + dz * dz);
    }

    void print() {
        std::cout << px << " " << py << " " << pz << " " << vx << " " << vy << " " << vz << " " << mass << std::endl;
    }

    void clearForce() {
        fx = 0.0;
        fy = 0.0;
        fz = 0.0;
    }

    void addForce(Body b) {

        float dx = b.px - px;
        float dy = b.py - py;
        float dz = b.pz - pz;
        float ds = sqrt(dx * dx + dy * dy + dz * dz);

        float df = (G * mass * b.mass) / (ds * ds + DAMPENING);
        fx += df * (dx / ds);
        fy += df * (dy / ds);
        fz += df * (dz / ds);

    }

    float avgForce() {

        return (fx + fy + fz) / (float) 3.0;
    }


    bool handleCollision(Body body) {
        // Equation for elastic collision from PHYS 204A
        // ((m1 – m2)u1 + 2m2u2) / (m1 + m2)

        // Check for a collision
        if (distance(body) <= radius + body.radius) {

            float m1 = mass;

            float m2 = body.mass;
            // vx += dt * fx / mass
            float dx = body.px - px;
            float dy = body.py - py;
            float dz = body.pz - pz;
            float ds = sqrt(dx * dx + dy * dy + dz * dz);

            float ux1 = vx;
            float ux2 = body.vx;
            float vdx = ((m1 - m2) * ux1 + 2 * m2 * ux2) / (m1 + m2);

            fx = (dx / ds) * vdx;

            float uy1 = vy;
            float uy2 = body.vy;
            float vdy = ((m1 - m2) * uy1 + 2 * m2 * uy2) / (m1 + m2);

            fy = (dy / ds) * vdy;

            float uz1 = vz;
            float uz2 = body.vz;
            float vdz = ((m1 - m2) * uz1 + 2 * m2 * uz2) / (m1 + m2);

            fz = (dz / ds) * vdz;

            lastCollision = SDL_GetTicks();
        }
        return false;
    }

    Body plus(Body b) const {
        float ms, nx, ny, nz;
        ms = mass + b.mass;
        nx = (px * mass + b.px * b.mass) / ms;
        ny = (py * mass + b.py * b.mass) / ms;
        nz = (pz * mass + b.pz * b.mass) / ms;

        return {nx, ny, nz, 0, 0, 0, 0, 0, 0, ms};
    }


    Body() = default;

    Body(float px, float py, float pz, float vx, float vy, float vz, float fx, float fy, float fz, float mass)
            : px(px), py(py), pz(pz), vx(vx), vy(vy), vz(vz), fx(fx), fy(fy), fz(fz), mass(mass) {
        lastCollision = 0;
    }

};


#endif //NBODIES_BODY_H
