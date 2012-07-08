#pragma once

#include "ofMain.h"

class ofxControl;

class ofxControlWidget : public ofNode
{
	friend class ofxControl;
	
public:
	
	static ofColor defaultTextColor, defaultForegroundColor, defaultBackgroundColor;
	
	ofxControlWidget(string label = "");
	ofxControlWidget(string label, int x, int y, int w, int h);
	virtual ~ofxControlWidget();

	inline int getID() { return widgetID; }
	
	inline const string& getLabel() const { return label; }
	inline const string& getDisplayLabel() const { return displayLabel; }
	
	void setLabel(const string &label_);
	
	void setOffset(const ofVec3f& pos);
	ofVec3f getOffset();
	
	virtual int getWidth() { return w(); }
	virtual int getHeight() { return h(); }
	
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
	
	void setParent(ofxControlWidget *o) { parent = o; }
	ofxControlWidget* getParent() const { return (ofxControlWidget*)parent; };
	
	void makeCurrentWidget();
	
	void addLinebreak(int extra_margine = 0);
	
protected:
	
	bool enable;
	ofVec2f size;
	string label, displayLabel;
	ofColor textColor, foregroundColor, backgroundColor;
	
	inline int x() const { return getX(); }
	inline int y() const { return getY(); }
	inline int w() const { return size.x; }
	inline int h() const { return size.y; }
	
	inline void setWidth(int v) { size.x = v; }
	inline void setHeight(int v) { size.y = v; }
	inline void setSize(int w, int h) { size.set(w, h); }
	
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
	
};