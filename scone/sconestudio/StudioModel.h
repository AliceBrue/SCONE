#pragma once

#include "scone/core/system_tools.h"
#include "scone/sim/Model.h"
#include "simvis/scene.h"
#include "scone/cs/SimulationObjective.h"
#include "scone/opt/Objective.h"

#include "flut/flag_set.hpp"
#include "simvis/arrow.h"

#include "SconeStorageDataModel.h"
#include "simvis/axes.h"
#include "scone/core/State.h"

namespace scone
{
	SCONE_DECLARE_CLASS_AND_PTR( StudioModel );

	class StudioModel
	{
	public:
		enum ViewSettings { ShowForces, ShowMuscles, ShowGeometry, EnableShadows };
		typedef flut::flag_set< ViewSettings > ViewFlags;
 
		StudioModel( vis::scene &s, const path& filename );
		virtual ~StudioModel();

		void UpdateVis( TimeInSeconds t );

		void UpdateForceVis( Index force_idx, Vec3 cop, Vec3 force );

		void EvaluateObjective();
		void EvaluateTo( TimeInSeconds t );
		void FinalizeEvaluation( bool output_results );

		const Storage<>& GetData() { return data; }
		sim::Model& GetSimModel() { return so->GetModel(); }
		cs::SimulationObjective& GetObjective() { return *so; }

		bool IsEvaluating() { return is_evaluating; }

		void SetViewFlags( const ViewFlags& f ) { view_flags = f; }
		void SetViewSetting( ViewSettings e, bool value );

	private:
		void InitVis( vis::scene& s );

		Storage<> data;
		cs::SimulationObjectiveUP so;
		path filename;

		ViewFlags view_flags;

		std::vector< size_t > state_data_index;
		scone::State model_state;

		vis::material bone_mat;
		vis::material arrow_mat;
		vis::material muscle_mat;

		bool is_evaluating;

		vis::group root;
		std::vector< std::vector< vis::mesh > > body_meshes;
		std::vector< vis::mesh > joints;
		std::vector< std::pair< vis::trail, vis::material > > muscles;
		std::vector< vis::arrow > forces;
		std::vector< vis::axes > body_centers;
		vis::mesh com;
		void InitStateDataIndices();
	};
}
