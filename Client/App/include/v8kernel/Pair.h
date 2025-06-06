#pragma once
#include "v8kernel/Body.h"
#include "util/Math.h"
#include <G3DAll.h>

namespace RBX
{
	enum GeoPairType
	{
		BALL_BALL_PAIR = 0,
		BALL_POINT_PAIR = 1,
		BALL_EDGE_PAIR = 2,
		BALL_PLANE_PAIR = 3,
		POINT_PLANE_PAIR = 4,
		EDGE_EDGE_PLANE_PAIR = 5,
		EDGE_EDGE_PAIR = 6
	};

	class PairParams
	{
	public:
		G3D::Vector3 normal;
		union
		{
			float length;
			float rotation;
		};
		G3D::Vector3 position;
		float rotVel;

	public:
		PairParams(const PairParams&);
		PairParams() : normal(0, 0, 0), position(0, 0, 0) {};

		RBX::PairParams& operator=(const RBX::PairParams&);
	};

	class GeoPair
	{
	private:
	struct GeoPairData
	{
		union
		{
			NormalId normalID0;
			float radius0;
		};
		union
		{
			NormalId normalID1;
			float radiusSum;
		};
		union
		{
			NormalId planeID;
			int point0ID;
		};
	};
	private:
		GeoPairType geoPairType;
		const G3D::Vector3* offset0;
		const G3D::Vector3* offset1;
		Body* body0;
		Body* body1;
		float edgeLength0;
		GeoPairData pairData;
	private:
		void computeBallBall(RBX::PairParams&);
		void computeBallPoint(RBX::PairParams&);
		void computeBallEdge(RBX::PairParams&);
		void computeBallPlane(RBX::PairParams&);
		void computePointPlane(RBX::PairParams&);
		void computeEdgeEdgePlane(RBX::PairParams&);
		void computeEdgeEdge(RBX::PairParams&);
	public:
		GeoPair();

		const RBX::Body* getBody(int i) const {return i == 0 ? this->body0 : this->body1;}
		RBX::Body* getBody(int i) {return i == 0 ? this->body0 : this->body1;}

		void computeLengthNormalPosition(PairParams& _params)
		{
			switch (this->geoPairType)
			{
			case BALL_BALL_PAIR:
				this->computeBallBall(_params);
				break;
			case BALL_POINT_PAIR:
				this->computeBallPoint(_params);
				break;
			case BALL_EDGE_PAIR:
				this->computeBallEdge(_params);
				break;
			case BALL_PLANE_PAIR:
				this->computeBallPlane(_params);
				break;
			case POINT_PLANE_PAIR:
				this->computePointPlane(_params);
				break;
			case EDGE_EDGE_PLANE_PAIR:
				this->computeEdgeEdgePlane(_params);
				break;
			case EDGE_EDGE_PAIR:
				this->computeEdgeEdge(_params);
				break;
			}
		}

		void computeNormalPerpVel(float& normalVel, G3D::Vector3& perpVel, const PairParams& _params);
		void forceToBodies(const G3D::Vector3&, const G3D::Vector3&);
		void setBallBall(Body*, Body*, float, float);
		void setBallBlock(Body*, Body*, float, const G3D::Vector3*, NormalId, GeoPairType);
		void setPointPlane(Body*, Body*, const G3D::Vector3*, const G3D::Vector3*, int, RBX::NormalId);
		void setEdgeEdgePlane(Body*, Body*, const G3D::Vector3*, const G3D::Vector3*, NormalId, NormalId, NormalId, float);
		void setEdgeEdge(Body*, Body*, const G3D::Vector3*, const G3D::Vector3*, NormalId, NormalId);

		bool GeoPair::match(Body* _b0, Body* _b1, GeoPairType _pairType, int param0, int param1)
		{
			if (_pairType == POINT_PLANE_PAIR)
			{
				return _b0 == this->body0 && _b1 == this->body1 && param0 == this->pairData.point0ID && param1 == this->pairData.normalID1;
			}
			else if (_pairType == EDGE_EDGE_PLANE_PAIR)
			{
				return _b0 == this->body0 && _b1 == this->body1 && param0 == this->pairData.normalID0 && param1 == this->pairData.normalID1;
			}
			else
			{
				return (_b0 == this->body0 && _b1 == this->body1 && param0 == this->pairData.normalID0 && param1 == this->pairData.normalID1)
					|| (_b0 == this->body1 && _b1 == this->body0 && param0 == this->pairData.normalID1 && param1 == this->pairData.normalID0);
			}
		}
	};
}