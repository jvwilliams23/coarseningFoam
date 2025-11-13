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

\*---------------------------------------------------------------------------*/

#include "IOFilteredProps.H"

#include "volFields.H"
#include "meshTools.H"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CloudType>
Foam::IOFilteredProps<CloudType>::IOFilteredProps(
    const CloudType& c, const word fWord, const word outWord, const word fType
)
:
    regIOobject
    (
        IOobject
        (
            IOobject::groupName(outWord, "filtered"),
            c.time().timeName(),
            c,
            IOobject::MUST_READ,
            IOobject::NO_WRITE
        )
    ),
    fieldName_(fWord),
    fieldType_(fType),
    cloud_(c)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
bool Foam::IOFilteredProps<CloudType>::write(const bool valid) const
{
    return regIOobject::write(cloud_.size());
}


template<class CloudType>
bool Foam::IOFilteredProps<CloudType>::writeData(Ostream& os) const
{

    const polyMesh& pmesh = cloud_.pMesh();

    //Info << "mesh " << mesh_ << endl;
    // FatalIOError.throwExceptions();

    Info << "Reading " << fieldName_ << endl;

    if (fieldType_ == "scalar")
    {
        const volScalarField psi = pmesh.lookupObject<volScalarField>(
                fieldName_);
        os  << cloud_.size() << nl << token::BEGIN_LIST << nl;

        forAllConstIter(typename CloudType, cloud_, iter)
        {
            os  << psi[iter().cell()]
                << nl;
        }

        os  << token::END_LIST << endl;

    }
    else if (fieldType_ == "vector")
    {
        const volVectorField psi = pmesh.lookupObject<volVectorField>(
                fieldName_);
        os  << cloud_.size() << nl << token::BEGIN_LIST << nl;

        forAllConstIter(typename CloudType, cloud_, iter)
        {
            os  << psi[iter().cell()]
                << nl;
        }

        os  << token::END_LIST << endl;

    }
    else if (fieldType_ == "tensor")
    {
        const volTensorField psi = pmesh.lookupObject<volTensorField>(
            fieldName_);
        os  << cloud_.size() << nl << token::BEGIN_LIST << nl;

        forAllConstIter(typename CloudType, cloud_, iter)
        {
            os  << psi[iter().cell()]
                << nl;
        }

        os  << token::END_LIST << endl;
    }

    return os.good();
}




// ************************************************************************* //
