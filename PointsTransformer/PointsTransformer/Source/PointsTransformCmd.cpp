#include "PointsTransformCmd.h"


void* PointsTransformCmd::creator()
{
	return new PointsTransformCmd;
}

PointsTransformCmd::PointsTransformCmd()
{
	setCommandString("Points Transform Command");
}

PointsTransformCmd::~PointsTransformCmd()
{
}

MStatus
PointsTransformCmd::doIt(const MArgList& args)
{
	return MStatus::kSuccess;
}
	
MStatus
PointsTransformCmd::redoIt()
{
	return MStatus::kSuccess;
}

MStatus
PointsTransformCmd::undoIt()
{
	return MStatus::kSuccess;
}

bool
PointsTransformCmd::isUndoable() const
{
	return false;
}
	
MStatus
PointsTransformCmd::finalize()
{
	return MStatus::kSuccess;
}

