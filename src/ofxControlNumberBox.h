#pragma once

#include "ofxControlWidget.h"

template <typename T>
class ofxControlNumberBox : public ofxControlWidget
{
	T default_value;
	
	T *value;
	T min, max;
	bool clamp;
	float temp;
	
public:
	
	ofEvent<T> valueChanged;
	
	ofxControlNumberBox(string label, int x, int y, int w = 80, int h = 14)
		: ofxControlWidget(label, x, y, w, h), default_value(0), value(&default_value), clamp(false) {}
	
	T getValue() { return *value; }
	void setValue(T v) { *value = v; }
	
	void draw()
	{
		ofPushStyle();
		ofFill();
		
		setBackgroundColor();
		ofRect(x(), y(), w(), h());
		
		T val = 0;
		
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
		// ofxControlDrawBitmapString(getDisplayLabel(), x(), y() + h() + 4);
		float c = (h() - r.height) * 0.5;
		ofxControlDrawBitmapString(getDisplayLabel(), x() + w() + 4, y() + c);
		ofPopStyle();
	}

	ofxControlNumberBox* bind(T *value_) { value = value_; return this; }
	ofxControlNumberBox* unbind() { value = NULL; return this; }
	
	ofxControlNumberBox* setMinMax(T min_, T max_)
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
	
	int getWidth()
	{
		ofRectangle r = ofxControlGetStringBoundingBox(getDisplayLabel(), 0, 0);
		return w() + r.width + 4;
	}
};

typedef ofxControlNumberBox<float> ofxControlNumberBoxF;
typedef ofxControlNumberBox<int> ofxControlNumberBoxI;
