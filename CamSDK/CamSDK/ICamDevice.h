#ifndef _ICAMMERA_DEVICE_H_
#define _ICAMMERA_DEVICE_H_
#include "BaseDataDefine.h"

class ICamDevice
{

};

class IDomDocument
{
public:
	IDomDocument() {};
	virtual void Init(const void *buffer, int size) = 0;
};
struct IGcNode {
	IGcNode	*next_sibling;
	IGcNode	*previous_sibling;
	IGcNode	*parent_node;
	IGcNode	*first_child;
	IGcNode	*last_child;
};
#endif
