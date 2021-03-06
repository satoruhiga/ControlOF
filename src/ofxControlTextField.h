#pragma once

#include "ofxControlWidget.h"

class ofxControlTextField : public ofxControlWidget
{
protected:
	
	string default_value;
	
	string *value;
	
	float min, max;
	int caretPos;
	bool editable;
	
public:
	
	ofEvent<string> valueChanged;
	ofEvent<string> enterPressed;
	
	ofxControlTextField(string label, int x, int y, int w = 180, int h = 20)
		: ofxControlWidget(label, x, y, w, h)
	{
		value = &default_value;
		editable = true;
		caretPos = 0;
	}
	
	string getValue() { return *value; }
	void setValue(const string& v) { *value = v; }
	
	void draw()
	{
		ofPushStyle();

		setBackgroundColor();
		ofFill();
		ofRect(0, 0, w(), h());
		
		setTextColor();
		if (value)
		{
			ofRectangle r = ofxControlGetStringBoundingBox(*value, 0, 0);
			float c = (h() - r.height) * 0.5;

			ofxControlDrawBitmapString(*value, 4, c);
			
			if (editable)
			{
				if (isResponder())
				{
					caretPos = ofClamp(caretPos, 0, (*value).size());
					
					string ss = value->substr(0, caretPos);
					ofRectangle r = ofxControlGetStringBoundingBox(ss, 4, 6);
					
					int xx = r.x + r.width;
					int yy = 14;
					ofLine(xx, yy, xx + 4, yy);
				}
			}
		}
		else
		{
			ofxControlDrawBitmapString("NONE", 4, 6);
		}
		
		setForegroundColor();
		ofNoFill();
		ofRect(0, 0, w(), h());
		
		setTextColor();
		ofxControlDrawBitmapString(getDisplayLabel(), 0, h() + 4);

		ofPopStyle();
	}
	
	void keyPressed(int key)
	{
		if (!value || !editable) return;
		
		string temp = *value;
		
		if (key == OF_KEY_BACKSPACE)
		{
			if (!value->empty())
			{
				value->erase(value->begin() + caretPos - 1);
				caretPos = ofClamp(caretPos - 1, 0, (*value).size());
			}
		}
		else if (key == OF_KEY_LEFT)
		{
			caretPos--;
			caretPos = ofClamp(caretPos, 0, (*value).size());
		}
		else if (key == OF_KEY_RIGHT)
		{
			caretPos++;
			caretPos = ofClamp(caretPos, 0, (*value).size());
		}
		else if (isgraph(key) || key == ' ')
		{
			value->insert(value->begin() + caretPos, key);
			caretPos++;
			caretPos = ofClamp(caretPos, 0, (*value).size());
		}
		
		if (temp != *value) ofNotifyEvent(valueChanged, *value, this);
		
		static ofEventArgs args;
		if (key == OF_KEY_RETURN) ofNotifyEvent(enterPressed, *value, this);
	}
	
	ofxControlTextField* bind(string *value_) { value = value_; return this; }
	ofxControlTextField* unbind() { value = NULL; return this; }
	
	ofxControlTextField* setEditable(bool yn) { editable = yn; return this; }
	
	int getHeight()
	{
		ofRectangle r = ofxControlGetStringBoundingBox(getDisplayLabel(), 0, 0);
		return h() + r.height + 4;
	}
	
};
