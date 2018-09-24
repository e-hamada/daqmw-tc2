#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <TApplication.h>
#include <TCanvas.h>
#include <TROOT.h>
#include <TFile.h>
#include <TH1.h>
#include <TRandom.h>
#include <TGraph.h>
#include <TText.h>

using namespace std;
int usage()
{
    cerr << "Usage: ./draw_graph" << std::endl;

    return 0;
}

int main(int argc, char *argv[])
{
    int max_data_points = 20;
    int n_points = 0;
    
    double x[max_data_points];
    double y[max_data_points];

    for (int i = 0; i < 10; i++) {
        x[i] = i;
        y[i] = i*i;
        n_points ++;
    }

    TApplication theApp("name", &argc, argv);
    TCanvas *c1 = new TCanvas("c1", "C1", 800, 600);
    TGraph *graph = new TGraph();
    TText *text = new TText();
    text->SetTextSize(0.05);
    text->SetTextColor(kBlack);
    text->SetNDC(1);

    for (int i = 0; i < n_points; i++) {
        graph->SetPoint(i, x[i], y[i]);
    }
    graph->SetTitle(Form("Channel: %d", 0));
    graph->SetMinimum(0.0);
    graph->SetMaximum(150.0);
    graph->Draw("al*");
    text -> DrawText(0.7, 0.92, Form("Trigger: %d", 100));
    c1->Update();

    sleep(5);
    // theApp.Run();
    return 0;
}
