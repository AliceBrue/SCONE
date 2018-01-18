#include "opt_tools.h"
#include "scone/core/types.h"

#include "scone/core/Factories.h"

#include "scone/objectives/SimulationObjective.h"
#include "scone/core/Profiler.h"

using namespace std;

#include "xo/time/timer.h"
#include "xo/stream/prop_node_tools.h"
#include "xo/filesystem/filesystem.h"

using xo::timer;

namespace scone
{
	SCONE_API OptimizerUP PrepareOptimization( const PropNode& props, const path& scenario_file )
	{
		// set current path to config file path
		if ( scenario_file.has_parent_path() )
			xo::current_path( scenario_file.parent_path() );

		// create optimizer and report unused parameters
		OptimizerUP o = CreateOptimizer( props.get_child( "Optimizer" ) );
		LogUntouched( props );

		// copy original and write resolved config files
		xo::path outdir( o->AcquireOutputFolder().str() );
		xo::copy_file( scenario_file.filename(), outdir / path( "config_original" ).replace_extension( scenario_file.extension() ), true );
		xo::save_xml( props, path( ( outdir / "config.xml" ).string() ) );

		// copy all objective resources to output folder
		for ( auto& f : o->GetObjective().GetExternalResources() )
			xo::copy_file( f, outdir / f.filename(), true );

		// return created optimizer
		return std::move( o );
	}

	PropNode SCONE_API SimulateObjective( const path& filename )
	{
		xo::path config_path = filename.parent_path() / "config.xml";
		if ( config_path.has_parent_path() )
			current_path( config_path.parent_path() );

		const PropNode configProp = xo::load_file_with_include( path( config_path.string() ), "INCLUDE" );
		const PropNode& objProp = configProp[ "Optimizer" ][ "Objective" ];
		ObjectiveUP obj = CreateObjective( objProp );
		SimulationObjective& so = dynamic_cast<SimulationObjective&>( *obj );

		// report unused parameters
		LogUntouched( objProp );

		// set data storage
		auto model = so.CreateModelFromParFile( filename );
		model->SetStoreData( true );
		Profiler::GetGlobalInstance().Reset();

		timer tmr;
		double result = so.EvaluateModel( *model );
		auto duration = tmr.seconds();

		// collect statistics
		PropNode statistics;
		statistics.set( "result", model->GetMeasure()->GetReport() );
		statistics.set( "simulation time", model->GetTime() );
		statistics.set( "performance (x real-time)", model->GetTime() / duration );

		log::info( statistics );

		// write results
		obj->WriteResults( path( filename ).replace_extension().str() );

		return statistics;
	}
}
