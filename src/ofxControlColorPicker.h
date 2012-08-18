#pragma once

#include "ofxControlWidget.h"

class ofxControlColorPicker : public ofxControlWidget
{
public:
	
	ofEvent<unsigned int> valueChanged;
	ofEvent<ofColor> colorChanged;
	
	ofxControlColorPicker(int x, int y, int w = 180, int h = 180) : ofxControlWidget("", x, y, w, h), margin(2), corsor_pos(margin, margin), default_value(0), value(&default_value)
	{
		updateColorPicker();
	}
	
	unsigned int getValue() { return *value; }
	void setValue(unsigned int v) { *value = v; color.setHex(v); updateCursor(); }
	
	ofColor getColor() { return color; }
	
	ofxControlColorPicker* bind(unsigned int *value_) { value = value_; return this; }
	ofxControlColorPicker* unbind() { value = NULL; return this; }
	
	void draw()
	{
		ofPushStyle();
		ofFill();

		setBackgroundColor();
		ofRect(0, 0, w(), h());
		
		ofSetColor(255);
		picker_image.draw(margin, margin);
		
		ofSetColor(color);
		ofCircle(corsor_pos, 4);
		
		ofSetColor(255);
		ofCircle(corsor_pos, 2);
		
		ofPopStyle();
	}
	
	void mousePressed(int x, int y, int button)
	{
		pick(x - margin, y - margin);
	}
	
	void mouseDragged(int x, int y, int button)
	{
		pick(x - margin, y - margin);
	}
	
protected:
	
	int margin;
	ofImage picker_image;
	
	unsigned int default_value;
	unsigned int *value;
	
	ofColor color;
	ofVec2f corsor_pos;
	
	void updateColorPicker()
	{
		picker_image.allocate(w() - margin * 2, h() - margin * 2, OF_IMAGE_COLOR);
		
		for (int y = 0; y < picker_image.getHeight(); y++)
		{
			for (int x = 0; x < picker_image.getWidth(); x++)
			{
				ofColor c = HSL2RGB(ofMap(x, 0, picker_image.getWidth(), 0, 1),
											 1,
											 ofMap(y, 0, picker_image.getHeight(), 1, 0));
				picker_image.setColor(x, y, c);
			}
		}
		
		picker_image.update();
	}
	
	void updateCursor()
	{
		double h, l, s;
		RGB2HSL(color, h, l, s);
		
		corsor_pos.set(h * picker_image.getWidth() + margin,
					   (1 - s) * picker_image.getHeight() + margin);
	}
	
	void pick(int x, int y)
	{
		x = ofClamp(x, 0, picker_image.getWidth());
		y = ofClamp(y, 0, picker_image.getHeight());
		
		color = picker_image.getColor(x, y);
		*value = color.getHex();
		
		updateCursor();
		
		ofNotifyEvent(valueChanged, *value);
		ofNotifyEvent(colorChanged, color);
	}

	
	// via http://www.geekymonkey.com/Programming/CSharp/RGB2HSL_HSL2RGB.htm
	inline ofColor HSL2RGB(double h, double sl, double l)
	{
		double v;
		double r,g,b;
		
		r = l;
		g = l;
		b = l;
		v = (l <= 0.5) ? (l * (1.0 + sl)) : (l + sl - l * sl);
		
		if (v > 0)
		{
			double m;
			double sv;
			int sextant;
			double fract, vsf, mid1, mid2;
			
			m = l + l - v;
			sv = (v - m ) / v;
			h *= 6.0;
			sextant = (int)h;
			fract = h - sextant;
			vsf = v * sv * fract;
			mid1 = m + vsf;
			mid2 = v - vsf;
			switch (sextant)
			{
				case 0:
					r = v;
					g = mid1;
					b = m;
					break;
				case 1:
					r = mid2;
					g = v;
					b = m;
					break;
				case 2:
					r = m;
					g = v;
					b = mid1;
					break;
				case 3:
					r = m;
					g = mid2;
					b = v;
					break;
				case 4:
					r = mid1;
					g = m;
					b = v;
					break;
				case 5:
					r = v;
					g = m;
					b = mid2;
					break;
			}
		}
		
		ofColor rgb;
		rgb.r = r * 255;
		rgb.g = g * 255;
		rgb.b = b * 255;
		return rgb;
	}
	
	inline void RGB2HSL(ofColor rgb, double& h, double& s, double& l)
	{
		double r = rgb.r/255.0;
		double g = rgb.g/255.0;
		double b = rgb.b/255.0;
		double v;
		double m;
		double vm;
		double r2, g2, b2;
		
		h = 0;
		s = 0;
		l = 0;
		v = max(r,g);
		v = max(v,b);
		m = min(r,g);
		m = min(m,b);
		
		l = (m + v) / 2.0;
		if (l <= 0.0)
		{
			return;
		}
		vm = v - m;
		s = vm;
		if (s > 0.0)
		{
			s /= (l <= 0.5) ? (v + m ) : (2.0 - v - m) ;
		}
		else
		{
			return;
		}
		r2 = (v - r) / vm;
		g2 = (v - g) / vm;
		b2 = (v - b) / vm;
		if (r == v)
		{
			h = (g == m ? 5.0 + b2 : 1.0 - g2);
		}
		else if (g == v)
		{
			h = (b == m ? 1.0 + r2 : 3.0 - b2);
		}
		else
		{
			h = (r == m ? 3.0 + g2 : 5.0 - r2);
		}
		h /= 6.0;
	}
};
