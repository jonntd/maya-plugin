#ifndef _POINTSTRANSFORMERCTX_H
#define _POINTSTRANSFORMERCTX_H

#include <maya/MPxSelectionContext.h>
#include <maya/MSelectionList.h>

class PointsTransformCtx : public MPxSelectionContext
{
public:
	PointsTransformCtx();
	
	virtual MStatus		doPress(MEvent & event);
	virtual MStatus		doRelease(MEvent & event);
	virtual MStatus		doDrag(MEvent & event);	
	virtual MStatus 	helpStateHasChanged(MEvent &);

	virtual MStatus     addManipulator(const MObject & manipulator);
	virtual MStatus     deleteManipulators();

private:
	void updateSelection(const short& x, const short& y);
	void doMove();
	void calcMinDist();
	bool isNear(const MPoint& cv);
	MSelectionList activeSelection_;
	short x_, y_, dx_, dy_;
	MVector delta_;
	double minDist_;
	bool dragging_;
};

#endif //_POINTSTRANSFORMERCTX_H