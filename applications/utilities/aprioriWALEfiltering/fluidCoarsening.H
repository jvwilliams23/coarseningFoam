// Filtering gas velocity
tildeUf = convKernel.coarseningVec("tildeUf",filterWidth[fWidth],U);
volTensorField tildeGradUf
(
	 IOobject
	 (
	  "gradTildeUf"+filterWidthName,
	  runTime.timeName(),
	  mesh,
	  IOobject::NO_READ,
	  IOobject::AUTO_WRITE
	 ),
	 fvc::grad(tildeUf)
);
tildeGradUf.write();
volTensorField gradUfPrime
(
     IOobject
     (
      "gradUfPrime"+filterWidthName,
      runTime.timeName(),
      mesh,
      IOobject::NO_READ,
      IOobject::AUTO_WRITE
     ),
     fvc::grad(U) - tildeGradUf
);
gradUfPrime.write();

if ( args.optionFound("channel") )
{

    volScalarField tildeP = convKernel.coarseningSca("tildeP",filterWidth[fWidth],p);

    //volVectorField gradTildeP = fvc::grad(tildeP);
    volVectorField gradTildeP
        (
         IOobject
         (
          "gradTildeP"+filterWidthName,
          runTime.timeName(),
          mesh,
          IOobject::NO_READ,
          IOobject::AUTO_WRITE
         ),
         fvc::grad(tildeP)
        );
    gradTildeP.write();

    volVectorField lapTildeUf
    (
         IOobject
         (
          "laplacianTildeUf"+filterWidthName,
          runTime.timeName(),
          mesh,
          IOobject::NO_READ,
          IOobject::AUTO_WRITE
         ),
         fvc::laplacian(tildeUf)
    );
    lapTildeUf.write();

}

volScalarField magSqrSd(magSqr(dev(symm(tildeGradUf & tildeGradUf))));

const scalar Ck_ = 0.094;
const scalar Cw_ = 0.325;
const scalar Ce_ = 1.048;

Info << tab << "Calculating k" << endl;

volScalarField k
(
     IOobject
     (
      "k"+filterWidthName,
      runTime.timeName(),
      mesh,
      IOobject::NO_READ,
      IOobject::AUTO_WRITE
      ),
     sqr(sqr(Cw_)*delta*filterWidth[fWidth]/Ck_)*
            (
                pow3(magSqrSd)
               /(
                   sqr
                   (
                       pow(magSqr(symm(tildeGradUf)), 5.0/2.0)
                     + pow(magSqrSd, 5.0/4.0)
                   )
                 + dimensionedScalar
                   (
                       "small",
                       dimensionSet(0, 0, -10, 0, 0),
                       SMALL
                   )
               )
            )
);
k.write();

Info << tab << "Calculating epsilon" << endl;

volScalarField epsilon
(
     IOobject
     (
      "epsilon"+filterWidthName,
      runTime.timeName(),
      mesh,
      IOobject::NO_READ,
      IOobject::AUTO_WRITE
      ),
    Ce_*k*sqrt(mag(k))/(delta*filterWidth[fWidth])
);
epsilon.write();

Info << tab << "Finished writing subgrid properties" << endl;

