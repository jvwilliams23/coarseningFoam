/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011-2017 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
July 2019: Bruno Santos @ FSD blueCAPE Lda
           - created function object based on OpenFOAM's cloudInfo
-------------------------------------------------------------------------------
License
    This file is derivative work of OpenFOAM.

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
    Foam::IOFilteredProps

Description
    Helper IO class to read and write particle positions in the old format.

SourceFiles
    IOFilteredProps.C

\*---------------------------------------------------------------------------*/

#ifndef IOFilteredProps_H
#define IOFilteredProps_H

// #include "fvMesh.H"
#include "polyMesh.H"
#include "regIOobject.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                         Class IOFilteredProps Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class IOFilteredProps
:
    public regIOobject
{

    // Private data

        //- Reference to the cloud
        const CloudType& cloud_;
        const word fieldName_;
        const word fieldType_;

public:

    // Static data

        //- Runtime type name information. Use cloud type.
        virtual const word& type() const
        {
            return Cloud<typename CloudType::particleType>::typeName;
        }


    // Constructors

        //- Construct from cloud
        IOFilteredProps(const CloudType&, const word, const word, const word);


    // Member functions

        virtual bool write(const bool valid = true) const;

        virtual bool writeData(Ostream& os) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "IOFilteredProps.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
