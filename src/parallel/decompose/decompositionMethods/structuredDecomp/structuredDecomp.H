/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2012 OpenFOAM Foundation
    Copyright (C) 2018 OpenCFD Ltd.
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
    Foam::structuredDecomp

Description
    Walk out decomposition of patch cells mesh - selectable as \c structured.

SourceFiles
    structuredDecomp.C

\*---------------------------------------------------------------------------*/

#ifndef structuredDecomp_H
#define structuredDecomp_H

#include "decompositionMethod.H"
#include "wordRes.H"

namespace Foam
{

/*---------------------------------------------------------------------------*\
                      Class structuredDecomp Declaration
\*---------------------------------------------------------------------------*/

class structuredDecomp
:
    public decompositionMethod
{
    // Private data

        dictionary methodDict_;

        wordRes patches_;

        autoPtr<decompositionMethod> method_;


    // Private Member Functions

        //- No copy construct
        structuredDecomp(const structuredDecomp&) = delete;

        //- No copy assignment
        void operator=(const structuredDecomp&) = delete;


public:

    //- Runtime type information
    TypeName("structured");


    // Constructors

        //- Construct given the decomposition dictionary
        structuredDecomp(const dictionary& decompDict);


    //- Destructor
    virtual ~structuredDecomp() = default;


    // Member Functions

        //- Is method parallel aware
        //  (i.e. does it synchronize domains across proc boundaries)
        virtual bool parallelAware() const;

        //- Return for every coordinate the wanted processor number.
        //  Use the mesh connectivity (if needed)
        virtual labelList decompose
        (
            const polyMesh& mesh,
            const pointField& points,
            const scalarField& pointWeights
        ) const;

        //- Return for every coordinate the wanted processor number.
        //  Explicitly provided connectivity - does not use mesh_.
        virtual labelList decompose
        (
            const labelListList& globalCellCells,
            const pointField& cc,
            const scalarField& cWeights
        ) const
        {
            NotImplemented;
            return labelList();
        }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
