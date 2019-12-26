/*
Copyright (c) 2019,
Battelle Memorial Institute; Lawrence Livermore National Security, LLC; Alliance for Sustainable Energy, LLC.  See
the top-level NOTICE for additional details. All rights reserved.
SPDX-License-Identifier: BSD-3-Clause
*/
#include "helics/application_api.hpp"
#include "helics/core/helicsCLI11.hpp"


int main (int argc, char *argv[]) { 
	helics::helicsCLI11App bbxcli;
	auto result = bbxcli.helics_parse(argc, argv);
	switch (result)
	{
		case helics::helicsCLI11App::parse_output::version_call:
			break;
		case helics::helicsCLI11App::parse_output::help_call:
			case helics::helicsCLI11App::parse_output::help_all_call:
				return 0;
			default:
				break;

	}

	helics::FederateInfo fi(bbxcli.remainArgs());
	fi.defName = "bbxfed";
	helics::CombinationFederate cFed(std::string(),fi);

	cFed.disconnect();
	
}
