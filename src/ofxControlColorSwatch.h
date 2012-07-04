#pragma once

#include "ofxControlWidget.h"

class ofxControlColorSwatch : public ofxControlWidget
{
public:
	
	ofEvent<unsigned int> valueChanged;
	ofEvent<ofColor> colorChanged;
	
	ofxControlColorSwatch(const vector<unsigned int>& colors, int x, int y, int w = 180) : ofxControlWidget("", x, y, w, 0), chipSize(20), chipMargin(2), default_value(0), value(&default_value)
	{
		setColors(colors);
	}
	
	~ofxControlColorSwatch()
	{
	}
	
	unsigned int getValue() { return *value; }
	void setValue(unsigned int idx) { *value = idx; }
	
	ofColor getColor() { return color; }
	
	ofxControlColorSwatch* bind(unsigned int *value_) { value = value_; return this; }
	ofxControlColorSwatch* unbind() { value = NULL; return this; }

	void draw()
	{
		glPushMatrix();
		
		ofPushStyle();
		
		
		setBackgroundColor();
		
		ofFill();
		ofRect(0, 0, w(), h());
		
		for (int i = 0; i < colorChips.size(); i++)
		{
			ColorChip &c = colorChips[i];
			
			ofFill();
			ofSetColor(c.color);
			ofRect(c.rect);
			
			if (*value == i)
			{
				ofNoFill();
				ofSetColor(255);
				ofRect(c.rect);
			}
		}
		
		ofPopStyle();
		
		glPopMatrix();
	}
	
	void update()
	{
		
	}
	
	void mousePressed(int x, int y, int button)
	{
		ofVec2f mpos(x, y);
		const ofVec2f offset(chipMargin + 1, chipMargin + 1);
		
		mpos -= offset;
		
		float n = chipSize + chipMargin;
		
		int xx = mpos.x / n;
		int yy = mpos.y / n;
		
		int line = floor((w()) / n);
		int index = yy * line + xx;
		
		if (index < colors.size())
		{
			*value = index;
			color.setHex(colors[*value]);
			
			ofNotifyEvent(valueChanged, *value);
			ofNotifyEvent(colorChanged, color);
		}
	}
	
	void setColors(const vector<unsigned int>& colors)
	{
		this->colors = colors;
		
		float n = chipSize + chipMargin;
		ofVec2f offset(chipMargin + 1, chipMargin + 1);
		
		for (int i = 0; i < colors.size(); i++)
		{
			ColorChip c;
			c.color = ofColor::fromHex(colors[i]);
			c.rect = ofRectangle(offset.x, offset.y, chipSize, chipSize);
			
			colorChips.push_back(c);

			offset.x += n;
			if (i != (colors.size() - 1)
				&& offset.x + n >= w())
			{
				offset.y += n;
				offset.x = chipMargin + 1;
			}
		}
		
		setHeight(offset.y + n + 1);
	}
	
protected:
	
	vector<unsigned int> colors;
	
	unsigned int default_value;
	unsigned int *value;
	
	ofColor color;
	
	float chipSize, chipMargin;
	
	struct ColorChip
	{
		ofColor color;
		ofRectangle rect;
	};
	
	vector<ColorChip> colorChips;
};