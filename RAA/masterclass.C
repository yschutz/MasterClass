#include <TApplication.h>
#include <TGClient.h>
#include <TGButton.h>
#include <TGListBox.h>
#include <TGLabel.h>
#include <TGComboBox.h>
#include <TList.h>
#include <TRootHelpDialog.h>
#include <TROOT.h>
#include <TSystem.h>
#include <TString.h>
#include <string.h>

class MasterClassFrame : public TGMainFrame {
    
private:
    
    TGTextButton		*fRole;
    TGTextButton		*fExit;
    Int_t 		        fChoice;
    Int_t 		        fMode;
    TGComboBox		    *fDataset;
    
public:
    
    MasterClassFrame(const TGWindow *p, UInt_t w, UInt_t h);
    virtual ~MasterClassFrame();
 
    void Start();
    void StartExample();
    void StartStudent();
    void StartTeacher();
    
    ClassDef(MasterClassFrame, 0)
};

void MasterClassFrame::Start()
{
    
#if defined(__CINT__) && !defined(__MAKECINT__)
    {
        if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0)
            Info("alice_vsd.C", "Has to be run in compiled mode ... doing this for you.");
        else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0)
            Info("alice_vsd.C", "Doit etre prealablement compile ... compilation en cours.");
        gSystem->CompileMacro("alice_vsd.C");
        alice_vsd(fChoice, fMode, fDataset->GetSelected());
    }
#else
    alice_vsd(fChoice, fMode, fDataset->GetSelected());
#endif
    
    UnmapWindow();
    
}

void MasterClassFrame::StartExample()
{
    
    fChoice = 1;
    fMode   = 1;
    
    Start();
    
}

void MasterClassFrame::StartStudent()
{
    
    fChoice = 2;
    fMode   = 2;
    
    Start();
    
}

void MasterClassFrame::StartTeacher()
{
    
    fChoice = 2;
    fMode   = 3;
    
    Start();
    
}

MasterClassFrame::MasterClassFrame(const TGWindow *p, UInt_t w, UInt_t h) :
TGMainFrame(p, w, h)
{
    
    TGVerticalFrame* hf = new TGVerticalFrame(this);
    {
        
        TGLabel* label = new TGLabel(hf,"MasterClass");
        hf->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 5, 5, 5, 5));
		
		TString fileName("2011-Nov-24-ALICE_logo_small.eps");	

        TGPictureButton* b1 = new TGPictureButton(hf, gClient->GetPicture(fileName.Prepend(gSystem->Getenv("IMPATH"))));
        hf->AddFrame(b1, new TGLayoutHints(kLHintsExpandX, 5, 5, 5, 5));
        
        fRole = new TGTextButton(hf,"Demarrer");
        hf->AddFrame(fRole, new TGLayoutHints(kLHintsExpandX, 5, 5, 5, 5));
		fRole->Connect("Clicked()", "MasterClassFrame", this, "StartStudent()");
        fDataset = new TGComboBox(hf);
        TString name;
        if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0)
            name = "dataset";
        else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0)
            name = "jeu de donnees";
        
        fDataset->AddEntry(Form("%s 1", name.Data()),   1);
        fDataset->AddEntry(Form("%s 2", name.Data()),   2);
        fDataset->AddEntry(Form("%s 3", name.Data()),   3);
        fDataset->AddEntry(Form("%s 4", name.Data()),   4);
        fDataset->AddEntry(Form("%s 5", name.Data()),   5);
        fDataset->AddEntry(Form("%s 6", name.Data()),   6);
        fDataset->AddEntry(Form("%s 7", name.Data()),   7);
        fDataset->AddEntry(Form("%s 8", name.Data()),   8);
        fDataset->AddEntry(Form("%s 9", name.Data()),   9);
        fDataset->AddEntry(Form("%s 10", name.Data()), 10);
        //       fDataset->AddEntry("dataset 11", 11);
        //       fDataset->AddEntry("dataset 12", 12);
        //       fDataset->AddEntry("dataset 13", 13);
        //       fDataset->AddEntry("dataset 14", 14);
        //       fDataset->AddEntry("dataset 15", 15);
        //       fDataset->AddEntry("dataset 16", 16);
        //       fDataset->AddEntry("dataset 17", 17);
        //       fDataset->AddEntry("dataset 18", 18);
        // 	     fDataset->AddEntry("dataset 19", 19);
        fDataset->Resize(100, 20);
        fDataset->Select(1, kFALSE);
        hf->AddFrame(fDataset, new TGLayoutHints(kLHintsExpandX));
        
        fExit = new TGTextButton(hf, "Terminer");
        hf->AddFrame(fExit, new TGLayoutHints(kLHintsExpandX, 5, 5, 10, 10));
        fExit->Connect("Clicked()", "TApplication", gApplication, "Terminate()");
    }
    
    AddFrame(hf);
    
    SetWindowName("MasterClass");
    MapSubwindows();
    
    Resize(GetDefaultSize());
    
    MapWindow();
    
}

MasterClassFrame::~MasterClassFrame()
{
    
    Cleanup();
    
}

void masterclass()
{
    
    gROOT->pwd();
    
    new MasterClassFrame(gClient->GetRoot(), 400, 200);
    
    TRootHelpDialog* instructions = new TRootHelpDialog(gClient->GetRoot(), "MASTERCLASS MENU INSTRUCTIONS", 700, 250);
    if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0)
        instructions->SetText("Welcome to ALICE Masterclass!\n\n You see the window <MasterClass>. In this window you can select the dataset which you are\n supposed to analyse, please ask your tutor which number it has, afterwards you just press \n the buttom \"Start\" and you can start the first part of the ALICE master classes.\n With the buttom \"Exit\" you can finish the exercise.\n ");
    else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0)
        instructions->SetText("Bienvenu.e a l'exercice Masterclass de ALICE !\n\n A partir de la fenetre <MasterClass> vous pouvez choisir le jeu de donnees \n qu'il vous est demande d'analyser, veuillez demander a votre tuteur le bon numero. \n Apres l'avoir selectionne le jeu de donnees, appuyez sur le bouton \"Demarrer\"  \n pour pouvoir lancer la premiere partie de l'exercice.\n\n En appuyant sur le bouton \"Terminer\" vous terminerez l'exercice.\n ");
    
    instructions->Popup();
    
}
