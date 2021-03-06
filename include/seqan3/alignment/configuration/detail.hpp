// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2006-2019, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2019, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/seqan/seqan3/blob/master/LICENSE
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \brief Provides some utility functions for the alignment configurations.
 * \author Rene Rahn <rene.rahn AT fu-berlin.de>
 */

#pragma once

#include <seqan3/core/algorithm/configuration_utility.hpp>

namespace seqan3::detail
{

/*!\brief An internal enum to check for a consistent configuration object.
 * \ingroup alignment_configuration
 */
enum struct align_config_id : uint8_t
{
    aligned_ends, //!< ID for the \ref seqan3::align_cfg::aligned_ends "aligned_ends" option.
    band, //!< ID for the \ref seqan3::align_cfg::band "band" option.
    gap, //!< ID for the \ref seqan3::align_cfg::gap "gap" option.
    global, //!< ID for the \ref seqan3::global_alignment "global alignment" option.
    max_error, //!< ID for the \ref seqan3::align_cfg::max_error "max_error" option.
    result, //!< ID for the \ref seqan3::align_cfg::result "result" option.
    scoring, //!< ID for the \ref seqan3::align_cfg::scoring "scoring" option.
    SIZE //!< Represents the number of configuration elements.
};

// ----------------------------------------------------------------------------
// compatibility_table
// ----------------------------------------------------------------------------

/*!\brief Declaration of algorithm specific compatibility table.
 * \ingroup algorithm
 * \tparam algorithm_id_type The type of the algorithm specific id. Algorithm configurations must maintain
 *                           this table to allow validation checks.
 */
template <>
inline constexpr std::array<std::array<bool, static_cast<uint8_t>(align_config_id::SIZE)>,
                            static_cast<uint8_t>(align_config_id::SIZE)> compatibility_table<align_config_id>
{
    {   //0  1  2  3  4  5  6
        { 0, 1, 1, 1, 1, 1, 1}, // 0: aligned_ends
        { 1, 0, 1, 1, 1, 1, 1}, // 1: band
        { 1, 1, 0, 1, 1, 1, 1}, // 2: gap
        { 1, 1, 1, 0, 1, 1, 1}, // 3: global
        { 1, 1, 1, 1, 0, 1, 1}, // 4: max_error
        { 1, 1, 1, 1, 1, 0, 1}, // 5: result
        { 1, 1, 1, 1, 1, 1, 0}  // 6: scoring
    }
};

} // namespace seqan3::detail
