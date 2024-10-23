#pragma once
#include <G3DAll.h>
//#include "body.h"

namespace RBX {
	class Body;
	class Cofm
	{
		private:
			Body *body;
			bool dirty;
			G3D::Vector3 cofmInBody;
			float mass;
			G3D::Matrix3 moment;
			void updateIfDirty();
		public:
			G3D::Vector3& getCofmInBody() const;
			bool getIsDirty() const {return dirty;}
			void makeDirty() {dirty = true;}
	};
}