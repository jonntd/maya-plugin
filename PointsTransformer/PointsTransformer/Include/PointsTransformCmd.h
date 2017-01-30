#ifndef _POINTSTRANSFORMCMD_H
#define _POINTSTRANSFORMCMD_H

#include <maya/MPxToolCommand.h>

class PointsTransformCmd : MPxToolCommand
{
public:
	static void* creator();
	PointsTransformCmd();
	virtual ~PointsTransformCmd();

	virtual MStatus     doIt(const MArgList& args);
	virtual	MStatus     redoIt();
	virtual MStatus     undoIt();
	virtual bool        isUndoable() const;
	virtual MStatus		finalize();

private:
};

#endif //_POINTSTRANSFORMCMD_H