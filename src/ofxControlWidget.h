#pragma once

#include "ofMain.h"

class ofxControl;

class ofxControlWidget
{
	friend class ofxControl;
	
public:
	
	static ofColor defaultTextColor, defaultForegroundColor, defaultBackgroundColor;
	
	ofxControlWidget(string label, int x, int y, int w, int h);
	virtual ~ofxControlWidget();

	inline int getID() { return widgetID; }
	
	inline const string& getLabel() const { return label; }
	inline const string& getDisplayLabel() const { return displayLabel; }
	
	void setLabel(const string &label_) 
	{
		label = label_; 
		displayLabel.clear();
		for (int i = 0; i < label.size(); i++) { displayLabel += toupper(label[i]); }
	}
	
	virtual int getWidth() { return rect.width; }
	virtual int getHeight() { return rect.height; }
	
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
	
	void linebreak(int extra_margine = 0);
	
protected:
	
	bool enable;
	ofRectangle rect;
	string label, displayLabel;
	ofColor textColor, foregroundColor, backgroundColor;
	
	int x() const { return rect.x; }
	int y() const { return rect.y; }
	int w() const { return rect.width; }
	int h() const { return rect.height; }
	
	bool isHover() { return hover; }
	bool isDown() { return down; }
	
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

	bool isResponder();
	
private:
	int widgetID;
	bool hover, down;
	ofxControlWidget *parent;
	
};