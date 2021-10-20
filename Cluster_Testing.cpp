#include <TROOT.h>

#include <fun4all/Fun4AllServer.h>

#include "G4_Input.C"
#include "Calo_DST_Tool.C"

R__LOAD_LIBRARY(libfun4all.so)

int Cluster_Testing(const char *inFile, const char *outFile, const char *detector, const int nEvents = 0) {
    // Set up Fun4All server
    Fun4AllServer *se = Fun4AllServer::instance();
    se->Verbosity(1);

    Input::READHITS = true;
    INPUTREADHITS::filename[0] = inFile;

    InputManagers();

    CaloDstTool *caloTester = new CaloDstTool("caloTester");
    caloTester->set_detector(detector);
    caloTester->set_output_file(outFile);
    se->registerSubsystem(caloTester);


    // Process Events
    se->run(nEvents);
    se->End();


    // Cleanup
    delete caloTester;
    delete se;
    gSystem->Exit(0);
    return 0;
    
}