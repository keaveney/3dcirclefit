#include "TCanvas.h"
#include "TRandom3.h"
#include "TGraph.h"
#include "TMath.h"
#include "TArc.h"
#include "TVector3.h"
#include "TPolyMarker3D.h"
#include "TRandom3.h"
#include "TVector3.h"
#include "TH3F.h"
#include "Math/Functor.h"



#include "Fit/Fitter.h"
//____________________________________________________________________
//void  fitCircle(Int_t n=500) {
int  main(Int_t n=500) {
  //generates n points around a circle and fit them
 // TCanvas *c1 = new TCanvas("c1","c1",600,600);
 // c1->SetGrid();
    
    
  TCanvas *c1 = new TCanvas( "sky", "Tornado", 300, 10, 700, 500 );

  TVector3 * points[n];
  TPolyMarker3D *pm3d = new TPolyMarker3D(n);
  TH3F *frame3d = new TH3F("frame3d","frame3d",10,-9,9,10,-9,9,10,-9,9);
  frame3d->Draw();
    

  TRandom3 r;
  Double_t x,y,z;
  for (Int_t i=0;i<n;i++) {
    r.Circle(x,y,r.Gaus(4,0.1));
    z = (0.5*y);
    TVector3 * point = new TVector3(x,y,z);
    points[i] = point;
    pm3d->SetPoint( i, x, y, z );
      
      std::cout <<"X = "<< x <<" Y = "<< y << " Z = " << z<< std::endl;
      
  }
    
 // c1->DrawFrame(-5,-5,5,5);
 
  auto chi2Function = [&](const Double_t *par) {
    //minimisation function computing the sum of squares of residuals
    // looping at the graph points
    Double_t f = 0;

    for (Int_t i=0;i<n;i++) {
        
        
        
    // apply two orthogonal rotation transformations to points (theta, phi)
        
        
      Double_t u = points[i]->X() - par[0];
      Double_t v = points[i]->Y() - par[1];
      Double_t w = points[i]->Z() - par[2];
        
      Double_t dr = par[3] - std::sqrt((u*u)+(v*v)+(w*w));
      f += dr*dr;
    }
    return f;
  };
  // wrap chi2 funciton in a function object for the fit
  // 3 is the number of fit parameters (size of array par)
  ROOT::Math::Functor fcn(chi2Function,4);

  ROOT::Fit::Fitter  fitter;
  double pStart[4] = {0,0,0,3};
  fitter.SetFCN(fcn, pStart);
  fitter.Config().ParSettings(0).SetName("x0");
  fitter.Config().ParSettings(1).SetName("y0");
  fitter.Config().ParSettings(2).SetName("z0");
  fitter.Config().ParSettings(3).SetName("R");
  // do the fit 
  bool ok = fitter.FitFCN();
  if (!ok) {
    Error("line3Dfit","Line3D Fit failed");
  }   
  const ROOT::Fit::FitResult & result = fitter.Result();
  result.Print(std::cout);
  //Draw the circle on top of the points
//  TArc *arc = new TArc(result.Parameter(0),result.Parameter(1),result.Parameter(2));
 // arc->SetLineColor(kRed);
 // arc->SetLineWidth(4);
 // arc->Draw();
 // gr->Draw("p");
    pm3d->SetMarkerSize( 1 );
    pm3d->SetMarkerColor( 2  );
    pm3d->SetMarkerStyle( 3 );
    pm3d->Draw();


    return 1;

}
