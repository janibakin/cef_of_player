#include "ofApp.h"

void ofApp::setup()
{
    ofSetWindowTitle("oF – bouncing ball stub");
    ofSetBackgroundColor(0);
    ofSetFrameRate(60);
}

void ofApp::update()
{
    x += v;
    if (x > ofGetWidth() - 30 || x < 30) v = -v;
}

void ofApp::draw()
{
    ofSetColor(255, 64, 64);
    ofDrawCircle(x, ofGetHeight() * 0.75, 30);
}