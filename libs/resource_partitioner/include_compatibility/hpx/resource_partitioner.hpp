//  Copyright (c) 2020 ETH Zurich
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <hpx/config.hpp>
#include <hpx/resource_partitioner/config/defines.hpp>
#include <hpx/modules/resource_partitioner.hpp>

#if HPX_RESOURCE_PARTITIONER_HAVE_DEPRECATION_WARNINGS
#if defined(HPX_MSVC)
#pragma message("The header hpx/resource_partitioner.hpp is deprecated, \
    please include hpx/modules/resource_partitioner.hpp instead")
#else
#warning "The header hpx/resource_partitioner.hpp is deprecated, \
    please include hpx/modules/resource_partitioner.hpp instead"
#endif
#endif
