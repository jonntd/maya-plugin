#include "PointsTransformCtx.h"
#include "stdlib.h"

#include "maya/MGlobal.h"
#include "maya/MSelectionList.h"
#include "maya/MRichSelection.h"
#include "maya/MItSelectionList.h"
#include "maya/MItMeshVertex.h"
#include "maya/MDagPath.h"
#include "maya/MFnDagNode.h"
#include "maya/MItDag.h"
#include "maya/MItGeometry.h"
#include "maya/MItCurveCV.h"
#include "maya/MItSurfaceCV.h"
#include "maya\MItMeshVertex.h"

PointsTransformCtx::PointsTransformCtx()
	: x_(0),
	y_(0),
	dx_(0),
	dy_(0),
	delta_(0,0),
	minDist_(0.0),
	dragging_(false)
{
	MString toolTitle("Points Transformer: click to select a model");
	setTitleString(toolTitle);
	
	// Hack to make cout work with output window
	//cout.rdbuf(cerr.rdbuf());	
}

MStatus
PointsTransformCtx::doPress(MEvent & event)
{	
	event.getPosition(x_, y_);	
	if (!event.isModifierShift())
	{
		updateSelection(x_, y_);
		dx_ = dy_ = 0;
	}
	return MStatus::kSuccess;
}

MStatus
PointsTransformCtx::doRelease(MEvent & event)
{
	if (event.isModifierShift())
	{
		MVector origVec(x_, y_);
		MVector dragVec(dx_, dy_);
		delta_ = dragVec - origVec;
		delta_ /= 100;
		doMove();
	}
	return MStatus::kSuccess;
}

MStatus
PointsTransformCtx::doDrag(MEvent & event)
{
	if (event.isModifierShift())
	{
		event.getPosition(dx_, dy_);
	}

	return MStatus::kSuccess;
}

MStatus 
PointsTransformCtx::helpStateHasChanged(MEvent &)
{
	return MStatus::kSuccess;
}

MStatus 
PointsTransformCtx::addManipulator(const MObject & manipulator)
{
	return MStatus::kSuccess;
}

MStatus 
PointsTransformCtx::deleteManipulators()
{
	return MStatus::kSuccess;
}

void PointsTransformCtx::updateSelection(const short& x, const short& y)
{
	MGlobal::setDisplayCVs(activeSelection_, false);
	activeSelection_.clear();
	MGlobal::setActiveSelectionList(activeSelection_);
	MGlobal::setSelectionMode(MGlobal::MSelectionMode::kSelectObjectMode);	
	MGlobal::selectFromScreen(x, y, MGlobal::kReplaceList);
	MGlobal::setDisplayCVs(activeSelection_, true);
	MGlobal::getActiveSelectionList(activeSelection_);
	calcMinDist();	
}

void PointsTransformCtx::calcMinDist()
{
	double minDist = 1e10;
	double maxDist = 0;
	MVector origVec(x_, y_, 0);
	MObject component;
	MStatus status;
	MSpace::Space cs = MSpace::kWorld;
	MItSelectionList selIter(activeSelection_, MFn::kInvalid);
	for (; !selIter.isDone(); selIter.next())
	{
		MDagPath dagPath;
		selIter.getDagPath(dagPath);
		dagPath.extendToShape();

		MFnDependencyNode depNodeFn(dagPath.node());

		cout << "Object Type: " << depNodeFn.name().asChar() << endl;

		MItCurveCV curveIter(dagPath, component, &status);
		if (status == MStatus::kSuccess)
		{
			for (; !curveIter.isDone(); curveIter.next())
			{
				double tempDist = curveIter.position(cs).distanceTo(origVec);
				if (tempDist < minDist)
					minDist = tempDist;
				if (tempDist > maxDist)
					maxDist = tempDist;
			}		
		}

		MItSurfaceCV surfIter(dagPath, component, true, &status);
		if (status == MStatus::kSuccess)
		{
			for (; !surfIter.isDone(); surfIter.next())
			{
				double tempDist = surfIter.position(cs).distanceTo(origVec);
				if (tempDist < minDist)
					minDist = tempDist;
				if (tempDist > maxDist)
					maxDist = tempDist;
			}			
		}

		MItMeshVertex meshIter(dagPath, component, &status);
		if (status == MStatus::kSuccess)
		{
			for (; !meshIter.isDone(); meshIter.next())
			{
				double tempDist = meshIter.position(cs).distanceTo(origVec);
				if (tempDist < minDist)
					minDist = tempDist;
				if (tempDist > maxDist)
					maxDist = tempDist;
			}			
		}
	}

	minDist_ = (minDist + maxDist) / 2.0;
}


void PointsTransformCtx::doMove()
{
	MObject component;
	MStatus status;
	MSpace::Space cs = MSpace::kWorld;
	MItSelectionList selIter(activeSelection_, MFn::kInvalid);
	for (; !selIter.isDone(); selIter.next())
	{
		MDagPath dagPath;
		selIter.getDagPath(dagPath);
		dagPath.extendToShape();

		MFnDependencyNode depNodeFn(dagPath.node());

		cout << "Object Type: " << depNodeFn.name().asChar() << endl;

		MItCurveCV curveIter(dagPath, component, &status);
		if (status == MStatus::kSuccess)
		{
			for (; !curveIter.isDone(); curveIter.next())
			{
				if (isNear(curveIter.position(cs)))
				{
					status = curveIter.translateBy(delta_, cs);					
					//cout << "Point: " << curveIter.position(cs).x << ", " << curveIter.position(cs).y << ", " << curveIter.position(cs).z << endl;
				}
			}
			curveIter.updateCurve();
		}

		MItSurfaceCV surfIter(dagPath, component, true, &status);
		if (status == MStatus::kSuccess)
		{
			for (; !surfIter.isDone(); surfIter.next())
			{
				if (isNear(surfIter.position(cs)))
				{
					status = surfIter.translateBy(delta_, cs);					
					//cout << "Point: " << surfIter.position(cs).x << ", " << surfIter.position(cs).y << ", " << surfIter.position(cs).z << endl;
				}
			}
			surfIter.updateSurface();
		}

		MItMeshVertex meshIter(dagPath, component, &status);
		if (status == MStatus::kSuccess)
		{
			for (; !meshIter.isDone(); meshIter.next())
			{			
				if (isNear(meshIter.position(cs)))
				{
					status = meshIter.translateBy(delta_, cs);					
					//cout << "Point: " << meshIter.position(cs).x << ", " << meshIter.position(cs).y << ", " << meshIter.position(cs).z << endl;
				}
			}
			meshIter.updateSurface();
		}
	}

}

bool PointsTransformCtx::isNear(const MPoint& cv)
{
	MVector origVec(x_, y_, 0);
	double dist = cv.distanceTo(origVec);

	if (dist < minDist_)
		return true;
	else
		return false;
}