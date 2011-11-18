#include "ofxControlWidget.h"

ofColor ofxControlWidget::defaultTextColor(200),
	ofxControlWidget::defaultForegroundColor(0, 105, 140),
	ofxControlWidget::defaultBackgroundColor(0, 54, 82, 200);

ofxControlWidget::ofxControlWidget(string label_, int x_, int y_, int w_, int h_)
	: label(label_), parent(NULL), hover(false), down(false), enable(true)
{
	ofxControl::getCurrentControl().registerWidget(this);
	rect.set(x_, y_ ,w_, h_);
	
	textColor = defaultTextColor;
	foregroundColor = defaultForegroundColor;
	backgroundColor = defaultBackgroundColor;
}

ofxControlWidget::~ofxControlWidget()
{
	ofxControl::getCurrentControl().unregisterWidget(this);
}

void ofxControlWidget::hittest()
{
	ofRect(rect.x, rect.y, rect.width, rect.height);
}

ofVec2f ofxControlWidget::getWorldPos()
{
	ofVec2f result(x(), y());
	if (getParent()) result += getParent()->getWorldPos();
	return result;
}

void ofxControlWidget::makeCurrentWidget()
{
	ofxControl::getCurrentControl().currentWidget = this;
}