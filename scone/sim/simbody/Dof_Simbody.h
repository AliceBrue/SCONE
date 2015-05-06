#pragma once

#include "sim_simbody.h"
#include "../Dof.h"

namespace OpenSim
{
	class Coordinate;
}

namespace scone
{
	namespace sim
	{
		class SCONE_SIM_SIMBODY_API Dof_Simbody : public Dof
		{
		public:
			Dof_Simbody( class Model_Simbody& model, OpenSim::Coordinate& coord );
			virtual ~Dof_Simbody();

			virtual Real GetPos() override;
			virtual Real GetVel() override;
			virtual const String& GetName() const override;

			virtual Real GetLimitForce() override;


		private:
			Model_Simbody& m_Model;
			OpenSim::Coordinate& m_osCoord;
			const OpenSim::CoordinateLimitForce* m_pOsLimitForce;
		};
	}
}