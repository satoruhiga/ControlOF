#include "ofxControlWidget.h"

#include "ofxControl.h"

ofColor ofxControlWidget::defaultTextColor(200),
	ofxControlWidget::defaultForegroundColor(0, 105, 140),
	ofxControlWidget::defaultBackgroundColor(0, 54, 82, 200);

ofxControlWidget::ofxControlWidget(string label_, int x_, int y_, int w_, int h_)
	: parent(NULL), hover(false), down(false), enable(true)
{
	ofxControl::getCurrentControl().registerWidget(this);
	
	setLabel(label_);
	
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

bool ofxControlWidget::isResponder()
{
	return ofxControl::getCurrentControl().getCurrentResponderWidget() == this;
}

void ofxControlWidget::linebreak(int extra_margine)
{
	ofxControl::getCurrentControl().linebreak(extra_margine);
}