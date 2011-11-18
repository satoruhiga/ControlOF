#include "testApp.h"

#include "ControlOF.h"

ofxControl c;

float slider_value;

string backgroundColorString;
string framerateString;
string windowTitleString;

float minValue = 0, maxValue = 255;

//--------------------------------------------------------------
void testApp::setup()
{
	ofSetVerticalSync(true);
	ofSetFrameRate(60);
	
	ofBackground(40);
	c.setup();
	
	c.begin();
	
	(new ofxControlSlider("SLIDER 1", 0, 255, 10, 10, 100, 14))->bind(&slider_value);
	(new ofxControlSlider("SLIDER 2", 0, 255, 10, 40, 100, 100))->bind(&slider_value);
	
	ofAddListener((new ofxControlButton("RANDOM", 10, 160, 100))->pressed, this, &testApp::onButtonPressed);

	(new ofxControlTextField("BGCOLOR", 180, 10))->bind(&backgroundColorString)->setEditable(false);
	(new ofxControlTextField("FRAMERATE", 180, 60))->bind(&framerateString)->setEditable(false);
	
	(new ofxControlRangeSlider("RANGE", 0, 255, 450, 10, 180, 14))->bind(&minValue, &maxValue);
	(new ofxControlNumberBox("MIN", 450, 40))->bind(&minValue);
	(new ofxControlNumberBox("MAX", 550, 40))->bind(&maxValue);
	
	windowTitleString = "title";
	(new ofxControlTextField("SET WINDOW TITLE", 10, 300))->bind(&windowTitleString);
	
	c.end();
	
	// show/hide control with TAB key
	c.setToggleKey('\t');
}

void testApp::onButtonPressed(ofEventArgs&)
{
	slider_value = ofRandom(0, 255);
}

//--------------------------------------------------------------
void testApp::update()
{
	ofBackground(slider_value);
	
	backgroundColorString = "0x" + ofToHex(slider_value);
	framerateString = ofToString(ofGetFrameRate(), 2);
	
	ofSetWindowTitle(windowTitleString);
}

//--------------------------------------------------------------
void testApp::draw()
{
}

//--------------------------------------------------------------
void testApp::keyPressed(int key)
{

}

//--------------------------------------------------------------
void testApp::keyReleased(int key)
{

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y)
{

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button)
{

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button)
{
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button)
{

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h)
{

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg)
{

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo)
{

}