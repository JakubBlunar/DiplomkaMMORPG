#ifndef MANAGER_H
#define MANAGER_H

class Manager
{
protected:
	Manager();
	~Manager();

public:
	bool dynamic;
	virtual void update() {}
};

#endif