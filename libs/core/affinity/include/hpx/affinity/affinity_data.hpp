//  Copyright (c) 2007-2025 Hartmut Kaiser
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <hpx/config.hpp>
#include <hpx/topology/topology.hpp>

#include <atomic>
#include <cstddef>
#include <string>
#include <vector>

#include <hpx/config/warnings_prefix.hpp>

namespace hpx::threads::policies::detail {

    ///////////////////////////////////////////////////////////////////////////
    // Structure holding the information related to thread affinity selection
    // for the shepherd threads of this instance
    struct HPX_CORE_EXPORT affinity_data
    {
        affinity_data();

        affinity_data(affinity_data const&);
        affinity_data(affinity_data&&) noexcept;
        affinity_data& operator=(affinity_data const&);
        affinity_data& operator=(affinity_data&&) noexcept;

        ~affinity_data();

        void init(std::size_t num_threads = 1, std::size_t max_cores = 1,
            std::size_t pu_offset = 0, std::size_t pu_step = 1,
            std::size_t used_cores = 0, std::string affinity_domain = "pu",
            std::string const& affinity_description = "balanced",
            bool use_process_mask = false);

        void set_num_threads(size_t num_threads) noexcept;

        void set_affinity_masks(
            std::vector<threads::mask_type> const& affinity_masks);
        void set_affinity_masks(
            std::vector<threads::mask_type>&& affinity_masks) noexcept;

        constexpr std::size_t get_num_threads() const noexcept
        {
            return num_threads_;
        }

        constexpr bool affinities_disabled() const noexcept
        {
            return disable_affinities_;
        }

        mask_type get_pu_mask(
            threads::topology const& topo, std::size_t global_thread_num) const;

        mask_type get_used_pus_mask(
            threads::topology const& topo, std::size_t pu_num) const;
        std::size_t get_thread_occupancy(
            threads::topology const& topo, std::size_t pu_num) const;

        std::size_t get_pu_num(std::size_t num_thread) const noexcept;
        void set_pu_nums(std::vector<std::size_t> const& pu_nums);
        void set_pu_nums(std::vector<std::size_t>&& pu_nums) noexcept;

        void add_punit(std::size_t virt_core, std::size_t thread_num);
        void init_cached_pu_nums(std::size_t hardware_concurrency);

        constexpr std::size_t get_num_pus_needed() const noexcept
        {
            return num_pus_needed_;
        }

    protected:
        std::size_t get_pu_num(
            std::size_t num_thread, std::size_t hardware_concurrency) const;

    private:
        ///< number of processing units managed
        std::size_t num_threads_;

        ///< offset of the first processing unit to use
        std::size_t pu_offset_;

        ///< step between used processing units
        std::size_t pu_step_;
        std::size_t used_cores_;
        std::string affinity_domain_;
        std::vector<mask_type> affinity_masks_;
        std::vector<std::size_t> pu_nums_;

        ///< mask of processing units which have no affinity
        mask_type no_affinity_;
        bool disable_affinities_;

        ///< use the process CPU mask to limit available PUs
        bool use_process_mask_;
        std::size_t num_pus_needed_;

        ///< counter for instance numbers
        static std::atomic<int> instance_number_counter_;
    };
}    // namespace hpx::threads::policies::detail

#include <hpx/config/warnings_suffix.hpp>
