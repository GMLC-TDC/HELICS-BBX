# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# Copyright (c) 2017-2019, Battelle Memorial Institute; Lawrence Livermore
# National Security, LLC; Alliance for Sustainable Energy, LLC.
# See the top-level NOTICE for additional details.
# All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


set(helics-bbx_sources
	helics-bbx.cpp
	)

set(helics-bbx_headers
	)

add_executable(helics-bbx ${helics-bbx_sources} ${helics-bbx_headers})


target_link_libraries(helics-bbx HELICS::helics_apps)


INSTALL(TARGETS helics-bbx RUNTIME DESTINATION bin)
