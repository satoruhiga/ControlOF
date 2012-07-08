#pragma once

#include "ofxControlWidget.h"

class ofxControlGroup : public ofxControlWidget
{
public:
	
	ofxControlGroup(string label = "");
	~ofxControlGroup();
	
	void add(ofxControlWidget *o);
	void remove(ofxControlWidget *o);
	
	enum LayoutDirecton {
		VERTICAL,
		HORIZONTAL
	};
	
	void setLayoutDirection(LayoutDirecton dir_) { dir = dir_; updateLayout(); }
	void addLinebreak(int extra_mergine = 0);
	void addSeparator(int extra_mergine = 0);
	
	void draw();
	void hittest();
	
protected:
	
	vector<ofxControlWidget*> widgets;
	LayoutDirecton dir;
	float margin;
	
	void updateLayout();
};
