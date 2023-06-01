volScalarField cellVolu
    (
    IOobject
       (
        "cellVolu",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE
       ),
        mesh,
    dimensionedScalar("zero", dimVolume, 0.0)
    );
    cellVolu.primitiveFieldRef() = mesh.V();

//volScalarField vols = mesh.V();

// we assume that delta is constant 
// as it is a requirement for our coarsening code
scalar maxVolume = max(cellVolu.internalField()).value();
scalar domainAvgDelta = Foam::pow(maxVolume, 1.0/3.0);
volScalarField delta
(
    IOobject
       (
        "delta",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE
       ),
        mesh,
    dimensionedScalar("zero", dimLength, domainAvgDelta)
);
//volScalarField delta = pow(cellVolu, 1.0/3.0);
Info << "Delta from cell vol is " << gAverage(delta) << tab << max(delta).value() << endl;

// RbDispersionAverageFieldsOnly.so"
Info<< tab <<"Reading U.air" << endl;
IOobject UmHeader
(
        "U.air",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ
);
volVectorField Um(UmHeader,mesh);

IOobject pHeader
(
        "p",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ
);
volScalarField p(pHeader,mesh);

// Insert into variables 
U = Um;

word continuousPhaseName
(
    IOdictionary
    (
        IOobject
        (
            "transportProperties",
            runTime.constant(),
            mesh,
            IOobject::MUST_READ
        )
    ).lookup("continuousPhaseName")
);

/*
singlePhaseTransportModel continuousPhaseTransport(U, phi);

Info << "Creating field alphac\n" << endl;
// alphac must be constructed before the cloud
// so that the drag-models can find it
volScalarField alphac
(
    IOobject
    (
        "alpha.air",
        runTime.timeName(),
        mesh,
        IOobject::READ_IF_PRESENT,
        IOobject::AUTO_WRITE
    ),
    mesh,
    dimensionedScalar("0", dimless, 0)
);

// Update alphac from the particle locations
alphac.correctBoundaryConditions();

surfaceScalarField alphacf("alphacf", fvc::interpolate(alphac));

surfaceScalarField alphaPhic
(
    "alphaPhi.air",
    alphacf*phi
);


singlePhaseTransportModel laminarTransport(U, phi);
autoPtr<incompressible::turbulenceModel> turbulence
(
        incompressible::turbulenceModel::New(U, phi, laminarTransport)
);
*/



