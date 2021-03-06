# Copyright (c) 2019 The STE||AR-Group
#
# SPDX-License-Identifier: BSL-1.0
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

if(NOT HPX_WITH_ASYNC_CUDA)
  return()
endif()

if(NOT TARGET Cuda::cuda)
  message(
    FATAL_ERROR
      "Cuda future support has been enabled, but the Cuda installation is not setup"
  )
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Default location is $HPX_ROOT/libs/async_cuda/include
set(async_cuda_headers
    hpx/async_cuda/cuda_event.hpp
    hpx/async_cuda/cuda_executor.hpp
    hpx/async_cuda/cuda_exception.hpp
    hpx/async_cuda/cuda_future.hpp
    hpx/async_cuda/cublas_executor.hpp
    hpx/async_cuda/get_targets.hpp
    hpx/async_cuda/target.hpp
)

# Default location is $HPX_ROOT/libs/async_cuda/include_compatibility
set(async_cuda_compat_headers hpx/compute/cuda/get_targets.hpp
                              hpx/compute/cuda/target.hpp
)

set(async_cuda_sources cuda_future.cpp cuda_target.cpp get_targets.cpp)

if(HPX_WITH_DISTRIBUTED_RUNTIME)
  set(async_cuda_extra_module_dependencies hpx_async_distributed)
endif()

include(HPX_AddModule)
add_hpx_module(
  async_cuda
  COMPATIBILITY_HEADERS ON
  DEPRECATION_WARNINGS
  GLOBAL_HEADER_GEN ON
  SOURCES ${async_cuda_sources}
  HEADERS ${async_cuda_headers}
  COMPAT_HEADERS ${async_cuda_compat_headers}
  DEPENDENCIES
    hpx_actions_base
    hpx_affinity
    hpx_allocator_support
    hpx_assertion
    hpx_concurrency
    hpx_config
    hpx_coroutines
    hpx_errors
    hpx_execution
    hpx_execution_base
    hpx_functional
    hpx_futures
    hpx_memory
    hpx_performance_counters
    hpx_program_options
    hpx_runtime_configuration
    hpx_synchronization
    hpx_static_reinit
    hpx_threading_base
    hpx_type_support
    ${async_cuda_extra_module_dependencies}
    Cuda::cuda
  CMAKE_SUBDIRS examples tests
)
