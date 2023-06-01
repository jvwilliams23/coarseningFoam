    Info<< "Creating field p\n" << endl;
    volScalarField p
    (
        IOobject
        (
            "p",
            runTime.timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh,
	dimensionedScalar("zero",dimensionSet(1, -1, -2, 0, 0), scalar(0))
    );

    Info<< "Creating gas velocity field U" << endl;
    volVectorField U
    (
        IOobject
        (
            "U",
            runTime.timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh,
        dimensionedVector("zero",dimensionSet(0, 1, -1, 0, 0),vector(0,0,0))
    );

// - Dummy turbulence model 
//
    Info<< "Reading/calculating face flux field phi\n" << endl;
    surfaceScalarField phi
     (
         IOobject
         (
            "phi",
            runTime.timeName(),
            mesh,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
         ),
         linearInterpolate(U) & mesh.Sf()
     );

/*    singlePhaseTransportModel laminarTransport(U, phi);

    autoPtr<incompressible::turbulenceModel> turbulence
    (
        incompressible::turbulenceModel::New(U, phi, laminarTransport)
    );
*/

//
// - Filtered variables
//
    volVectorField tildeUf
    (
        IOobject
        (
            "tildeU.air",
            runTime.timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh,
	dimensionedVector("zero", dimensionSet(0, 1, -1, 0, 0), vector(0,0,0))
    );
    //tildeUf.write();    
    volTensorField tildeGradUf
    (
        IOobject
        (
            "tildeGradU.air",
            runTime.timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh,
    dimensionedTensor("zero", dimensionSet(0, 0, -1, 0, 0), tensor(0,0,0,0,0,0,0,0,0))
    );




