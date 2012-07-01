#include "ofxControl.h"

#include "ofxControlWidget.h"

ofxControl* ofxControl::currentControl = NULL;
int ofxControl::currentWidgetID = 1;

ofxControl::ofxControl()
{
	toggleKey = 0;
	visible = true;
	currentWidget = NULL;
	responderWidget = NULL;
	
	autoLayoutStartX = 0;
	autoLayoutStartY = 0;
	
	autoLayoutMarginX = 20;
	autoLayoutMarginY = 10;
}

ofxControl::~ofxControl()
{
	try
	{
		disableAllEvents();
	}
	catch (...)
	{
	}
	
	map<GLuint, ofxControlWidget*> t = widgets;
	
	map<GLuint, ofxControlWidget*>::iterator it = t.begin();
	while (it != t.end())
	{
		ofxControlWidget *w = (*it).second;
		delete w;
		it++;
	}
	
	widgets.clear();
}

void ofxControl::setup(bool autodraw)
{
	if (autodraw)
		enableAllEvents();
	else
		enableBaseicEvents();
}

void ofxControl::update()
{
	if (!visible) return;
	
	makeCurrentControl();
	
	map<GLuint, ofxControlWidget*>::iterator it = widgets.begin();
	while (it != widgets.end())
	{
		ofxControlWidget *w = (*it).second;
		w->update();
		it++;
	}
}

void ofxControl::draw()
{
	if (!visible) return;
	
	makeCurrentControl();
	
	ofPushView();
	
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetIntegerv(GL_VIEWPORT, viewport);
	
	glDisable(GL_DEPTH_TEST);
	
	ofPushStyle();
	ofEnableAlphaBlending();
	ofEnableSmoothing();
	
	map<GLuint, ofxControlWidget*>::iterator it = widgets.begin();
	while (it != widgets.end())
	{
		ofxControlWidget *w = (*it).second;
		w->draw();
		it++;
	}
	
	ofPopStyle();
	
	ofPopView();
}

void ofxControl::hittest()
{
	if (!visible) return;
	
	makeCurrentControl();
	
	ofPushStyle();
	ofFill();
	
	glEnable(GL_DEPTH_TEST);
	
	map<GLuint, ofxControlWidget*>::iterator it = widgets.begin();
	while (it != widgets.end())
	{
		ofxControlWidget *w = (*it).second;
		glPushName(w->getID());
		w->hittest();
		glPopName();
		it++;
	}
	
	ofPopStyle();
}

void ofxControl::setVisible(bool yn)
{
	visible = yn;
}

void ofxControl::setToggleKey(int key)
{
	toggleKey = key;
}

#pragma mark WIDGET MANAGEMENT

void ofxControl::registerWidget(ofxControlWidget *w)
{
	w->widgetID = currentWidgetID++;
	widgets[w->widgetID] = w;
	widget_map[w->getLabel()] = w;
}

void ofxControl::unregisterWidget(ofxControlWidget *w)
{
	widgets.erase(w->widgetID);
}

ofxControlWidget* ofxControl::getControl(string label)
{
	label = ofToUpper(label);
	return widget_map[label];
}

#pragma mark EVENTS

void ofxControl::enableBaseicEvents()
{
	ofAddListener(ofEvents().update, this, &ofxControl::onUpdate);
	
	ofAddListener(ofEvents().mousePressed, this, &ofxControl::onMousePressed);
	ofAddListener(ofEvents().mouseReleased, this, &ofxControl::onMouseReleased);
	ofAddListener(ofEvents().mouseMoved, this, &ofxControl::onMouseMoved);
	ofAddListener(ofEvents().mouseDragged, this, &ofxControl::onMouseDragged);
	
	ofAddListener(ofEvents().keyPressed, this, &ofxControl::onKeyPressed);
	ofAddListener(ofEvents().keyReleased, this, &ofxControl::onKeyReleased);
}

void ofxControl::enableAllEvents()
{
	ofAddListener(ofEvents().update, this, &ofxControl::onUpdate);
	ofAddListener(ofEvents().draw, this, &ofxControl::onDraw);
	
	ofAddListener(ofEvents().mousePressed, this, &ofxControl::onMousePressed);
	ofAddListener(ofEvents().mouseReleased, this, &ofxControl::onMouseReleased);
	ofAddListener(ofEvents().mouseMoved, this, &ofxControl::onMouseMoved);
	ofAddListener(ofEvents().mouseDragged, this, &ofxControl::onMouseDragged);
	
	ofAddListener(ofEvents().keyPressed, this, &ofxControl::onKeyPressed);
	ofAddListener(ofEvents().keyReleased, this, &ofxControl::onKeyReleased);
}

void ofxControl::disableAllEvents()
{
	ofRemoveListener(ofEvents().update, this, &ofxControl::onUpdate);
	ofRemoveListener(ofEvents().draw, this, &ofxControl::onDraw);
	
	ofRemoveListener(ofEvents().mousePressed, this, &ofxControl::onMousePressed);
	ofRemoveListener(ofEvents().mouseReleased, this, &ofxControl::onMouseReleased);
	ofRemoveListener(ofEvents().mouseMoved, this, &ofxControl::onMouseMoved);
	ofRemoveListener(ofEvents().mouseDragged, this, &ofxControl::onMouseDragged);
	
	ofRemoveListener(ofEvents().keyPressed, this, &ofxControl::onKeyPressed);
	ofRemoveListener(ofEvents().keyReleased, this, &ofxControl::onKeyReleased);
}

void ofxControl::onUpdate(ofEventArgs&)
{
	if (!visible) return;
	
	makeCurrentControl();
	
	update();
}

void ofxControl::onDraw(ofEventArgs&)
{
	if (!visible) return;
	
	makeCurrentControl();
	
	draw();
}

void ofxControl::onKeyPressed(ofKeyEventArgs &e)
{
	if (toggleKey != 0 && e.key == toggleKey)
	{
		setVisible(!isVisible());
	}
	
	if (!visible) return;
	
	makeCurrentControl();
	
	if (responderWidget)
	{
		responderWidget->keyPressed(e.key);
	}
}

void ofxControl::onKeyReleased(ofKeyEventArgs &e)
{
	if (!visible) return;
	
	makeCurrentControl();
	
	if (responderWidget)
	{
		responderWidget->keyReleased(e.key);
	}
}

void ofxControl::onMousePressed(ofMouseEventArgs &e)
{
	if (!visible) return;
	
	makeCurrentControl();
	
	map<GLuint, ofxControlWidget*>::iterator it = widgets.begin();
	while (it != widgets.end())
	{
		ofxControlWidget *w = (*it).second;
		w->hover = false;
		it++;
	}
	
	ofVec2f m(e.x, e.y);
	
	vector<Selection> p = pickup(e.x, e.y);
	
	if (!p.empty())
	{
		Selection &s = p[0];
		
		currentWidgetDepth = s.min_depth;
		
		for (int i = 0; i < s.name_stack.size(); i++)
		{
			ofxControlWidget *w = widgets[s.name_stack.at(i)];
			ofVec2f localPos = getLocalPosition(m.x, m.y);
			ofVec2f p = localPos - w->getWorldPos();
			
			w->mousePressed(p.x, p.y, e.button);
			
			w->hover = true;
			w->down = true;
			
			w->makeCurrentWidget();
			
			responderWidget = w;
		}
	}
	
	if (p.empty()) responderWidget = NULL;
}

void ofxControl::onMouseReleased(ofMouseEventArgs &e)
{
	if (!visible) return;
	
	makeCurrentControl();
	
	map<GLuint, ofxControlWidget*>::iterator it = widgets.begin();
	while (it != widgets.end())
	{
		ofxControlWidget *w = (*it).second;
		w->hover = false;
		it++;
	}
	
	ofVec2f m(e.x, e.y);
	
	vector<Selection> p = pickup(e.x, e.y);
	
	if (!p.empty())
	{
		Selection &s = p[0];
		
		for (int i = 0; i < s.name_stack.size(); i++)
		{
			ofxControlWidget *w = widgets[s.name_stack.at(i)];
			ofVec2f localPos = getLocalPosition(m.x, m.y);
			ofVec2f p = localPos - w->getWorldPos();
			
			w->mouseReleased(p.x, p.y, e.button);
			w->hover = true;
		}
	}
	
	if (currentWidget)
	{
		currentWidget->down = false;
		currentWidget = NULL;
	}
}

void ofxControl::onMouseMoved(ofMouseEventArgs &e)
{
	if (!visible) return;
	
	makeCurrentControl();
	
	map<GLuint, ofxControlWidget*>::iterator it = widgets.begin();
	while (it != widgets.end())
	{
		ofxControlWidget *w = (*it).second;
		w->hover = false;
		it++;
	}
	
	ofVec2f m(e.x, e.y);
	
	vector<Selection> p = pickup(e.x, e.y);
	
	if (!p.empty())
	{
		Selection &s = p[0];
		
		for (int i = 0; i < s.name_stack.size(); i++)
		{
			ofxControlWidget *w = widgets[s.name_stack.at(i)];
			ofVec2f localPos = getLocalPosition(m.x, m.y);
			ofVec2f p = localPos - w->getWorldPos();
			
			w->mouseMoved(p.x, p.y);
			w->hover = true;
		}
	}
}

void ofxControl::onMouseDragged(ofMouseEventArgs &e)
{
	if (!visible) return;
	
	makeCurrentControl();
	
	map<GLuint, ofxControlWidget*>::iterator it = widgets.begin();
	while (it != widgets.end())
	{
		ofxControlWidget *w = (*it).second;
		w->hover = false;
		it++;
	}
	
	bool forcus_is_out = true;
	
	ofVec2f m(e.x, e.y);
	
	if (!currentWidget)
	{
		vector<Selection> p = pickup(e.x, e.y);
		
		if (!p.empty())
		{
			Selection &s = p[0];
			
			for (int i = 0; i < s.name_stack.size(); i++)
			{
				ofxControlWidget *w = widgets[s.name_stack.at(i)];
				ofVec2f localPos = getLocalPosition(m.x, m.y);
				ofVec2f p = localPos - w->getWorldPos();
				
				w->mouseDragged(p.x, p.y, e.button);
				w->hover = true;
				
				if (w == currentWidget) forcus_is_out = false;
			}
		}
	}
	
	if (forcus_is_out && currentWidget)
	{
		ofVec2f localPos = getLocalPosition(m.x, m.y);
		ofVec2f p = localPos - currentWidget->getWorldPos();
		
		currentWidget->mouseDragged(p.x, p.y, e.button);
		currentWidget->hover = true;
	}
}


#pragma mark PICKUP

bool ofxControl::sort_by_depth(const ofxControl::Selection &a, const ofxControl::Selection &b)
{
	return a.min_depth < b.min_depth;
}

vector<ofxControl::Selection> ofxControl::pickup(int x, int y)
{
	const int BUFSIZE = 256;
	GLuint selectBuf[BUFSIZE];
	GLint hits;
	
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	ofPushView();
	
	glEnable(GL_DEPTH_TEST);
	
	glSelectBuffer(BUFSIZE, selectBuf);
	glRenderMode(GL_SELECT);
	glMatrixMode(GL_PROJECTION);
	
	glPushMatrix();
	{
		glLoadIdentity();
		gluPickMatrix(x, viewport[3] - y, 1.0, 1.0, viewport);
		glMultMatrixd(projection);
		
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glMultMatrixd(modelview);
		
		hittest();
		
		glMatrixMode(GL_PROJECTION);
	}
	glPopMatrix();
	
	glMatrixMode(GL_MODELVIEW);
	
	hits = glRenderMode(GL_RENDER);
	
	ofPopView();
	glPopAttrib();
	
	if (hits <= 0) return vector<Selection>();
	
	GLuint *ptr = selectBuf;
	
	vector<Selection> picked_stack;
	
	for (int i = 0; i < hits; i++)
	{
		GLuint num_names = ptr[0];
		GLuint min_depth = ptr[1];
		GLuint max_depth = ptr[2];
		
		GLuint *names = &ptr[3];
		
		Selection d;
		
		d.min_depth = min_depth;
		d.max_depth = max_depth;
		d.name_stack.insert(d.name_stack.begin(), names, (names + num_names));
		
		picked_stack.push_back(d);
		
		ptr += (3 + num_names);
	}
	
	sort(picked_stack.begin(), picked_stack.end(), sort_by_depth);
	
	return picked_stack;
}

ofVec2f ofxControl::getLocalPosition(int x, int y)
{
	GLdouble ox, oy, oz;
	
	gluUnProject(x, y, (float)currentWidgetDepth / (float)0xffffffff,
				 modelview, projection, viewport,
				 &ox, &oy, &oz);
	
	oy = viewport[3] - oy;
	
	return ofVec2f(ox, oy);
}


#pragma SHORTCUTS

ofxControlButton* ofxControl::addButton(string label, int width, int height)
{
	ofxControlButton *o = new ofxControlButton(label, 0, 0, width, height);
	applyAutoLayout(o);
	return o;
}

ofxControlSliderF* ofxControl::addSliderF(string label, float min, float max, int width, int height) 
{
	ofxControlSliderF *o = new ofxControlSliderF(label, min, max, 0, 0, width, height);
	applyAutoLayout(o);
	return o;
}

ofxControlSliderI* ofxControl::addSliderI(string label, int min, int max, int width, int height)
{
	ofxControlSliderI *o = new ofxControlSliderI(label, min, max, 0, 0, width, height);
	applyAutoLayout(o);
	return o;
}

ofxControlRangeSliderF* ofxControl::addRangeSliderF(string label, float min, float max, int width, int height)
{
	ofxControlRangeSliderF *o = new ofxControlRangeSliderF(label, min, max, 0, 0, width, height);
	applyAutoLayout(o);
	return o;
}

ofxControlRangeSliderI* ofxControl::addRangeSliderI(string label, int min, int max, int width, int height)
{
	ofxControlRangeSliderI *o = new ofxControlRangeSliderI(label, min, max, 0, 0, width, height);
	applyAutoLayout(o);
	return o;
}

ofxControlNumberBoxF* ofxControl::addNumberBoxF(string label, int width, int height)
{
	ofxControlNumberBoxF *o = new ofxControlNumberBoxF(label, 0, 0, width, height);
	applyAutoLayout(o);
	return o;
}

ofxControlNumberBoxI* ofxControl::addNumberBoxI(string label, int width, int height)
{
	ofxControlNumberBoxI *o = new ofxControlNumberBoxI(label, 0, 0, width, height);
	applyAutoLayout(o);
	return o;
}

ofxControlTextField* ofxControl::addTextField(string label, int width, int height)
{
	ofxControlTextField *o = new ofxControlTextField(label, 0, 0, width, height);
	applyAutoLayout(o);
	return o;
}

#pragma mark AUTOLAYOUT

void ofxControl::begin(int x_, int y_)
{
	makeCurrentControl();
	
	setOffset(x_, y_);
}

void ofxControl::end()
{
	currentControl = NULL;
	currentLineWidgets.clear();
}

void ofxControl::linebreak(int extra_margine)
{
	int lineheight = 0;
	
	for (int i = 0; i < currentLineWidgets.size(); i++)
	{
		lineheight = std::max(currentLineWidgets[i]->getHeight(), lineheight);
	}
	
	autoLayoutCurrentOffsetY += lineheight + autoLayoutMarginY + extra_margine;
	autoLayoutCurrentOffsetX = autoLayoutStartX;
	
	currentLineWidgets.clear();
}

void ofxControl::setOffset(int x, int y)
{
	currentLineWidgets.clear();
	
	autoLayoutStartX = autoLayoutCurrentOffsetX = x;
	autoLayoutStartY = autoLayoutCurrentOffsetY = y;
}

void ofxControl::applyAutoLayout(ofxControlWidget *w)
{
	w->rect.x = autoLayoutCurrentOffsetX;
	w->rect.y = autoLayoutCurrentOffsetY;
	
	autoLayoutCurrentOffsetX += w->getWidth() + autoLayoutMarginX;
	
	currentLineWidgets.push_back(w);
}
