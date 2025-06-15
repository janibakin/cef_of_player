#pragma once
#include "ofMain.h"

class ofApp : public ofBaseApp {
public:
    void setup() override;
    void update() override;
    void draw() override;

private:
    glm::vec2 position;
    glm::vec2 velocity;
    float radius = 20.0f;
};