/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011-2013 OpenFOAM Foundation
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

Application
    postCalc

Description
    Generic wrapper for calculating a quantity at each time

\*---------------------------------------------------------------------------*/

#include "argList.H"
#include "Time.H"
#include "fvMesh.H"

#include "interpolationCellPoint.H"

#include "timeSelector.H"
#include "fvCFD.H"
// #include "singlePhaseTransportModel.H"

#include "boxFilter.H"
// #include "GaussianFilter.H"
#include "constructFilter.H"
#include "convolution.H"

// #include "meshSearch.H"

// #include "vectorList.H"

// #include "CPCCellToCellStencil.H"	

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
		
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{	

	Foam::timeSelector::addOptions();
	#   include "addRegionOption.H"
	Foam::argList::addBoolOption
	(
	"noWrite",
	"suppress writing results"
	);
    Foam::argList::addBoolOption
    (
    "channel",
    "add extra filtered fields for channel flow"
    );

	#include "addDictOption.H"

	#include "setRootCase.H"
	#include "createTime.H"
	Foam::instantList timeDirs = Foam::timeSelector::select0(runTime, args);
	#include "createNamedMesh.H"

	#include "createFields.H" 	 
	
        // Post-processing dictionary
        #include "postProcessingDict.H"
	
	// Create box filters
	//boxFilter lesFilters(mesh,false);

	
        autoPtr<constructFilter> lesFilters
        (
                constructFilter::New(mesh,postProcessingDict)
        );
	

	//Define convolution kernel
	convolution convKernel(mesh,lesFilters,readFilteredVars);	

	// Create output folder	
	if( !isDir(mesh.time().path()/outputRelativePath ) )
	{
		mkDir(mesh.time().path()/outputRelativePath );
	}
    Info << "path " << mesh.time().path()/outputRelativePath << endl;

	Info << " timeDirs.size() = " << timeDirs.size() << endl;
	Info << " filterWidth.size() = " << filterWidth.size() << endl;
				
		
	//-Time loop
	forAll(timeDirs, timeI)
	{
		runTime.setTime(timeDirs[timeI], timeI);

		Foam::Info << " " << endl;
		Foam::Info << "Time = " << runTime.timeName() << Foam::endl;

		mesh.readUpdate();
                    		
		// Read Eulerian variables
		#include "readEulerianVar.H"

        Info << tab << "Coarsen Eulerian vars" << endl;
		  forAll(filterWidth,fWidth)
		  {
		    //- Create name for filter width			  
		    char charName[100];
		    sprintf(charName, "%dX%dX%d",filterWidth[fWidth],filterWidth[fWidth],filterWidth[fWidth]);
		    word filterWidthName(charName);
			
		    // Fluid coarsening
		    #include "fluidCoarsening.H"
		    		    
		  } 	
	
	}
	//-Write outputs 
	//#include "output.H"	


	Foam::Info	<<  " " << Foam::endl;    
	Foam::Info	<< "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
        		<< "  ClockTime = " << runTime.elapsedClockTime() << " s"
        		<< nl << Foam::endl;

	Foam::Info<< "End\n" << Foam::endl;
	
	

}


// ************************************************************************* //
