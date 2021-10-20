#ifndef CALO_DST_TOOL
#define CALO_DST_TOOL

#include <fun4all/SubsysReco.h>
#include <TH1.h>
#include <TH2.h>

#include <string>

#include <phool/PHCompositeNode.h>
#include <phool/phool.h>

class CaloDstTool : public SubsysReco {
    public:
        CaloDstTool(const std::string &module_name);
        virtual  ~CaloDstTool() {};

        int InitRun(PHCompositeNode *topNode) override;
        int process_event(PHCompositeNode *topNode) override;
        int End(PHCompositeNode *topNode) override;

        void set_output_file(const std::string &path) {outPath = path;};
        void set_detector(const std::string &d) {detector = d;};

    private:
        // Housekeeping
        std::string outPath;
        std::string detector;

        // Data
        TH1 *energyScale;
        TH2 *eScaleEta;
        TH2 *eScaleGe;
};

#endif // CALO_DST_TOOL
