//  Copyright (c) 2011 Matt Anderson
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#if defined(sgi) || defined(SGI) || defined(__sgi__) || defined(__SGI__)
#define FNAME(n_) n_##_
#elif defined(__INTEL_COMPILER)
#define FNAME(n_) n_##_
#elif defined(__GNUC__) && !defined(__INTEL_COMPILER)
#define FNAME(n_) n_##_
#elif defined(__PGI)
#define FNAME(n_) n_##_
#elif defined(HPX_MSVC)
#define FNAME(n_) n_
#else
#error "Unknown Fortran name mangling convention"
#endif
