#include "RotatingSkyObject.h"

using namespace Ogre;

namespace CartoonCaelum {

	RotatingSkyObject::RotatingSkyObject
	(
		SceneNode *pNode,
		int nDistance,
		Radian pitch
	):
		m_pObjectNode(pNode),
		m_previousRotation(Radian(0)),
		m_nObjectDistance(nDistance),
		m_cyclePitch(pitch)
	{

	}

	RotatingSkyObject::~RotatingSkyObject()
	{

	}

	void RotatingSkyObject::moveObject(Radian degrees)
	{
		//add degree to the previously accumulated rotation, and calculate new position of object.
		m_previousRotation += degrees;
		int _newX = InternalUtilities::round(m_nObjectDistance*(Math::Sin(m_previousRotation, false)));
		int _newY = InternalUtilities::round(m_nObjectDistance*(Math::Cos(m_previousRotation, false)));
		int _newZ = InternalUtilities::round(_newY*(Math::Sin(m_cyclePitch, false))); 
		m_pObjectNode->setPosition(Vector3(_newX, _newY, _newZ));
		m_pObjectNode->lookAt(Vector3(0,0,0), 
			Node::TransformSpace::TS_WORLD, Vector3::NEGATIVE_UNIT_Y);
	}

}
