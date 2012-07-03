#pragma once

#include "ofxControlWidget.h"

class ofxControlSeparator : public ofxControlWidget
{
public:
	
	ofxControlSeparator(string label = "", int extra_margine = 0) : ofxControlWidget(label, 0, 0, 0, 0), extra_margine(extra_margine)
	{}
	
	int getExtraMargine() { return extra_margine; }
	
protected:
	
	int extra_margine;
	
};