#include "ofxControlGroup.h"

#include "ofxControlSeparator.h"

ofxControlGroup::ofxControlGroup(string label) : ofxControlWidget("group"), dir(VERTICAL), margin(2)
{
}

ofxControlGroup::~ofxControlGroup()
{
	
}

void ofxControlGroup::add(ofxControlWidget *o)
{
	if (o == NULL) return;
	
	o->setParent(this);
	
	widgets.push_back(o);
	updateLayout();
}

void ofxControlGroup::remove(ofxControlWidget *o)
{
	vector<ofxControlWidget*>::iterator it = find(widgets.begin(), widgets.end(), o);
	
	if (it == widgets.end()) return;
	
	o->setParent(NULL);
	
	widgets.erase(it);
	delete *it;
	
	updateLayout();
}

void ofxControlGroup::linebreak(int extra_mergine)
{
	add(new ofxControlSeparator("", extra_mergine));
}

//

void ofxControlGroup::updateLayout()
{
	ofVec2f layout_pos(0, 0);
	float max_size = 0;
	
	if (dir == VERTICAL)
	{
		for (int i = 0; i < widgets.size(); i++)
		{
			ofxControlWidget *w = widgets[i];
			w->setOffset(layout_pos);
			
			max_size = max(max_size, (float)w->getWidth());
			
			if (ofxControlSeparator *sep = dynamic_cast<ofxControlSeparator*>(w))
			{
				layout_pos.x += max_size + margin + sep->getExtraMargine();
				layout_pos.y = 0;
				
				max_size = 0;
			}
			else
			{
				layout_pos.y += w->getHeight() + margin;
			}
		}
	}
	else
	{
		// TODO
	}
}

//

void ofxControlGroup::draw()
{
	transformGL();
	
	for (int i = 0; i < widgets.size(); i++)
		widgets[i]->draw();
	
	restoreTransformGL();
}


void ofxControlGroup::hittest()
{
	transformGL();
	
	for (int i = 0; i < widgets.size(); i++)
	{
		ofxControlWidget *w = widgets[i];
		glPushName(w->getID());
		w->hittest();
		glPopName();
	}

	restoreTransformGL();
}