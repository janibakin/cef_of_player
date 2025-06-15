#include "ofApp.h"

void ofApp::setup() {
    ofBackground(30);
    position = glm::vec2(ofGetWidth() / 2, ofGetHeight() / 2);
    velocity = glm::vec2(5.0f, 4.0f);
    ofSetWindowPosition(100, 100);
}

void ofApp::update()
{
    position += velocity;

    // Bounce off edges
    if (position.x - radius < 0 || position.x + radius > ofGetWidth()) {
        velocity.x *= -1;
    }
    if (position.y - radius < 0 || position.y + radius > ofGetHeight()) {
        velocity.y *= -1;
    }
}

void ofApp::draw()
{
    ofSetColor(255, 100, 100);
    ofDrawCircle(position, radius);
}