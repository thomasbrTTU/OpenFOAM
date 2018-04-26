/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2018 OpenCFD Ltd.
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Class
    Foam::bitSet

Description
    A bitSet stores bits (elements with only two states) in packed internal
    format and supports a variety of bit-set operations.
    Its behaviour is largely list-like, with some HashSet features.

SourceFiles
    bitSetI.H
    bitSet.C
    bitSetIO.C
    bitSetTemplates.C

See also
    Foam::BitOps
    Foam::PackedList

\*---------------------------------------------------------------------------*/

#ifndef bitSet_H
#define bitSet_H

#include "className.H"
#include "PackedList.H"
#include "UIndirectList.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// Forward declarations
class bitSet;
class labelRange;

/*---------------------------------------------------------------------------*\
                           Class bitSet Declaration
\*---------------------------------------------------------------------------*/

class bitSet
:
    public PackedList<1>
{
protected:

    // Protected Member Functions

        //- Write as a dictionary entry
        void writeEntry(Ostream& os) const;

    // Logic/Set Operations

        //- The set difference
        //  \code
        //      A = (A - B)
        //      A = (A & !B)
        //      A = (A & ~B)
        //  \endcode
        //  A and B can have different sizes.
        //  Does not change the original set size.
        bitSet& minusEq(const bitSet& other);

        //- The set logical AND
        //  \code
        //      A = (A & B)
        //
        //  \endcode
        //  A and B can have different sizes..
        //  Does not change the original set size.
        bitSet& andEq(const bitSet& other);

        //- The set logical OR
        //  \code
        //      A = (A | B)
        //  \endcode
        //  A and B can have different sizes
        //
        //  \note
        //      The default (strict=true) ignores additional length from B,
        //      whereas (strict=false) permits the set to automatically grow
        //      to accommodate additional elements arising from B.
        bitSet& orEq(const bitSet& other, const bool strict=true);

        //- The set logical XOR
        //  \code
        //      A = (A ^ B)
        //  \endcode
        //  A and B can have different sizes. Sizing behaviour as per orEq.
        bitSet& xorEq(const bitSet& other, const bool strict=true);

public:

    // Forward declaration of access classes

        class reference;
        class const_iterator;
        typedef unsigned int const_reference;

    //- Define class name and debug
    ClassName("bitSet");


    // Constructors

        //- Construct an empty, zero-sized set
        inline constexpr bitSet() noexcept;

        //- Construct from Istream
        explicit bitSet(Istream& is);

        //- Construct with given size, with all bits set to 0
        explicit inline bitSet(const label n);

        //- Construct with given size and value for all elements
        inline bitSet(const label n, const bool val);

        //- Copy construct
        inline bitSet(const bitSet& bitset);

        //- Move construct
        inline bitSet(bitSet&& bitset);

        //- Construct from a list of bools
        explicit inline bitSet(const UList<bool>& bools);

        //- Construct with given size with all bits set to 0,
        //- subsequently add specified locations as 1.
        inline bitSet(const label n, const labelUList& locations);

        //- Construct with given size with all bits set to 0,
        //- subsequently add specified locations as 1.
        inline bitSet(const label n, const labelUIndList& locations);

        //- Construct with given size with all bits set to 0,
        //- subsequently add specified locations as 1.
        inline bitSet(const label n, std::initializer_list<label> locations);

        //- Construct with automatic sizing (filled with 0),
        //- and populate with specified locations as 1.
        explicit inline bitSet(const labelUList& locations);

        //- Construct with automatic sizing (filled with 0),
        //- and populate with specified locations as 1.
        explicit inline bitSet(const labelUIndList& locations);

        //- Clone
        inline autoPtr<bitSet> clone() const;


    // Member Functions

    // Query

        //- True if all bits in this bitset are set or if the set is empty.
        inline bool all() const;

        //- True if any bits in this bitset are set.
        inline bool any() const;

        //- True if no bits in this bitset are set.
        inline bool none() const;

        //- True if there are two or more entries and all entries have
        //- identical values.
        inline bool uniform() const;

        //- Count number of bits set.
        inline unsigned int count() const;

        //- True if any bits in the other bitset intersect (are the same).
        //
        //  \note Method name compatibility with boost::dynamic_bitset
        bool intersects(const bitSet& other) const;

        //- Test value at specified position, never auto-vivify entries.
        //
        //  \note Method name compatibility with std::bitset
        inline bool test(const label pos) const;

        //- Locate the first bit set.
        //  \return the location or -1 if there are no bits set.
        //
        //  \note Method name compatibility with boost::dynamic_bitset
        inline label find_first() const;

        //- Locate the last bit set.
        //  \return the location or -1 if there are no bits set.
        //
        //  \note Provided for symmetry with find_first()
        inline label find_last() const;

        //- Locate the next bit set, starting one beyond the specified position
        //  \return the location or -1 if there are no further bits set.
        //
        //  \note Method name compatibility with boost::dynamic_bitset
        inline label find_next(label pos) const;

        //- The indices of the \a on bits as a sorted labelList.
        //
        //  \note Method name compatibility with HashSet
        labelList toc() const;

        //- The indices of the \a on bits as a sorted labelList.
        //  This is identical to toc(), which is always sorted.
        //
        //  \note Method name compatibility with HashSet
        inline labelList sortedToc() const;

        //- Return the bitset values as a boolList.
        List<bool> values() const;


    // Assignment

        //- Assign all entries to the given value.
        inline void assign(const bool val);

        //- Copy assign all entries from a list of bools.
        void assign(const UList<bool>& bools);


    // Setting single or multiple values

        //- Single index/value assign
        using PackedList<1>::set;

        //- Set specified bits from another bitset.
        //  The current set size may grow to accommodate any new bits
        //  (auto-vivifies).
        inline void set(const bitSet& bitset);

        //- Set the specified range of bits specified
        //  The current set size may grow to accommodate any new bits
        //  (auto-vivifies).
        //  \note this operation is generally more efficient than calling
        //      set(pos) on individual bits.
        void set(const labelRange& range);


    // Unsetting single or multiple values

        //- Unset a single index
        using PackedList<1>::unset;

        //- Unset (subtract) the bits specified in the other bitset, which is
        //- a set difference corresponds to the logical operation
        //  \code
        //      A = (A & !B)
        //  \endcode
        //  The result is comparable to 'operator-='
        //  \endcode
        //
        //  A and B can have different sizes.
        //  Does not change the original set size.
        inline bitSet& unset(const bitSet& other);

        //- Unset the specified range of bits specified, never auto-vivifies.
        //  \note this operation can be more efficient than calling
        //      unset(pos) on individual bits.
        void unset(const labelRange& range);


    // Edit

        //- Invert all bits in the addressable region
        inline void flip();

        //- Invert bits at the specified position.
        //  A no-op if the position is out-of-range
        inline void flip(const label pos);

        //- Swap contents
        inline void swap(bitSet& bitset);

        //- Transfer the contents of the argument list into this list
        //- and annul the argument list.
        inline void transfer(bitSet& bitset);


    // Convenience methods

        //- Ensure the addressable range does not exceed maxSize.
        //  Either decreases the size of the bitSet or is a no-op.
        //
        //  \code
        //      pointField& pts = mesh.points();
        //      bitset.bound(pts.size());
        //
        //      for (const label pointi : bitset)
        //      {
        //          pts[pointi]  ...
        //      }
        //  \endcode
        inline bitSet& bound(const label maxSize);

        //- Ensure the addressable range does not exceed that of other.
        //  Either decreases the size of the bitSet or is a no-op.
        inline bitSet& bound(const bitSet& other);

        //- Ensure that minSize is covered by the bitSet.
        //  Either increases the size of the bitSet or is a no-op.
        inline bitSet& extend(const label minSize);

        //- Ensure the bitset is addressable throughout the range of other.
        //  Either increases the size of the bitSet or is a no-op.
        inline bitSet& extend(const bitSet& other);

        //- Set the listed locations to true.
        //  Does auto-vivify for non-existent entries.
        inline void setMany(const labelUList& locations);

        //- Set the listed locations to true.
        //  Does auto-vivify for non-existent entries.
        inline void setMany(const labelUIndList& locations);

        //- Set the locations listed by the iterator range,
        //  auto-vivify entries if needed.
        template<class InputIter>
        void setMany(InputIter first, InputIter last);

        //- Unset the listed locations, never auto-vivifies.
        inline void unsetMany(const labelUList& locations);

        //- Unset the listed locations, never auto-vivifies.
        inline void unsetMany(const labelUIndList& locations);

        //- Unset the locations listed by the iterator range,
        //- never auto-vivify entries.
        template<class InputIter>
        void unsetMany(InputIter first, InputIter last);


    // Access helpers

        //- A reference supporting read/write access to an entry
        class reference
        :
            public PackedList<1>::reference
        {
        protected:
            friend class bitSet;        // Access for parent
            void operator&() = delete;  // Refuse to provide its address

            //- Construct by taking reference of block from within
            //- the list and the specified index.
            inline reference(bitSet* parent, const label index);

        public:

            //- Flip the bit at the position, no range-checking
            inline void flip();

            //- Value assignment
            inline void operator=(const reference& other);

            //- Value assignment
            inline void operator=(const unsigned int val);

            //- Conversion operator
            inline operator unsigned int () const;
        };


    // Iteration

        //- A const_iterator for iterating across \a on values
        class const_iterator
        {
            friend class bitSet;

            //- The parent being iterated
            const bitSet* set_;

            //- Global position of the current \a on bit
            label pos_;

            //- Construct null - an end iterator
            inline const_iterator() noexcept;

            //- Construct begin iterator
            inline const_iterator(const bitSet* bitset);

        public:

            //- Return the current \a on position
            inline label operator*() const noexcept;

            //- Move to the next \a on position
            inline const_iterator& operator++();

            inline bool operator==(const const_iterator& iter) const noexcept;
            inline bool operator!=(const const_iterator& iter) const noexcept;
        };


        //- Iterator set to the position of the first \a on bit
        inline const_iterator begin() const;

        //- Iterator set to the position of the first \a on bit
        inline const_iterator cbegin() const;

        //- Iterator beyond the end of the bitSet
        inline const_iterator end() const noexcept;

        //- Iterator beyond the end of the bitSet
        inline const_iterator cend() const noexcept;


    // Member Operators

        //- Identical to get() - get value at index.
        //  Never auto-vivify entries.
        inline unsigned int operator[](const label i) const;

        //- Non-const access to value at index.
        //  Fatal for out-of-range indices
        inline reference operator[](const label i);

        //- Assignment of all entries to the given value.
        inline bitSet& operator=(const bool val);

        //- Copy assignment
        inline bitSet& operator=(const bitSet& bitset);

        //- Move assignment
        inline bitSet& operator=(bitSet&& bitset);

        //- Complement operator.
        //  Return a copy of the existing set with all its bits flipped.
        inline bitSet operator~() const;

        //- Bitwise-AND all the bits in other with the bits in this bitset.
        //  The operands may have dissimilar sizes without affecting the size
        //  of the set.
        inline bitSet& operator&=(const bitSet& other);

        //- Bitwise-OR operator - similar to the set() method.
        //  The operands may have dissimilar sizes without affecting the size
        //  of the set.
        inline bitSet& operator|=(const bitSet& other);

        //- Bitwise-XOR operator - retains unique entries.
        //  The operands may have dissimilar sizes without affecting the size
        //  of the set.
        inline bitSet& operator^=(const bitSet& other);

        //- Remove entries from this list - identical to the unset() method.
        //  The operands may have dissimilar sizes without affecting the size
        //  of the set.
        inline bitSet& operator-=(const bitSet& other);


    // IO

        //- Write the bitSet, with line-breaks in ASCII if the size
        //- exceeds shortListLen.
        //  Using '0' suppresses line-breaks entirely.
        Ostream& writeList(Ostream& os, const label shortListLen=0) const;

        //- Write as a dictionary entry with keyword
        void writeEntry(const word& keyword, Ostream& os) const;


    // IOstream Operators

        //- Return info proxy
        InfoProxy<bitSet> info() const
        {
            return *this;
        }

        friend Ostream& operator<<
        (
            Ostream& os,
            const InfoProxy<bitSet>& info
        );

        friend Ostream& operator<<
        (
            Ostream& os,
            const bitSet& bitset
        );
};


// Global Operators

//- Bitwise-AND of two bitsets.
//  See bitSet::operator&= for more details.
inline bitSet operator&(const bitSet& a, const bitSet& b);

//- Bitwise-OR of two bitsets
//  See bitSet::operator|= for more details.
inline bitSet operator|(const bitSet& a, const bitSet& b);

//- Bitwise-XOR of two bitsets to form a unique bit-set
//  See bitSet::operator^= for more details.
inline bitSet operator^(const bitSet& a, const bitSet& b);

//- Bitwise difference (subset) of two bitsets to form a unique bit-set
//  See bitSet::operator-= for more details.
inline bitSet operator-(const bitSet& a, const bitSet& b);


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "bitSetI.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "bitSetTemplates.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //