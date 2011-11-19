#pragma once

#include "ofMain.h"

#include "ofxControlBitmapString.h"
#include "ofxControlWidget.h"

class ofxControlWidget;

class ofxControl
{
	friend class ofxControlWidget;
	
	static ofxControl *currentControl;
	static int currentWidgetID;
	ofxControlWidget *currentWidget;
	ofxControlWidget *responderWidget;
	
public:

	static ofxControl& getCurrentControl() { return *currentControl; }
	void makeCurrentControl() { currentControl = this; }
	
	ofxControlWidget* getCurrentResponderWidget() { return responderWidget; }
	
	ofxControl();
	~ofxControl();

	void setup(bool autodraw = true);
	
	void update();
	void draw();
	void hittest();
	
	void begin();
	void end();
	
	void setVisible(bool yn);
	bool isVisible() { return visible; }
	
	void setToggleKey(int key);
	
	// events
	
	void enableAllEvents();
	void enableBaseicEvents();
	void disableAllEvents();

	void onUpdate(ofEventArgs&);
	void onDraw(ofEventArgs&);
	void onKeyPressed(ofKeyEventArgs &e);
	void onKeyReleased(ofKeyEventArgs &e);
	void onMousePressed(ofMouseEventArgs &e);
	void onMouseReleased(ofMouseEventArgs &e);
	void onMouseMoved(ofMouseEventArgs &e);
	void onMouseDragged(ofMouseEventArgs &e);
	
protected:
	
	map<GLuint, ofxControlWidget*> widgets;
	
	bool visible;
	int toggleKey;
	
	void registerWidget(ofxControlWidget *w);
	void unregisterWidget(ofxControlWidget *w);
	
private:
	
	GLint viewport[4];
	GLdouble projection[16], modelview[16];
	GLuint currentWidgetDepth;
	
	struct Selection
	{
		GLuint min_depth, max_depth;
		vector<GLuint> name_stack;
	};
	
	static bool sort_by_depth(const ofxControl::Selection &a, const ofxControl::Selection &b);
	vector<Selection> pickup(int x, int y);

	ofVec2f getLocalPosition(int x, int y);
	
};