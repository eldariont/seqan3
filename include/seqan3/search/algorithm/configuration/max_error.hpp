// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2006-2019, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2019, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/seqan/seqan3/blob/master/LICENSE
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \brief Provides the configuration for maximum number of errors for all error types.
 * \author Christopher Pockrandt <christopher.pockrandt AT fu-berlin.de>
 * \author Rene Rahn <rene.rahn AT fu-berlin.de>
 */

#pragma once

#include <range/v3/algorithm/fill.hpp>
#include <range/v3/algorithm/for_each.hpp>
#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/view/slice.hpp>

#include <seqan3/core/algorithm/pipeable_config_element.hpp>
#include <seqan3/core/algorithm/parameter_pack.hpp>
#include <seqan3/search/algorithm/configuration/detail.hpp>
#include <seqan3/search/algorithm/configuration/max_error_common.hpp>

namespace seqan3::search_cfg
{
/*!\brief A configuration element for the maximum number of errors across all error types (mismatches, insertions,
 *        deletions). This is an upper bound of errors independent from error numbers of specific error types.
 * \details An insertion corresponds to a base inserted into the query that does not occur in the text at the position,
 *          a deletion corresponds to a base deleted from the query sequence that does occur in the indexed text.
 *          Deletions at the beginning and at the end of the sequence are not considered during a search.
 * \ingroup search_configuration
 */
template <typename ...errors_t>
//!\cond
    requires sizeof...(errors_t) <= 4 &&
            ((detail::is_type_specialisation_of_v<std::remove_reference_t<errors_t>, total> ||
              detail::is_type_specialisation_of_v<std::remove_reference_t<errors_t>, substitution> ||
              detail::is_type_specialisation_of_v<std::remove_reference_t<errors_t>, deletion>  ||
              detail::is_type_specialisation_of_v<std::remove_reference_t<errors_t>, insertion>) && ...)
//!\endcond
class max_error : public pipeable_config_element<max_error<errors_t...>, std::array<uint8_t, 4>>
{
    //!\brief An alias type for the base class.
    using base_t = pipeable_config_element<max_error<errors_t...>, std::array<uint8_t, 4>>;

    //!\brief Helper function to check valid max error configuration.
    template <typename ..._errors_t>
    static constexpr bool check_consistency(_errors_t ...errors)
    {
        if constexpr (sizeof...(errors) < 2)
        {
            return true;
        }
        else
        {
            return [] (auto head, auto ...tail) constexpr
            {
                using head_t = decltype(head);
                if constexpr (((head_t::_id != decltype(tail)::_id) && ...))
                    return check_consistency(tail...);
                else
                    return false;
            }(errors...);
        }
    }

    static_assert(check_consistency(errors_t{}...),
                  "You may not use the same error specifier more than once.");

public:

    //!\privatesection
    //!\brief Internal id to check for consistent configuration settings.
    static constexpr detail::search_config_id id{detail::search_config_id::max_error};

    //!\publicsection
    /*!\name Constructor, destructor and assignment
     * \brief Defaulted all standard constructor.
     * \{
     */
    constexpr max_error()                              noexcept = default;
    constexpr max_error(max_error const &)             noexcept = default;
    constexpr max_error(max_error &&)                  noexcept = default;
    constexpr max_error & operator=(max_error const &) noexcept = default;
    constexpr max_error & operator=(max_error &&)      noexcept = default;
    ~max_error()                                       noexcept = default;

    /*!\brief Constructs the object from a set of error specifiers.
     * \tparam    errors_t A template parameter pack with the error types.
     * \param[in] errors   A pack of error specifiers.
     *
     * \details
     *
     * \todo write me
     */
    constexpr max_error(errors_t && ...errors) noexcept
    //!\cond
        requires sizeof...(errors_t) > 0
    //!\endcond
        : base_t{}
    {
        detail::for_each_value([this](auto e)
        {
            base_t::value[remove_cvref_t<decltype(e)>::_id()] = e.get();
        }, std::forward<errors_t>(errors)...);

        // Only total is set so we set all other errors to the total limit.
        if constexpr (((std::remove_reference_t<errors_t>::_id() == 0) || ...) && sizeof...(errors) == 1)
        {
            ranges::fill(base_t::value | ranges::view::slice(1, 4), base_t::value[0]);
        } // otherwise if total is not set but any other field is set than use total as the sum of all set errors.
        else if constexpr (!((std::remove_reference_t<errors_t>::_id() == 0) || ...) && sizeof...(errors) > 0)
        {
            base_t::value[0] = std::min(static_cast<uint8_t>(255), ranges::accumulate(base_t::value | ranges::view::slice(1, 4),
                                                                              static_cast<uint8_t>(0)));
        }
    }
    //!}
};

/*!\name Type deduction guides
 * \relates seqan3::search_cfg::max_error
 * \{
 */

//!\brief Deduces empty list of error specifiers.
max_error() -> max_error<>;

//!\brief Deduces template arguments from the passed error specifiers.
template <typename ...errors_t>
max_error(errors_t && ...) -> max_error<remove_cvref_t<errors_t>...>;
//!\}

} // namespace seqan3::search_cfg
