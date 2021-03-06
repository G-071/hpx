# Copyright (c) 2019-2020 The STE||AR-Group
#
# SPDX-License-Identifier: BSL-1.0
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(runtime_local_headers
    hpx/runtime_local/config_entry.hpp
    hpx/runtime_local/custom_exception_info.hpp
    hpx/runtime_local/debugging.hpp
    hpx/runtime_local/get_locality_id.hpp
    hpx/runtime_local/get_num_localities.hpp
    hpx/runtime_local/get_os_thread_count.hpp
    hpx/runtime_local/get_thread_name.hpp
    hpx/runtime_local/get_worker_thread_num.hpp
    hpx/runtime_local/interval_timer.hpp
    hpx/runtime_local/os_thread_type.hpp
    hpx/runtime_local/pool_executor.hpp
    hpx/runtime_local/report_error.hpp
    hpx/runtime_local/run_as_hpx_thread.hpp
    hpx/runtime_local/run_as_os_thread.hpp
    hpx/runtime_local/runtime_handlers.hpp
    hpx/runtime_local/runtime_local.hpp
    hpx/runtime_local/runtime_local_fwd.hpp
    hpx/runtime_local/service_executors.hpp
    hpx/runtime_local/state.hpp
    hpx/runtime_local/shutdown_function.hpp
    hpx/runtime_local/startup_function.hpp
    hpx/runtime_local/thread_hooks.hpp
    hpx/runtime_local/thread_mapper.hpp
    hpx/runtime_local/thread_pool_helpers.hpp
    hpx/runtime_local/thread_stacktrace.hpp
    hpx/util/thread_aware_timer.hpp
)

set(runtime_local_compat_headers
    hpx/custom_exception_info.hpp
    hpx/parallel/executors/pool_executor.hpp
    hpx/parallel/executors/service_executors.hpp
    hpx/runtime/config_entry.hpp
    hpx/runtime/get_locality_id.hpp
    hpx/runtime/get_num_localities.hpp
    hpx/runtime/get_os_thread_count.hpp
    hpx/runtime/get_thread_name.hpp
    hpx/runtime/get_worker_thread_num.hpp
    hpx/runtime/report_error.hpp
    hpx/runtime/shutdown_function.hpp
    hpx/runtime/startup_function.hpp
    hpx/runtime/thread_hooks.hpp
    hpx/runtime/thread_pool_helpers.hpp
    hpx/runtime/threads/executors/pool_executor.hpp
    hpx/runtime/threads/executors/service_executors.hpp
    hpx/runtime/threads/run_as_hpx_thread.hpp
    hpx/runtime/threads/run_as_os_thread.hpp
    hpx/runtime_handlers.hpp
    hpx/util/debug/thread_stacktrace.hpp
    hpx/util/debugging.hpp
    hpx/util/interval_timer.hpp
    hpx/util/thread_mapper.hpp
)

set(runtime_local_sources
    custom_exception_info.cpp
    debugging.cpp
    interval_timer.cpp
    os_thread_type.cpp
    runtime_handlers.cpp
    runtime_local.cpp
    state.cpp
    thread_aware_timer.cpp
    thread_mapper.cpp
    thread_pool_helpers.cpp
    thread_stacktrace.cpp
)

include(HPX_AddModule)
add_hpx_module(
  runtime_local
  COMPATIBILITY_HEADERS ON
  DEPRECATION_WARNINGS
  GLOBAL_HEADER_GEN ON
  SOURCES ${runtime_local_sources}
  HEADERS ${runtime_local_headers}
  COMPAT_HEADERS ${runtime_local_compat_headers}
  DEPENDENCIES
    hpx_assertion
    hpx_command_line_handling
    hpx_config
    hpx_errors
    hpx_execution
    hpx_format
    hpx_functional
    hpx_io_service
    hpx_lcos_local
    hpx_logging
    hpx_naming_base
    hpx_program_options
    hpx_resource_partitioner
    hpx_runtime_configuration
    hpx_threadmanager
    hpx_thread_executors
    hpx_threading
    hpx_threading_base
    hpx_topology
    hpx_version
  CMAKE_SUBDIRS examples tests
)
