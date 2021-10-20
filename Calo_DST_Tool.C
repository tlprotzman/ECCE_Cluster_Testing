#include "Calo_DST_Tool.h"

#include <calobase/RawTower.h>
#include <calobase/RawTowerContainer.h>
#include <calobase/RawTowerDefs.h>
#include <fun4all/Fun4AllReturnCodes.h>
#include <fun4all/SubsysReco.h>

#include <TH1F.h>
#include <TH2F.h>
#include <TFile.h>
#include <TVector3.h>

#include <phool/getClass.h>
#include <phool/PHCompositeNode.h>
#include <phool/phool.h>
#include <g4main/PHG4TruthInfoContainer.h>
#include <g4main/PHG4Particle.h>

CaloDstTool::CaloDstTool(const std::string &name):
 SubsysReco(name)
{
}

int CaloDstTool::InitRun(PHCompositeNode *topNode) {
    this->energyScale = new TH1F("eScale", "EEEMC Single Electron Energy Scale", 100, -0.5, 1.2);
    this->energyScale->GetXaxis()->SetTitle("(ge - e) / ge");
    this->energyScale->GetYaxis()->SetTitle("Counts");

    this->eScaleEta = new TH2F("eScaleEta", "EEEMC Single Electron Energy Scale", 100, -4, -1, 100, -0.5, 1.2);
    this->eScaleEta->GetXaxis()->SetTitle("gEta");
    this->eScaleEta->GetYaxis()->SetTitle("(ge - e) / ge");
    
    this->eScaleGe = new TH2F("eScaleGe", "EEEMC Single Electron Energy Scale", 100, 0, 80, 100, -0.5, 1.2);
    this->eScaleGe->GetXaxis()->SetTitle("ge");
    this->eScaleGe->GetYaxis()->SetTitle("(ge - e) / ge");
    return Fun4AllReturnCodes::EVENT_OK;
}

int CaloDstTool::End(PHCompositeNode *topNode) {
    TFile *f = new TFile(this->outPath.c_str(), "RECREATE");
    this->energyScale->Write();
    this->eScaleEta->Write();
    this->eScaleGe->Write();
    f->Close();
    return Fun4AllReturnCodes::EVENT_OK;
}

int CaloDstTool::process_event(PHCompositeNode *topNode) {
    // Grab calo towers
    std::string towerNodeName = "TOWER_CALIB_" + this->detector;
    RawTowerContainer *towers = findNode::getClass<RawTowerContainer>(topNode, towerNodeName);
    if (!towers) {
        std::cout << PHWHERE << ": Could not find node " << towerNodeName << std::endl;
        return Fun4AllReturnCodes::DISCARDEVENT;
    }

    // Grab truth information
    std::string truthNodeName = "G4TruthInfo";
    PHG4TruthInfoContainer *truth = findNode::getClass<PHG4TruthInfoContainer>(topNode, truthNodeName);
    if (!truth) {
        std::cout << PHWHERE << ": Could not find node " << truthNodeName << std::endl;
        return Fun4AllReturnCodes::DISCARDEVENT;
    }


    // Sum the energy in the calorimeter
    float e_total = 0;
    RawTowerContainer::ConstRange begin_end_calo = towers->getTowers();
    for (RawTowerContainer::ConstIterator itr = begin_end_calo.first; itr != begin_end_calo.second; ++itr) {
        RawTower *tower = itr->second;
        e_total += tower->get_energy();
    }

    float ge_total = 0;
    int total_particles = 0;
    float eta = -99;
    PHG4TruthInfoContainer::ConstRange begin_end_truth = truth->GetParticleRange();
    for (PHG4TruthInfoContainer::ConstIterator itr = begin_end_truth.first; itr != begin_end_truth.second; ++itr) {
        PHG4Particle *particle = itr->second;
        ge_total += particle->get_e();
        TVector3 p_vec = TVector3(particle->get_px(), particle->get_py(), particle->get_pz());
        eta = p_vec.Eta();
        total_particles++;
        // std::cout << "PID: " << particle->get_pid() << std::endl;    // double check everything is electrons
    }
    // std::cout << PHWHERE << ": Found " << total_particles << " truth particles" << std::endl;

    float scale = (ge_total - e_total) / ge_total;
    if (e_total < 0.1) {
        scale = -0.4;
    }
    this->energyScale->Fill(scale);
    this->eScaleEta->Fill(eta, scale);
    this->eScaleGe->Fill(ge_total, scale);
    return Fun4AllReturnCodes::EVENT_OK;
}
