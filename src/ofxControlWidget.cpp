#include "ofxControlWidget.h"

#include "ofxControl.h"

ofColor ofxControlWidget::defaultTextColor(200),
	ofxControlWidget::defaultForegroundColor(0, 105, 140),
	ofxControlWidget::defaultBackgroundColor(0, 54, 82, 200);

ofxControlWidget::ofxControlWidget(string label)
	: hover(false), down(false), enable(true)
{
	setLabel(label);
	
	textColor = defaultTextColor;
	foregroundColor = defaultForegroundColor;
	backgroundColor = defaultBackgroundColor;
	
	ofxControl::getCurrentControl().registerWidget(this);
}

ofxControlWidget::ofxControlWidget(string label_, int x_, int y_, int w_, int h_)
	: hover(false), down(false), enable(true)
{
	setLabel(label_);
	
	setPosition(x_, y_, 0);
	setSize(w_, h_);
	
	textColor = defaultTextColor;
	foregroundColor = defaultForegroundColor;
	backgroundColor = defaultBackgroundColor;
	
	ofxControl::getCurrentControl().registerWidget(this);
}

ofxControlWidget::~ofxControlWidget()
{
	ofxControl::getCurrentControl().unregisterWidget(this);
}

void ofxControlWidget::setLabel(const string &label_) 
{
	label = ofToUpper(label_);
	displayLabel.clear();
	for (int i = 0; i < label.size(); i++) { displayLabel += toupper(label[i]); }
}

void ofxControlWidget::hittest()
{
	ofRect(0, 0, getWidth(), getHeight());
}

void ofxControlWidget::makeCurrentWidget()
{
	ofxControl::getCurrentControl().currentWidget = this;
}

bool ofxControlWidget::isResponder()
{
	return ofxControl::getCurrentControl().getCurrentResponderWidget() == this;
}

void ofxControlWidget::addLinebreak(int extra_margine)
{
	ofxControl::getCurrentControl().addLinebreak(extra_margine);
}

void ofxControlWidget::setOffset(const ofVec3f& pos)
{
	setPosition(pos);
}

ofVec3f ofxControlWidget::getOffset()
{
	return getPosition();
}
