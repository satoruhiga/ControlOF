#pragma once

#include "ofxControlWidget.h"

template <typename T>
class ofxControlRangeSlider : public ofxControlWidget
{
	T *minValue, *maxValue;
	T min, max;
	
public:
	
	ofEvent< pair<T, T> > valueChanged;
	
	ofxControlRangeSlider(string label, T min_, T max_, 
						  int x, int y, int width = 180, int height = 14)
		: ofxControlWidget(label, x, y, width, height)
	{
		minValue = NULL;
		maxValue = NULL;
		min = std::min(min_, max_);
		max = std::max(max_, min_);
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
		
		T minVal = min;
		T maxVal = max;
		
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
			
			ofRectangle r = ofxControlGetStringBoundingBox(getDisplayLabel(), 0, 0);
			float c = (h() - r.height) * 0.5;
			ofxControlDrawBitmapString(getDisplayLabel(), x() + w() + 4, y() + c);
			
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
			ofxControlDrawBitmapString(getDisplayLabel(), x(), y() + h() + 4);
			
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
		
		pair<T, T> temp(*minValue, *maxValue);
		
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
		
		if (temp != pair<T, T>(*minValue, *maxValue))
		{
			temp = pair<T, T>(*minValue, *maxValue);
			ofNotifyEvent(valueChanged, temp);
		}
	}
	
	void mouseDragged(int x_, int y_, int button)
	{
		if (!enable || !minValue) return;
		
		pair<T, T> temp(*minValue, *maxValue);
		
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
		
		if (temp != pair<T, T>(*minValue, *maxValue))
		{
			temp = pair<T, T>(*minValue, *maxValue);
			ofNotifyEvent(valueChanged, temp);
		}
	}
	
	ofxControlRangeSlider<T>* bind(T *minValue_, T *maxValue_)
	{
		minValue = minValue_; 
		maxValue = maxValue_; 
		return this;
	}
	
	ofxControlRangeSlider<T>* unbind() { minValue = NULL; maxValue = NULL; return this; }

	int getWidth()
	{
		ofRectangle r = ofxControlGetStringBoundingBox(getDisplayLabel(), 0, 0);
		
		if (w() > h())
		{
			return w() + r.width + 4;
		}
		else
		{
			return std::max(w(), (int)r.width);
		}
	}
	
	int getHeight()
	{
		ofRectangle r = ofxControlGetStringBoundingBox(getDisplayLabel(), 0, 0);
		
		if (h() >= w())
		{
			return h() + r.height + 4;
		}
		else
		{
			return std::max(h(), (int)r.height);
		}
	}
	
};

typedef ofxControlRangeSlider<float> ofxControlRangeSliderF;
typedef ofxControlRangeSlider<int> ofxControlRangeSliderI;
