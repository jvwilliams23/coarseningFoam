// Post-processing dictionary 
IOdictionary coarseningProperties
(
    IOobject
    (
        //"EECoarseningDragPostProcessing",
        "coarseningPostProcessing",
        mesh.time().constant(),
        mesh,
        IOobject::MUST_READ,
        IOobject::NO_WRITE
    )
);

// Define coarseningProperties dictionary
const dictionary dict(coarseningProperties);

// Define postprocessing dictionary
const dictionary postProcessingDict(dict.subDict("postProcessingProps"));			

// Read particle & gas coarseningProperties
scalar rhoFluid(readScalar(postProcessingDict.lookup("rhoFluid")));
scalar muFluid(readScalar(postProcessingDict.lookup("muFluid")));

// Fluid coarsening
// bool fluidCoarsening(false);
bool readFilteredVars(false);
labelList filterWidth(1,1);
dictionary fluidCoarseningDict;
label nFilter(0);
if(postProcessingDict.found("fluidCoarsening")) 
{
	// fluidCoarsening = true;
	Info << "\nFluid coarsening will be performed" << endl;
	fluidCoarseningDict = postProcessingDict.subDict("fluidCoarsening");
	filterWidth = labelList(fluidCoarseningDict.lookup("filterWidth"));
	nFilter = filterWidth.size();
	if(fluidCoarseningDict.found("readFilteredVars")) readFilteredVars = true;
}

// Output folder
fileName outputRelativePath(postProcessingDict.lookup("outputRelativePath"));
