/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2012-2016 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
July 2019: Bruno Santos @ FSD blueCAPE Lda
           - created function object based on OpenFOAM's cloudInfo
November 2019: Bruno Santos @ FSD blueCAPE Lda
           - added support for MPPIC clouds
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

#include "writeCloudFilteredProps.H"

#include "basicKinematicCollidingCloud.H"
#include "basicKinematicCollidingParcel.H"

#include "basicKinematicMPPICCloud.H"
#include "basicKinematicMPPICParcel.H"

#include "IOFilteredProps.H"
#include "dictionary.H"
//#include "PstreamReduceOps.T.H"
#include "PstreamReduceOps.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
namespace functionObjects
{
    defineTypeNameAndDebug(writeCloudFilteredProps, 0);

    addToRunTimeSelectionTable
    (
        functionObject,
        writeCloudFilteredProps,
        dictionary
    );
}
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::functionObjects::writeCloudFilteredProps::writeCloudFilteredProps
(
    const word& name,
    const Time& runTime,
    const dictionary& dict
)
:
    regionFunctionObject(name, runTime, dict)
{
    read(dict);
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::functionObjects::writeCloudFilteredProps::~writeCloudFilteredProps()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool Foam::functionObjects::writeCloudFilteredProps::read(const dictionary& dict)
{
    regionFunctionObject::read(dict);

    names = wordList(dict.lookup("clouds"));
    filters_ = wordList(dict.lookup("filterWidth"));

    Info<< type() << " " << name() << ": ";
    if (names.size())
    {
        Info<< "applying to clouds:" << nl;
        forAll(names, i)
        {
            Info<< "    " << names[i] << nl;
        }
        Info<< endl;
    }
    else
    {
        Info<< "no clouds to be processed" << nl << endl;
    }

    return true;
}


bool Foam::functionObjects::writeCloudFilteredProps::execute()
{
    return true;
}


bool Foam::functionObjects::writeCloudFilteredProps::write()
{
    forAll(names, i)
    {
        forAll(filters_, i)
        {
            const word& cloudName = names[i];
            const word& filterWidth = filters_[i];
            //char charfPhi[100];
            //sprintf(charfPhi, "%dX%dX%d",filterWidth,filterWidth,filterWidth);
            word charfPhi = filterWidth+"X"+filterWidth+"X"+filterWidth;


            const kinematicCloud& kcloud =
                obr_.lookupObject<kinematicCloud>(cloudName);
            // declare field names
            /*word epsilonName = "epsilon";
            word tkeName = "k";
            word UcName = "U";*/
            word epsilonName = "epsilon" + charfPhi;
            word tkeName = "k" + charfPhi;
            word UcName = "tildeUf" + charfPhi;

            word epsilonNameOut = "epsilon";
            word tkeNameOut = "k";
            word UcNameOut = "Uc";
            // add string for each filter width
            epsilonNameOut = epsilonNameOut+charfPhi;
            tkeNameOut = tkeNameOut+charfPhi;
            UcNameOut = UcNameOut+charfPhi;

            const word scalWord = "scalar";
            const word vecWord = "vector";

            Info << "Reading field for " << UcName << endl;


            if (isA<basicKinematicCollidingCloud>(kcloud))
            {
                const basicKinematicCollidingCloud& cloud =
                    dynamic_cast<const basicKinematicCollidingCloud&>(kcloud);

                IOFilteredProps<basicKinematicCollidingCloud> ioFPeps(cloud, epsilonName, epsilonNameOut, scalWord);
                ioFPeps.write(cloud.nParcels() > 0);
                IOFilteredProps<basicKinematicCollidingCloud> ioFPtke(cloud, tkeName, tkeNameOut, scalWord);
                ioFPtke.write(cloud.nParcels() > 0);
                IOFilteredProps<basicKinematicCollidingCloud> ioFPuc(cloud, UcName, UcNameOut, vecWord);
                ioFPuc.write(cloud.nParcels() > 0);

                /*
                //If you want direct control over the output, here is an example to
                //write to screen from all processors

                forAllConstIter(basicKinematicCollidingCloud, cloud, pIter)
                {
                    const basicKinematicCollidingParcel& p = pIter();

                    Pout << "p:" << p.position() << endl;
                }
                */
            }
            else if (isA<basicKinematicMPPICCloud>(kcloud))
            {
                const basicKinematicMPPICCloud& cloud =
                    dynamic_cast<const basicKinematicMPPICCloud&>(kcloud);

                Info << "Epsilon" << tab << epsilonName << tab << epsilonNameOut << endl;
                IOFilteredProps<basicKinematicMPPICCloud> ioFPeps(cloud, epsilonName, epsilonNameOut, scalWord);
                ioFPeps.write(cloud.nParcels() > 0);
                Info << "TKE" << tab << tkeName << tab << tkeNameOut << endl;
                IOFilteredProps<basicKinematicMPPICCloud> ioFPtke(cloud, tkeName, tkeNameOut, scalWord);
                ioFPtke.write(cloud.nParcels() > 0);
                Info << "Uc" << tab << UcName << tab << UcNameOut << endl;
                IOFilteredProps<basicKinematicMPPICCloud> ioFPuc(cloud, UcName, UcNameOut, vecWord);
                ioFPuc.write(cloud.nParcels() > 0);

                /*
                //If you want direct control over the output, here is an example to
                //write to screen from all processors

                forAllConstIter(basicKinematicCollidingCloud, cloud, pIter)
                {
                    const basicKinematicCollidingParcel& p = pIter();

                    Pout << "p:" << p.position() << endl;
                }
                */
            }
        }
    }

    return true;
}


// ************************************************************************* //
