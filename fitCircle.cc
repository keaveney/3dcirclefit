#include "TCanvas.h"
#include "TRandom3.h"
#include "TGraph.h"
#include "TMath.h"
#include "TArc.h"
#include "TVector3.h"
#include "TPolyMarker3D.h"
#include "TFile.h"
#include "TRandom3.h"
#include "TVector3.h"
#include "TH3F.h"
#include "Math/Functor.h"

#include "Fit/Fitter.h"


//____________________________________________________________________
//void  fitCircle(Int_t n=500) {
int  main() {
  //generates n points around a circle and fit them
 // TCanvas *c1 = new TCanvas("c1","c1",600,600);
 // c1->SetGrid();    

  int n = 1000;
    
  TCanvas *c1 = new TCanvas( "", "", 300, 10, 700, 500 );

  std::vector<TVector3*> points;
  TPolyMarker3D *pm3d = new TPolyMarker3D(n);
  TH3F *frame3d = new TH3F("frame3d","frame3d",10,-9,9,10,-9,9,10,-9,9);
  frame3d->Draw();
    
  TRandom3 rand;
  Double_t x,y,z;
  Double_t r = 5.0;
  Double_t u, v, w, dr;
  TVector3 * point;

   
  for (Int_t i=0;i<n;i++) {
  //  r.Circle(x,y,r.Gaus(4,0.1));
    rand.Circle(x,z,r);
    y = (1.0);
    point = new TVector3();
    point->SetXYZ(x,y,z);
    points.push_back(point);
    pm3d->SetPoint( i, x, y, z );
    //std::cout <<"X = "<< x <<" Y = "<< y << " Z = " << z<< std::endl;
    //delete point;
  }
    
 // c1->DrawFrame(-5,-5,5,5);
 
  auto chi2Function = [&](const Double_t *par){
    //minimisation function computing the sum of squares of residuals
    // looping at the graph points
    Double_t f = 0;

      std::cout <<"  "<<  std::endl;
      std::cout <<" par "<<  par[0]<<  std::endl;
      
    for (Int_t i=0;i<n;i++) {
    // apply two orthogonal rotation transformations to points (theta, phi)
        u = points[i]->X() - par[0];
        v = points[i]->Z() - par[1];
        w = points[i]->Y() - par[2];
       dr = par[3] - std::sqrt((u*u)+(v*v)+(w*w));
    //f += dr*dr;
       // dr = std::sqrt((u*u)+(v*v));
        f += dr*dr;
    }
    return f;
  };
    
  // wrap chi2 funciton in a function object for the fit
  // 3 is the number of fit parameters (size of array par)
  ROOT::Math::Functor fcn(chi2Function,4);
  ROOT::Fit::Fitter  fitter;
//  double pStart[4] = {0.0,0.0,0.0,1.0};
  double pStart[4] = {0.0, 0.0, 0.0, 1.0};

  fitter.SetFCN(fcn, pStart);
  fitter.Config().ParSettings(0).SetName("x0");
  fitter.Config().ParSettings(1).SetName("z0");
  fitter.Config().ParSettings(2).SetName("y0");
  fitter.Config().ParSettings(3).SetName("R");

  // do the fit 
  bool ok = fitter.FitFCN();
  if (!ok) {
    Error("line3Dfit","Line3D Fit failed");
  }   

  const ROOT::Fit::FitResult & result = fitter.Result();
  result.Print(std::cout);
  //Draw the circle on top of the points
    TArc *arc = new TArc(result.Parameter(0),result.Parameter(1),r);
    arc->SetLineColor(kBlue);
    arc->SetLineWidth(1);
    pm3d->Draw("p");
    pm3d->SetMarkerSize( 0.5 );
    pm3d->SetMarkerColor( 2  );
    pm3d->SetMarkerStyle( 3 );
    pm3d->Draw();
    //arc->Draw("same");
    c1->SaveAs("fit.pdf");
    
    TFile * f = new TFile("fit.root","RECREATE");
    c1->Write();
    f->Close();
    

    return 1;

}
