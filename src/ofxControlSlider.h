#pragma once

#include "ofxControlWidget.h"

template <typename T>
class ofxControlSlider : public ofxControlWidget
{
	T default_value;
	
	T *value;
	T min, max;
	
public:
	
	ofEvent<T> valueChanged;

	ofxControlSlider(string label, T min_, T max_,
					 int x, int y, int width = 180, int height = 14)
		: ofxControlWidget(label, x, y, width, height), default_value(min_), value(&default_value), min(min_), max(max_) {}
	
	T getValue() { return *value; }
	void setValue(T v) { *value = v; }

	void draw()
	{
		ofPushStyle();
		ofFill();

		setBackgroundColor();
		ofRect(x(), y(), w(), h());
		
		setForegroundColor();
		
		T val = min;
		
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

			ofRectangle r = ofxControlGetStringBoundingBox(getDisplayLabel(), 0, 0);
			float c = (h() - r.height) * 0.5;
			
			ofxControlDrawBitmapString(getDisplayLabel(), x() + w() + 4, y() + c);
			
			if (value)
				ofxControlDrawBitmapString(ofToString(val, 2), x() + 4, y() + c);
			else
				ofxControlDrawBitmapString("NULL", x() + 4, y() + 2);
		}
		else
		{
			int yy = h() * -v;
			ofRect(x(), y() + h(), w(), yy);

			setTextColor();
			ofxControlDrawBitmapString(getDisplayLabel(), x(), y() + h() + 4);
			
			if (value)
				ofxControlDrawBitmapString(ofToString(val, 2), x() + w() + 4, y() + h() + yy - 8);
			else
				ofxControlDrawBitmapString("NULL", x() + w() + 4, y() + h() + yy - 8);
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
	
	ofxControlSlider<T>* bind(T *value_) { value = value_; return this; }
	ofxControlSlider<T>* unbind() { value = NULL; return this; }

	int getWidth()
	{
		ofRectangle r = ofxControlGetStringBoundingBox(getDisplayLabel(), 0, 0);
		
		if (w() > h())
		{
			return w() + r.width + 4;
		}
		else
		{
			return std::max(w(), (int)r.width);
		}
	}
	
	int getHeight()
	{
		ofRectangle r = ofxControlGetStringBoundingBox(getDisplayLabel(), 0, 0);
		
		if (h() >= w())
		{
			return h() + r.height + 4;
		}
		else
		{
			return std::max(h(), (int)r.height);
		}
	}
};

typedef ofxControlSlider<float> ofxControlSliderF;
typedef ofxControlSlider<int> ofxControlSliderI;

