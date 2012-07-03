#pragma once

#include "ofxControlWidget.h"

class ofxControlButton : public ofxControlWidget
{
protected:
	
	bool default_value;
	
	bool toggle;
	bool *value;
	
public:
	
	ofEvent<bool> valueChanged;
	ofEvent<ofEventArgs> pressed;
	
	ofxControlButton(string label, int x, int y, int w = 80, int h = 18)
		: ofxControlWidget(label, x, y, w, h), default_value(false), value(&default_value), toggle(false) {}
	
	~ofxControlButton()
	{
		value = NULL;
	}
	
	bool getValue() { return *value; }
	void setValue(bool v) { *value = v; }
	
	void draw()
	{
		ofPushStyle();
		ofFill();
		
		if (toggle)
		{
			if (*value)
				setForegroundColor();
			else
				setBackgroundColor();
		}
		else
		{
			if (isHover())
				setForegroundColor();
			else
				setBackgroundColor();
		}
		
		ofRect(x(), y(), w(), h());
		
		ofRectangle r = ofxControlGetStringBoundingBox(getDisplayLabel(), 0, 0);
		float c = (h() - r.height) * 0.5;
		
		setTextColor();
		ofxControlDrawBitmapString(getDisplayLabel(), x() + 4, y() + c);
		ofPopStyle();
	}

	ofxControlButton* bind(bool *value_) { value = value_; return this; }
	ofxControlButton* unbind() { value = NULL; return this; }

	ofxControlButton* setToggle(bool yn) { toggle = yn; return this; }
	
	void update()
	{
		if (!toggle && value)
		{
			*value = false;
		}
	}
	
	void mousePressed(int,int,int)
	{
		static ofEventArgs args;
		ofNotifyEvent(pressed, args);
		
		if (!value) return;
		
		if (toggle)
		{
			*value = !*value;
			ofNotifyEvent(valueChanged, *value);
		}
		else
		{
			*value = true;
		}
	}
};