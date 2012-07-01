#pragma once

#include "ofMain.h"

#include "ofxControlBitmapString.h"
#include "ofxControlWidget.h"

#include "ofxControlButton.h"
#include "ofxControlSlider.h"
#include "ofxControlRangeSlider.h"
#include "ofxControlNumberBox.h"
#include "ofxControlTextField.h"

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
	
	void setVisible(bool yn);
	bool isVisible() { return visible; }
	
	void setToggleKey(int key);
	
public: // autolayout
	
	void begin(int x = 0, int y = 0);
	void end();

	void setOffset(int x, int y);
	void linebreak(int extra_margine = 0);
	
	ofxControlWidget* getControl(string label);
	
	ofxControlButton* addButton(string label, int width = 80, int height = 18);
	
	ofxControlSliderF* addSliderF(string label, float min, float max, int width = 180, int height = 14);
	ofxControlSliderI* addSliderI(string label, int min, int max, int width = 180, int height = 14);
	
	ofxControlRangeSliderF* addRangeSliderF(string label, float min, float max, int width = 180, int height = 14);
	ofxControlRangeSliderI* addRangeSliderI(string label, int min, int max, int width = 180, int height = 14);
	
	ofxControlNumberBoxF* addNumberBoxF(string label, int width = 80, int height = 14);
	ofxControlNumberBoxI* addNumberBoxI(string label, int width = 80, int height = 14);
	
	ofxControlTextField* addTextField(string label, int width = 180, int height = 20);
	
public: // events
	
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
	map<string, ofxControlWidget*> widget_map;
	
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
	
private:
	
	int autoLayoutCurrentOffsetX, autoLayoutCurrentOffsetY;
	int autoLayoutStartX, autoLayoutStartY;
	int autoLayoutMarginX, autoLayoutMarginY;
	
	vector<ofxControlWidget*> currentLineWidgets;
	
	void applyAutoLayout(ofxControlWidget *w);
};