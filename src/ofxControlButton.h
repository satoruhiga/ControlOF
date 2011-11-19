#pragma once

#include "ofxControlWidget.h"

class ofxControlButton : public ofxControlWidget
{
private:
	
	bool toggle;
	bool *value;
	
public:
	
	ofEvent<bool> valueChanged;
	ofEvent<ofEventArgs> pressed;
	
	ofxControlButton(string label, int x, int y, int w = 80, int h = 20)
		: ofxControlWidget(label, x, y, w, h), value(NULL), toggle(false) {}
	
	~ofxControlButton()
	{
		value = NULL;
	}
	
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
		
		ofRectangle r = ofxControlGetStringBoundingBox(label, 0, 0);
		float c = (h() - r.height) * 0.5;
		
		setTextColor();
		ofxControlDrawBitmapString(label, x() + 4, y() + c);
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