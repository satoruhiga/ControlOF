#pragma once

#include "ofxControlWidget.h"

class ofxControlSlider : public ofxControlWidget
{
	float *value;
	float min, max;
	
public:
	
	ofEvent<float> valueChanged;
	
	ofxControlSlider(string label, float min_, float max_,
					 int x, int y, int width = 180, int height = 14)
		: ofxControlWidget(label, x, y, width, height)
	{
		value = NULL;
		min = min_;
		max = max_;
	}
	
	void draw()
	{
		ofPushStyle();
		ofFill();

		setBackgroundColor();
		ofRect(x(), y(), w(), h());
		
		setForegroundColor();
		
		float val = min;
		
		if (value)
		{
			val = *value;
		}
		
		float v = ofMap(val, min, max, 0, 1, true);
		
		if (w() > h())
		{
			int xx = w() * v;
			ofRect(x(), y(), xx, h());
			
			setTextColor();

			ofRectangle r = ofxControlGetStringBoundingBox(label, 0, 0);
			float c = (h() - r.height) * 0.5;
			
			ofxControlDrawBitmapString(label, x() + w() + 4, y() + c);
			
			if (value)
				ofxControlDrawBitmapString(ofToString(val, 2), x() + 4, y() + c);
			else
				ofxControlDrawBitmapString("NAN", x() + 4, y() + 2);
		}
		else
		{
			int yy = h() * -v;
			ofRect(x(), y() + h(), w(), yy);

			setTextColor();
			ofxControlDrawBitmapString(label, x(), y() + h() + 4);
			
			if (value)
				ofxControlDrawBitmapString(ofToString(val, 2), x() + w() + 4, y() + h() + yy - 8);
			else
				ofxControlDrawBitmapString("NAN", x() + w() + 4, y() + h() + yy - 8);
		}
		
		ofPopStyle();
	}
	
	void mousePressed(int x_, int y_, int button)
	{
		if (!enable || !value) return;
		
		float temp = *value;
		
		if (w() > h())
		{
			*value = ofMap(x_, 0, w(), min, max, true);
		}
		else
		{
			*value = ofMap(y_, h(), 0, min, max, true);
		}
		
		if (temp != *value)
		{
			ofNotifyEvent(valueChanged, *value);
		}
	}
	
	void mouseDragged(int x_, int y_, int button)
	{
		if (!enable || !value) return;
		
		float temp = *value;
		
		if (w() > h())
		{
			*value = ofMap(x_, 0, w(), min, max, true);
		}
		else
		{
			*value = ofMap(y_, h(), 0, min, max, true);
		}
		
		if (temp != *value)
		{
			ofNotifyEvent(valueChanged, *value);
		}
	}
	
	ofxControlSlider* bind(float *value_) { value = value_; return this; }
	ofxControlSlider* unbind() { value = NULL; return this; }

};