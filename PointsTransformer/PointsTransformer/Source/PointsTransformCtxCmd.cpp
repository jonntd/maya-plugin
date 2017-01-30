#include "PointsTransformCtxCmd.h"
#include "PointsTransformCtx.h"


void* PointsTransformCtxCmd::creator()
{
	return  new PointsTransformCtxCmd;
}

MPxContext* PointsTransformCtxCmd::makeObj()
{
	return new PointsTransformCtx;
}