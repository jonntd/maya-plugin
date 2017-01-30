#ifndef _POINTSTRNSFORMCTXCMD_H
#define _POINTSTRNSFORMCTXCMD_H

#include <maya/MPxContextCommand.h>

class PointsTransformCtxCmd : public MPxContextCommand
{
public:
	static void* creator();	
	virtual MPxContext* makeObj();

private:

};

#endif //_POINTSTRNSFORMCTXCMD_H