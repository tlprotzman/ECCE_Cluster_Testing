#include <TROOT.h>
#include <TString.h>

#include <fun4all/Fun4AllServer.h>

#include "G4_Input.C"
#include "Calo_DST_Tool.C"

R__LOAD_LIBRARY(libfun4all.so)

int BEMC_Cluster_Testing(const TString &inFile = "G4EICDetector.root", const int nEvents = 0) {
    // Set up Fun4All server
    Fun4AllServer *se = Fun4AllServer::instance();
    se->Verbosity(1);

    Input::READHITS = true;
    INPUTREADHITS::filename[0] = inFile;

    InputManagers();


    // Register Analysis
    // CaloDstTool *bemcTester = new CaloDstTool("caloTester");
    // bemcTester->set_detector("BECAL");
    // bemcTester->set_output_file("bemc_test.root");
    // se->registerSubsystem(bemcTester);

    // CaloDstTool *femcTester = new CaloDstTool("caloTester");
    // femcTester->set_detector("FEMC");
    // femcTester->set_output_file("femc_test.root");
    // se->registerSubsystem(femcTester);

    CaloDstTool *eemchTester = new CaloDstTool("caloTester");
    eemchTester->set_detector("EEMC");
    eemchTester->set_output_file("eemc_test.root");
    se->registerSubsystem(eemchTester);


    // Process Events
    se->run(nEvents);
    se->End();


    // Cleanup
    // delete bemcTester;
    // delete femcTester;
    delete eemchTester;
    delete se;
    gSystem->Exit(0);
    return 0;
    
}