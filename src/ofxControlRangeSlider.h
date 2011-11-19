#pragma once

#include "ofxControlWidget.h"

class ofxControlRangeSlider : public ofxControlWidget
{
	float *minValue, *maxValue;
	float min, max;
	
public:
	
	ofEvent< pair<float, float> > valueChanged;
	
	ofxControlRangeSlider(string label, float min_, float max_, 
						  int x, int y, int width = 180, int height = 14)
		: ofxControlWidget(label, x, y, width, height)
	{
		minValue = NULL;
		maxValue = NULL;
		min = min_;
		max = max_;
	}
	
	void update()
	{
		if (minValue)
		{
			if (*minValue > *maxValue) std::swap(*minValue, *maxValue);
			if (*minValue < min) *minValue = min;
			if (*maxValue > max) *maxValue = max;
		}
	}
	
	void draw()
	{
		ofPushStyle();
		ofFill();

		setBackgroundColor();
		ofRect(x(), y(), w(), h());
		
		setForegroundColor();
		
		float minVal = min;
		float maxVal = max;
		
		if (minValue)
		{
			minVal = *minValue;
			maxVal = *maxValue;
		}
		
		if (w() > h())
		{
			int xx1 = w() * ofMap(minVal, min, max, 0, 1, true);
			int xx2 = w() * ofMap(maxVal, min, max, 0, 1, true);
			ofRect(x() + xx1, y(), xx2 - xx1, h());
			
			setTextColor();
			
			ofRectangle r = ofxControlGetStringBoundingBox(label, 0, 0);
			float c = (h() - r.height) * 0.5;
			ofxControlDrawBitmapString(label, x() + w() + 4, y() + c);
			
			if (minValue)
			{
				ofRectangle r = ofxControlGetStringBoundingBox(ofToString(maxVal, 2), 0, 0);
				float xx = w() - r.width - 4;
				
				ofxControlDrawBitmapString(ofToString(minVal, 2), x() + 4, y() + c);
				ofxControlDrawBitmapString(ofToString(maxVal, 2), x() + xx, y() + c);
			}
			else
			{
				ofRectangle r1 = ofxControlGetStringBoundingBox("NAN", 0, 0);
				float yy = (h() - r1.height) * 0.5;

				ofxControlDrawBitmapString("NAN", x() + 4, y() + yy);
			}
		}
		else
		{
			int yy1 = h() * -ofMap(minVal, min, max, 0, 1, true);
			int yy2 = h() * -ofMap(maxVal, min, max, 0, 1, true);
			ofRect(x(), y() + h() + yy1, w(), yy2 - yy1);

			setTextColor();
			ofxControlDrawBitmapString(label, x(), y() + h() + 4);
			
			if (minValue)
			{
				ofxControlDrawBitmapString(ofToString(minVal, 2), x() + w() + 4, y() + h() + yy1 - 8);
				ofxControlDrawBitmapString(ofToString(maxVal, 2), x() + w() + 4, y() + h() + yy2 - 8);

			}
			else
			{
				ofxControlDrawBitmapString("NAN", x() + w() + 4, y() + h() + yy1 - 8);
				ofxControlDrawBitmapString("NAN", x() + w() + 4, y() + h() + yy2 - 8);
			}
		}
		
		ofPopStyle();
	}
	
	void mousePressed(int x_, int y_, int button)
	{
		if (!enable || !minValue) return;
		
		pair<float, float> temp(*minValue, *maxValue);
		
		if (w() > h())
		{
			float xx = ofMap(x_, 0, w(), min, max, true);
			
			if (fabs(xx - *minValue) < fabs(xx - *maxValue))
			{
				*minValue = xx;
			}
			else
			{
				*maxValue = xx;
			}
		}
		else
		{
			float yy = ofMap(y_, 0, h(), max, min, true);
			
			if (fabs(yy - *minValue) < fabs(yy - *maxValue))
			{
				*minValue = yy;
			}
			else
			{
				*maxValue = yy;
			}
		}
		
		if (temp != pair<float, float>(*minValue, *maxValue))
		{
			temp = pair<float, float>(*minValue, *maxValue);
			ofNotifyEvent(valueChanged, temp);
		}
	}
	
	void mouseDragged(int x_, int y_, int button)
	{
		if (!enable || !minValue) return;
		
		pair<float, float> temp(*minValue, *maxValue);
		
		if (w() > h())
		{
			float xx = ofMap(x_, 0, w(), min, max, true);
			
			if (fabs(xx - *minValue) < fabs(xx - *maxValue))
			{
				*minValue = xx;
			}
			else
			{
				*maxValue = xx;
			}
		}
		else
		{
			float yy = ofMap(y_, 0, h(), max, min, true);
			
			if (fabs(yy - *minValue) < fabs(yy - *maxValue))
			{
				*minValue = yy;
			}
			else
			{
				*maxValue = yy;
			}
		}
		
		if (temp != pair<float, float>(*minValue, *maxValue))
		{
			temp = pair<float, float>(*minValue, *maxValue);
			ofNotifyEvent(valueChanged, temp);
		}
	}
	
	ofxControlRangeSlider* bind(float *minValue_, float *maxValue_)
	{
		minValue = minValue_; 
		maxValue = maxValue_; 
		return this;
	}
	
	ofxControlRangeSlider* unbind() { minValue = NULL; maxValue = NULL; return this; }

};