// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2006-2019, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2019, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/seqan/seqan3/blob/master/LICENSE
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \author Hannes Hauswedell <hannes.hauswedell AT fu-berlin.de>
 * \brief Contains seqan3::rna5, container aliases and string literals.
 */

#pragma once

#include <vector>

#include <seqan3/alphabet/nucleotide/dna5.hpp>
#include <seqan3/alphabet/nucleotide/nucleotide_base.hpp>

// ------------------------------------------------------------------
// rna5
// ------------------------------------------------------------------

namespace seqan3
{

/*!\brief The five letter RNA alphabet of A,C,G,U and the unknown character N.
 * \ingroup nucleotide
 * \implements seqan3::NucleotideAlphabet
 * \implements seqan3::detail::ConstexprAlphabet
 * \implements seqan3::TriviallyCopyable
 * \implements seqan3::StandardLayout
 *
 * \details
 * This alphabet has the same internal representation as seqan3::dna5, the only difference is that it prints 'U' on
 * character conversion instead of 'T'. You can assign between values of seqan3::dna5 and seqan3::rna5.
 *
 * The alphabet may be brace initialized from the static letter members. Note that you cannot
 * assign the alphabet by using letters of type `char`, but you instead have to use the
 * function seqan3::rna5::assign_char().
 *
 *\snippet test/snippet/alphabet/nucleotide/rna5.cpp code
 */
class rna5 : public nucleotide_base<rna5, 5>
{
private:
    //!\brief The base class.
    using base_t = nucleotide_base<rna5, 5>;

    //!\brief Befriend nucleotide_base.
    friend base_t;
    //!\cond \brief Befriend seqan3::alphabet_base.
    friend base_t::base_t;
    //!\endcond

public:
    /*!\name Constructors, destructor and assignment
     * \{
     */
    constexpr rna5() noexcept : base_t{} {}
    constexpr rna5(rna5 const &) = default;
    constexpr rna5(rna5 &&) = default;
    constexpr rna5 & operator=(rna5 const &) = default;
    constexpr rna5 & operator=(rna5 &&) = default;
    ~rna5() = default;

    using base_t::base_t;

    //!\brief Allow implicit construction from dna/rna of the same size.
    constexpr rna5(dna5 const & r) noexcept
    {
        assign_rank(r.to_rank());
    }
    //!\}

protected:
    //!\privatesection

    //!\copydoc seqan3::dna4::rank_to_char
    static constexpr char_type rank_to_char[value_size]
    {
        'A',
        'C',
        'G',
        'N',
        'U'
    };

    //!\copydoc seqan3::dna4::char_to_rank
    static constexpr std::array<rank_type, 256> char_to_rank = dna5::char_to_rank;

    //!\copydoc seqan3::dna4::complement_table
    static const std::array<rna5, value_size> complement_table;
};

// ------------------------------------------------------------------
// containers
// ------------------------------------------------------------------

//!\brief Alias for an std::vector of seqan3::rna5.
//!\relates rna5
using rna5_vector = std::vector<rna5>;

// ------------------------------------------------------------------
// literals
// ------------------------------------------------------------------

/*!\name Literals
 * \{
 */

/*!\brief The seqan3::rna5 char literal.
 * \relates seqan3::rna5
 * \returns seqan3::rna5
 */
constexpr rna5 operator""_rna5(char const c) noexcept
{
    return rna5{}.assign_char(c);
}

/*!\brief The seqan3::rna5 string literal.
 * \relates seqan3::rna5
 * \returns seqan3::rna5_vector
 *
 * You can use this string literal to easily assign to rna5_vector:
 *
 * \snippet test/snippet/alphabet/nucleotide/rna5.cpp operator""_rna5
 *
 */
inline rna5_vector operator""_rna5(char const * s, std::size_t n)
{
    rna5_vector r;
    r.resize(n);

    for (size_t i = 0; i < n; ++i)
        r[i].assign_char(s[i]);

    return r;
}
//!\}

// ------------------------------------------------------------------
// rna5 (deferred definition)
// ------------------------------------------------------------------

constexpr std::array<rna5, rna5::value_size> rna5::complement_table
{
    'U'_rna5,    // complement of 'A'_rna5
    'G'_rna5,    // complement of 'C'_rna5
    'C'_rna5,    // complement of 'G'_rna5
    'N'_rna5,    // complement of 'N'_rna5
    'A'_rna5     // complement of 'U'_rna5
};

} // namespace seqan3
