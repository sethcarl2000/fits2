#include <TH1F.h> 
#include <TF1.h> 
#include <TCanvas.h> 
#include <functional> 
#include <TFitResultPtr.h> 
#include <TFitResult.h> 
#include <TFile.h> 
#include <cmath>
#include <TLegend.h> 
#include <vector> 
#include <TGraph.h> 
#include <TLine.h> 

using namespace std; 

double fcn(double *x, double *par)
{
    double height = fabs(par[0]);
    double mean   = par[1];
    double sigma  = fabs(par[2]); 

    return height * exp( -0.5*pow((x[0]-mean)/sigma, 2) );  
}

int fit_histo(const char* path_file, const char* histo_name)
{
    auto file = new TFile(path_file, "READ"); 
    auto hist = (TH1F*)file->Get(histo_name); 

    if (!file || !hist) {
        Error("fit_histo", "Unable to open file or hist"); 
        return -1; 
    }

    auto tf1 = new TF1("my_fcn", fcn, 
        hist->GetXaxis()->GetXmin(), 
        hist->GetXaxis()->GetXmax(), 
        3
    ); 

    double params[] = {80., 8., 2.};
    tf1->SetParameters(params); 

    auto c = new TCanvas("c", "fit", 1400, 500); 
    c->Divide(3,1); 

    c->cd(1);
    hist->SetStats(0); 
    hist->Draw("E");
    

    tf1->SetLineWidth(2);
    tf1->SetLineStyle(kDashed); 
    auto tf1_cpy1 = tf1->DrawCopy("SAME");

    auto frp = hist->Fit("my_fcn", "S Q N"); 

    tf1->SetLineWidth(2);
    tf1->SetLineStyle(kSolid); 
    auto tf1_cpy2 = tf1->DrawCopy("SAME");  

    
    auto legend = new TLegend(0.3, 0.7, 0.8, 0.9); 
    legend->AddEntry(hist, "Data"); 
    legend->AddEntry(tf1_cpy1, "First guess"); 
    legend->AddEntry(tf1_cpy2, "Best fit"); 

    legend->Draw(); 


    auto hist_resid = new TH1F("h_resid", "Residuals", 30, -10, 10); 
    vector<double> bins, residuals;
    auto ax = hist->GetXaxis();  
    for (int b=1; b<=ax->GetNbins(); b++) {

        const double x = ax->GetBinCenter(b); 
        
        bins     .push_back(x); 
        residuals.push_back( (hist->GetBinContent(b) - tf1->Eval(x))/hist->GetBinError(b) );
        hist_resid->Fill( residuals.back() ); 
    }

    c->cd(2); 
    auto graph = new TGraph(bins.size(), bins.data(), residuals.data());
    
    graph->SetTitle("Residuals");
    graph->Draw(); 

    auto line = new TLine(bins.front(),0., bins.back(),0.); 
    line->SetLineStyle(kDotted); 
    line->Draw("SAME"); 
    

    c->cd(3); 
    hist_resid->SetStats(1);
    hist_resid->SetTitle("Dist. of Residuals"); 
    hist_resid->Draw("HIST"); 

    printf(
        "Results of fit:\n"
        " sigma     - %-4.4f +/- %.4f\n"  
        " mean      - %-4.4f +/- %.4f\n"
        " height    - %-4.4f +/- %.4f\n"
        "\n"
        " Reduced chi^2     -   %-4.4f\n"
        " P-value:          -   %-4.4f\n",
        frp->Parameter(2), frp->ParError(2),
        frp->Parameter(1), frp->ParError(1),
        frp->Parameter(0), frp->ParError(0),
        frp->Chi2() / frp->Ndf(), 
        frp->Prob()
    );

    return 0; 
}