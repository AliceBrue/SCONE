#pragma once

#include "ExampleObjective.h"
#include "..\opt\Optimizer.h"
#include "..\cs\SimulationObjective.h"

using namespace scone;

void OptimizationTest()
{
	// register new objective
	ExampleObjective::RegisterFactory();
	cs::RegisterFactoryTypes();

	opt::OptimizerSP opt = opt::CreateOptimizerFromXml( "config/example_optimization.xml" );

	opt->Run();
}
