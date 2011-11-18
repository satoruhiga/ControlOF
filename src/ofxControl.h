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
	static ofxControlWidget *currentWidget;
	static ofxControlWidget *responderWidget;
	
public:

	static ofxControl& getCurrentControl() { return *currentControl; }
	void makeCurrentControl() { currentControl = this; }
	
	static ofxControlWidget* getCurrentResponderWidget() { return responderWidget; }
	
	ofxControl();

	void setup();
	void update();
	void draw();
	void hittest();
	
	void begin();
	void end();
	
	void setVisible(bool yn);
	bool isVisible() { return visible; }
	
	void setToggleKey(int key);
	
	// events
	
	void enableEvents();	
	void disableEvents();

	void onUpdate(ofEventArgs&);
	void onDraw(ofEventArgs&);
	void onKeyPressed(ofKeyEventArgs &e);
	void onKeyReleased(ofKeyEventArgs &e);
	void onMousePressed(ofMouseEventArgs &e);
	void onMouseReleased(ofMouseEventArgs &e);
	void onMouseMoved(ofMouseEventArgs &e);
	void onMouseDragged(ofMouseEventArgs &e);
	
protected:
	
	ofMatrix4x4 projection, modelview;
	map<GLuint, ofxControlWidget*> widgets;
	
	bool visible;
	int toggleKey;
	
	struct Selection
	{
		GLuint min_depth, max_depth;
		vector<GLuint> name_stack;
		void print();
	};
	
	static bool sort_by_depth(const ofxControl::Selection &a, const ofxControl::Selection &b);
	vector<GLuint> pickup(int x, int y);
	
	void registerWidget(ofxControlWidget *w);
	void unregisterWidget(ofxControlWidget *w);
	
};