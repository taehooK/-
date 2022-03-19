//Observer.h

#ifndef _OBSERVER_H
#define _OBSERVER_H

class Subject;
class Observer {
public:
	virtual ~Observer() = 0;
	virtual void Update() = 0;
	virtual Observer* GetClone() = 0;
	Observer();
	Observer(const Observer& source);

};

#endif // !_OBSERVER_H
