#pragma once

#include "ofxControlWidget.h"

class ofxControlNumberBox : public ofxControlWidget
{
private:
	
	float *value;
	float min, max;
	bool clamp;
	float temp;
	
public:
	
	ofEvent<float> valueChanged;
	
	ofxControlNumberBox(string label, int x, int y, int w = 80, int h = 14)
		: ofxControlWidget(label, x, y, w, h), value(NULL), clamp(false) {}
	
	~ofxControlNumberBox()
	{
		value = NULL;
	}
	
	void draw()
	{
		ofPushStyle();
		ofFill();
		
		setBackgroundColor();
		ofRect(x(), y(), w(), h());
		
		float val = 0;
		
		if (value)
			val = *value;
		
		setTextColor();
		
		ofRectangle r = ofxControlGetStringBoundingBox(ofToString(val, 2), 0, 0);
		float yy = (h() - r.height) * 0.5;
		
		ofxControlDrawBitmapString(ofToString(val, 2), x() + 12, y() + yy);
		
		setForegroundColor();
		glPushMatrix();
		glTranslatef(x(), y() + (h() - 14) * 0.5, 0);
		glBegin(GL_TRIANGLES);
		glVertex2f(0, 0);
		glVertex2f(6, 14/2);
		glVertex2f(0, 14);
		glEnd();
		glPopMatrix();
		
		setTextColor();
		ofxControlDrawBitmapString(label, x(), y() + h() + 4);
		ofPopStyle();
	}

	ofxControlNumberBox* bind(float *value_) { value = value_; return this; }
	ofxControlNumberBox* unbind() { value = NULL; return this; }
	
	ofxControlNumberBox* setMinMax(float min_, float max_)
	{
		min = min_;
		max = max_;
		clamp = true;
		return this;
	}

	void mousePressed(int x_, int y_, int)
	{
		temp = y_;
	}
	
	void mouseDragged(int x_, int y_, int)
	{
		if (!value) return;
		
		*value += temp - y_;
		
		if (clamp)
			*value = ofClamp(*value, min, max);
		
		ofNotifyEvent(valueChanged, *value);
		
		temp = y_;
	}
};
