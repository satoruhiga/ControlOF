#include "ofxControl.h"

#include "ofxControlWidget.h"

ofxControl* ofxControl::currentControl = NULL;
int ofxControl::currentWidgetID = 1;
ofxControlWidget* ofxControl::currentWidget = NULL;
ofxControlWidget* ofxControl::responderWidget = NULL;

ofxControl::ofxControl()
{
	toggleKey = 0;
	visible = true;
}

void ofxControl::setup()
{
	enableEvents();
}

void ofxControl::update()
{
	if (!visible) return;
	
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
	
	ofPushView();
	
	glGetFloatv(GL_PROJECTION_MATRIX, projection.getPtr());
	glGetFloatv(GL_MODELVIEW_MATRIX, modelview.getPtr());
	
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

void ofxControl::begin()
{
	makeCurrentControl();
}

void ofxControl::end()
{
	currentControl = NULL;
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
}

void ofxControl::unregisterWidget(ofxControlWidget *w)
{
	widgets.erase(w->widgetID);
}

#pragma mark EVENTS

void ofxControl::enableEvents()
{
	ofAddListener(ofEvents.update, this, &ofxControl::onUpdate);
	ofAddListener(ofEvents.draw, this, &ofxControl::onDraw);
	
	ofAddListener(ofEvents.mousePressed, this, &ofxControl::onMousePressed);
	ofAddListener(ofEvents.mouseReleased, this, &ofxControl::onMouseReleased);
	ofAddListener(ofEvents.mouseMoved, this, &ofxControl::onMouseMoved);
	ofAddListener(ofEvents.mouseDragged, this, &ofxControl::onMouseDragged);
	
	ofAddListener(ofEvents.keyPressed, this, &ofxControl::onKeyPressed);
	ofAddListener(ofEvents.keyReleased, this, &ofxControl::onKeyReleased);
}

void ofxControl::disableEvents()
{
	ofRemoveListener(ofEvents.update, this, &ofxControl::onUpdate);
	ofRemoveListener(ofEvents.draw, this, &ofxControl::onDraw);
	
	ofRemoveListener(ofEvents.mousePressed, this, &ofxControl::onMousePressed);
	ofRemoveListener(ofEvents.mouseReleased, this, &ofxControl::onMouseReleased);
	ofRemoveListener(ofEvents.mouseMoved, this, &ofxControl::onMouseMoved);
	ofRemoveListener(ofEvents.mouseDragged, this, &ofxControl::onMouseDragged);
	
	ofRemoveListener(ofEvents.keyPressed, this, &ofxControl::onKeyPressed);
	ofRemoveListener(ofEvents.keyReleased, this, &ofxControl::onKeyReleased);
}

void ofxControl::onUpdate(ofEventArgs&)
{
	if (!visible) return;
	
	update();
}

void ofxControl::onDraw(ofEventArgs&)
{
	if (!visible) return;
	
	draw();
}

void ofxControl::onKeyPressed(ofKeyEventArgs &e)
{
	if (toggleKey != 0 && e.key == toggleKey)
	{
		setVisible(!isVisible());
	}
	
	if (!visible) return;
	
	if (responderWidget)
	{
		responderWidget->keyPressed(e.key);
	}
}

void ofxControl::onKeyReleased(ofKeyEventArgs &e)
{
	if (!visible) return;
	
	if (responderWidget)
	{
		responderWidget->keyReleased(e.key);
	}
}

void ofxControl::onMousePressed(ofMouseEventArgs &e)
{
	if (!visible) return;
	
	map<GLuint, ofxControlWidget*>::iterator it = widgets.begin();
	while (it != widgets.end())
	{
		ofxControlWidget *w = (*it).second;
		w->hover = false;
		it++;
	}
	
	ofVec2f m(e.x, e.y);
	
	vector<GLuint> p = pickup(e.x, e.y);
	for (int i = 0; i < p.size(); i++)
	{
		ofxControlWidget *w = widgets[p.at(i)];
		ofVec2f p = m - w->getWorldPos();
		w->mousePressed(p.x, p.y, e.button);
		
		w->hover = true;
		w->down = true;
		
		w->makeCurrentWidget();
		
		responderWidget = w;
	}
	
	if (p.empty()) responderWidget = NULL;
}

void ofxControl::onMouseReleased(ofMouseEventArgs &e)
{
	if (!visible) return;
	
	map<GLuint, ofxControlWidget*>::iterator it = widgets.begin();
	while (it != widgets.end())
	{
		ofxControlWidget *w = (*it).second;
		w->hover = false;
		it++;
	}
	
	ofVec2f m(e.x, e.y);
	
	vector<GLuint> p = pickup(e.x, e.y);
	for (int i = 0; i < p.size(); i++)
	{
		ofxControlWidget *w = widgets[p.at(i)];
		ofVec2f p = m - w->getWorldPos();
		w->mouseReleased(p.x, p.y, e.button);
		w->hover = true;
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
	
	map<GLuint, ofxControlWidget*>::iterator it = widgets.begin();
	while (it != widgets.end())
	{
		ofxControlWidget *w = (*it).second;
		w->hover = false;
		it++;
	}
	
	ofVec2f m(e.x, e.y);
	
	vector<GLuint> p = pickup(e.x, e.y);
	for (int i = 0; i < p.size(); i++)
	{
		ofxControlWidget *w = widgets[p.at(i)];
		ofVec2f p = m - w->getWorldPos();
		w->mouseMoved(p.x, p.y);
		w->hover = true;
	}
}

void ofxControl::onMouseDragged(ofMouseEventArgs &e)
{
	if (!visible) return;
	
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
		vector<GLuint> p = pickup(e.x, e.y);
		for (int i = 0; i < p.size(); i++)
		{
			ofxControlWidget *w = widgets[p.at(i)];
			ofVec2f p = m - w->getWorldPos();
			w->mouseDragged(p.x, p.y, e.button);
			w->hover = true;
			
			if (w == currentWidget) forcus_is_out = false;
		}
	}
	
	if (forcus_is_out && currentWidget)
	{
		ofVec2f p = m - currentWidget->getWorldPos();
		currentWidget->mouseDragged(p.x, p.y, e.button);
		currentWidget->hover = true;
	}
}


#pragma mark PICKUP

bool ofxControl::sort_by_depth(const ofxControl::Selection &a, const ofxControl::Selection &b)
{
	return a.min_depth < b.min_depth;
}

vector<GLuint> ofxControl::pickup(int x, int y)
{
	const int BUFSIZE = 256;
	GLuint selectBuf[BUFSIZE];
	GLint hits;
	
	ofPushView();
	
	glEnable(GL_DEPTH_TEST);
	
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	
	glSelectBuffer(BUFSIZE, selectBuf);
	glRenderMode(GL_SELECT);
	glMatrixMode(GL_PROJECTION);
	
	glPushMatrix();
	{
		glLoadIdentity();
		gluPickMatrix(x, viewport[3] - y, 1.0, 1.0, viewport);
		glMultMatrixf(projection.getPtr());
		
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glMultMatrixf(modelview.getPtr());
		
		hittest();
		
		glMatrixMode(GL_PROJECTION);
	}
	glPopMatrix();
	
	glMatrixMode(GL_MODELVIEW);
	
	hits = glRenderMode(GL_RENDER);
	
	ofPopView();
	
	if (hits <= 0) return vector<GLuint>();
	
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
		
		// d.print();
		
		ptr += (3 + num_names);
	}
	
	sort(picked_stack.begin(), picked_stack.end(), sort_by_depth);
	
	return picked_stack[0].name_stack;
}

void ofxControl::Selection::print()
{
	printf("num:%i => ", name_stack.size());
	for (int i = 0; i < name_stack.size(); i++)
	{
		printf("%i ", name_stack[i]);
	}
	printf("\n");
}
