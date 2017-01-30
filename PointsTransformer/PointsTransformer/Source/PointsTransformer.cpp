#include "PointsTransformer.h"
#include "PointsTransformCtx.h"
#include "PointsTransformCtxCmd.h"
#include "PointsTransformCmd.h"

// Functions required for registering
// and unregistering the plugin
MStatus initializePlugin(MObject obj)
{
	MStatus status;
	MFnPlugin plugin(obj, "Tesla", "0.1", "xxx");	

	status = plugin.registerContextCommand("PointsTransformCtx", &PointsTransformCtxCmd::creator,
										   "TransformPoints", &PointsTransformCmd::creator);

	return MStatus::kSuccess;
}

MStatus uninitializePlugin(MObject obj)
{

	return MStatus::kSuccess;
}