#pragma once

#include "ofMain.h"

#include "ofxControl.h"

class ofxControl;

class ofxControlWidget
{
	friend class ofxControl;
	
public:
	
	static ofColor defaultTextColor, defaultForegroundColor, defaultBackgroundColor;
	
	ofxControlWidget(string label, int x, int y, int w, int h);
	virtual ~ofxControlWidget();

	inline int getID() { return widgetID; }
	
	inline void setEnable(bool yn) { enable = yn; }
	inline bool isEnable() { return enable; }
	
	virtual void update() {}
	virtual void draw() {}
	virtual void hittest();

	virtual void mousePressed(int x, int y, int button) {}
	virtual void mouseReleased(int x, int y, int button) {}
	virtual void mouseMoved(int x, int y) {}
	virtual void mouseDragged(int x, int y, int button) {}
	virtual void keyPressed(int key) {}
	virtual void keyReleased(int key) {}
	
	ofxControlWidget* getParent() const { return parent; };
	ofVec2f getWorldPos();
	
	void makeCurrentWidget();
	
protected:
	
	bool enable;
	ofRectangle rect;
	string label;
	ofColor textColor, foregroundColor, backgroundColor;
	
	bool isHover() { return hover; }
	bool isDown() { return down; }
	
	int x() const { return rect.x; }
	int y() const { return rect.y; }
	int w() const { return rect.width; }
	int h() const { return rect.height; }
	
	inline void setTextColor() { ofSetColor(textColor); }
	inline void setForegroundColor()
	{
		ofColor c = foregroundColor;
		
		if (enable)
		{
			if (isHover()) c.setBrightness(c.getBrightness() + 20);
			if (isDown()) c.setBrightness(c.getBrightness() + 20);
		}
		else
			c.setBrightness(c.getBrightness() - 20);
		
		ofSetColor(c);
	}
	inline void setBackgroundColor() { ofSetColor(backgroundColor); }

private:
	int widgetID;
	bool hover, down;
	ofxControlWidget *parent;
	
};