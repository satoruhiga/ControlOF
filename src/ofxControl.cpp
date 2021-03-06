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
		
		if (!w->getParent())
		{
			w->transformGL();
			w->draw();
			w->restoreTransformGL();
		}
		
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
		
		if (!w->getParent())
		{
			
			w->transformGL();
			glPushName(w->getID());
			w->hittest();
			glPopName();
			w->restoreTransformGL();
		}
		
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
		currentWidgetDepth = (float)s.min_depth / 0xffffffff;
		
		for (int i = 0; i < s.name_stack.size(); i++)
		{
			ofxControlWidget *w = widgets[s.name_stack.at(i)];
			ofVec3f p = getLocalPosition(m.x, m.y);
			p = w->getGlobalTransformMatrix().getInverse().preMult(p);

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
		currentWidgetDepth = (float)s.min_depth / 0xffffffff;
		
		for (int i = 0; i < s.name_stack.size(); i++)
		{
			ofxControlWidget *w = widgets[s.name_stack.at(i)];
			ofVec3f p = getLocalPosition(m.x, m.y);
			p = w->getGlobalTransformMatrix().getInverse().preMult(p);
			
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
		currentWidgetDepth = (float)s.min_depth / 0xffffffff;
		
		for (int i = 0; i < s.name_stack.size(); i++)
		{
			ofxControlWidget *w = widgets[s.name_stack.at(i)];
			ofVec3f p = getLocalPosition(m.x, m.y);
			p = w->getGlobalTransformMatrix().getInverse().preMult(p);
			
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
			currentWidgetDepth = (float)s.min_depth / 0xffffffff;
			
			for (int i = 0; i < s.name_stack.size(); i++)
			{
				ofxControlWidget *w = widgets[s.name_stack.at(i)];
				ofVec3f p = getLocalPosition(m.x, m.y);
				p = w->getGlobalTransformMatrix().getInverse().preMult(p);
				
				w->mouseDragged(p.x, p.y, e.button);
				w->hover = true;
				
				if (w == currentWidget) forcus_is_out = false;
			}
		}
	}
	
	if (forcus_is_out && currentWidget)
	{
		ofVec3f p = getLocalPosition(m.x, m.y);
		p = currentWidget->getGlobalTransformMatrix().getInverse().preMult(p);

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

ofVec3f ofxControl::getLocalPosition(int x, int y)
{
	GLdouble ox, oy, oz;
	
	gluUnProject(x, y, currentWidgetDepth,
				 modelview, projection, viewport,
				 &ox, &oy, &oz);
	
	oy = viewport[3] - oy;
	
	return ofVec2f(ox, oy);
}


#pragma SHORTCUTS

ofxControlButton* ofxControl::addButton(string label, int width, int height)
{
	ofxControlButton *o = new ofxControlButton(label, 0, 0, width, height);
	if (currentGroup) currentGroup->add(o);
	return o;
}

ofxControlSliderF* ofxControl::addSliderF(string label, float min, float max, int width, int height) 
{
	ofxControlSliderF *o = new ofxControlSliderF(label, min, max, 0, 0, width, height);
	if (currentGroup) currentGroup->add(o);
	return o;
}

ofxControlSliderI* ofxControl::addSliderI(string label, int min, int max, int width, int height)
{
	ofxControlSliderI *o = new ofxControlSliderI(label, min, max, 0, 0, width, height);
	if (currentGroup) currentGroup->add(o);
	return o;
}

ofxControlRangeSliderF* ofxControl::addRangeSliderF(string label, float min, float max, int width, int height)
{
	ofxControlRangeSliderF *o = new ofxControlRangeSliderF(label, min, max, 0, 0, width, height);
	if (currentGroup) currentGroup->add(o);
	return o;
}

ofxControlRangeSliderI* ofxControl::addRangeSliderI(string label, int min, int max, int width, int height)
{
	ofxControlRangeSliderI *o = new ofxControlRangeSliderI(label, min, max, 0, 0, width, height);
	if (currentGroup) currentGroup->add(o);
	return o;
}

ofxControlNumberBoxF* ofxControl::addNumberBoxF(string label, int width, int height)
{
	ofxControlNumberBoxF *o = new ofxControlNumberBoxF(label, 0, 0, width, height);
	if (currentGroup) currentGroup->add(o);
	return o;
}

ofxControlNumberBoxI* ofxControl::addNumberBoxI(string label, int width, int height)
{
	ofxControlNumberBoxI *o = new ofxControlNumberBoxI(label, 0, 0, width, height);
	if (currentGroup) currentGroup->add(o);
	return o;
}

ofxControlTextField* ofxControl::addTextField(string label, int width, int height)
{
	ofxControlTextField *o = new ofxControlTextField(label, 0, 0, width, height);
	if (currentGroup) currentGroup->add(o);
	return o;
}

#pragma mark AUTOLAYOUT

ofxControlGroup* ofxControl::begin(int x, int y)
{
	makeCurrentControl();
	
	currentGroup = new ofxControlGroup;
	currentGroup->setPosition(x, y, 0);
	groupStack.push(currentGroup);
	
	return currentGroup;
}

void ofxControl::end()
{
	groupStack.pop();
	
	if (!groupStack.empty())
	{
		currentGroup = groupStack.top();
	}
	else
	{
		currentGroup = NULL;
		currentControl = NULL;
	}
}

void ofxControl::addLinebreak(int extra_margine)
{
	if (currentGroup) currentGroup->addLinebreak(extra_margine);
}

void ofxControl::addSeparator(int extra_margine)
{
	if (currentGroup) currentGroup->addSeparator(extra_margine);
}
