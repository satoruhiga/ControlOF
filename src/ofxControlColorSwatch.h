#pragma once

#include "ofxControlWidget.h"

class ofxControlColorSwatch : public ofxControlWidget
{
public:
	
	ofEvent<unsigned int> valueChanged;
	
	ofxControlColorSwatch(const vector<unsigned int>& colors, int x, int y, int w = 180) : ofxControlWidget("", x, y, w, 0), chipSize(10), chipMargin(2)
	{
		setColors(colors);
	}
	
	~ofxControlColorSwatch()
	{
		
	}
	
	unsigned int getValue() { return *value; }
	void setValue(unsigned int idx) { *value = idx; }
	
	ofxControlColorSwatch* bind(unsigned int *value_) { value = value_; return this; }
	ofxControlColorSwatch* unbind() { value = NULL; return this; }

	void draw()
	{
		glPushMatrix();
		
		glTranslatef(x(), y(), 0);
		
		ofPushStyle();
		ofFill();
		
		setBackgroundColor();
		
		ofRect(0, 0, w(), h());
		
		
		ofPopStyle();
		
		glPopMatrix();
	}
	
	void update()
	{
		
	}
	
	void mousePressed(int x, int y, int button)
	{
		printf("%i %i\n", x, y);
	}
	
	void setColors(const vector<unsigned int>& colors)
	{
		rect.height = 100;
	}
	
protected:	
	
	vector<unsigned int> colors;
	
	unsigned int default_value;
	unsigned int *value;
	
	float chipSize, chipMargin;
	
};