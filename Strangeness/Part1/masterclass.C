#include <TApplication.h>
#include <TGClient.h>
#include <TGButton.h>
#include <TGListBox.h>
#include <TGComboBox.h>
#include <TList.h>

class MasterClassFrame : public TGMainFrame {

private:

	TGTextButton		*fDemo;
	TGTextButton		*fStudent;
	TGTextButton		*fTeacher;
	TGTextButton		*fExit;
	Int_t 		 fChoice;
	Int_t 		 fMode;
	TGComboBox		*fDataset;

public:

	MasterClassFrame(const TGWindow *p, UInt_t w, UInt_t h);
	virtual ~MasterClassFrame();
	void Start();
	void StartDemo();
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
		gSystem->CompileMacro("alice_vsd.C");
		alice_vsd(fChoice, fMode, fDataset->GetSelected());
	}
#else
	alice_vsd(fChoice, fMode, fDataset->GetSelected());
#endif

	UnmapWindow();

}

void MasterClassFrame::StartDemo()
{

	fChoice = 1;
	fMode = 1;

	Start();

}

void MasterClassFrame::StartStudent()
{

	fChoice = 2;
	fMode = 2;

	Start();

}

void MasterClassFrame::StartTeacher()
{

	fChoice = 2;
	fMode = 3;

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
 
 
		fDemo = new TGTextButton(hf,"Demo");
		hf->AddFrame(fDemo, new TGLayoutHints(kLHintsExpandX, 5, 5, 5, 5));
		fDemo->Connect("Clicked()", "MasterClassFrame", this, "StartDemo()");

        TString name;
        if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0)
            name = "Student";
        else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0)
            name = "Etudiant";

		fStudent = new TGTextButton(hf, name);
		hf->AddFrame(fStudent, new TGLayoutHints(kLHintsExpandX, 5, 5, 5, 5));
		fStudent->Connect("Clicked()", "MasterClassFrame", this, "StartStudent()");

        if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0)
            name = "Teacher";
        else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0)
            name = "Tuteur";

		fTeacher = new TGTextButton(hf, name);
		hf->AddFrame(fTeacher, new TGLayoutHints(kLHintsExpandX, 5, 5, 5, 5));
		fTeacher->Connect("Clicked()", "MasterClassFrame", this, "StartTeacher()");
      
		fDataset = new TGComboBox(hf);
        if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0)
            name = "dataset";
        else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0)
            name = "jeu de donnees";
   
        
      fDataset->AddEntry(Form("%s  01", name.Data()), 1);
      fDataset->AddEntry(Form("%s  02", name.Data()), 2);
      fDataset->AddEntry(Form("%s  03", name.Data()), 3);
      fDataset->AddEntry(Form("%s  04", name.Data()), 4);
      fDataset->AddEntry(Form("%s  05", name.Data()), 5);
      fDataset->AddEntry(Form("%s  06", name.Data()), 6);
      fDataset->AddEntry(Form("%s  07", name.Data()), 7);
      fDataset->AddEntry(Form("%s  08", name.Data()), 8);
      fDataset->AddEntry(Form("%s  09", name.Data()), 9);
      fDataset->AddEntry(Form("%s  10", name.Data()), 10);
      fDataset->AddEntry(Form("%s  11", name.Data()), 11);
      fDataset->AddEntry(Form("%s  12", name.Data()), 12);
      fDataset->AddEntry(Form("%s  13", name.Data()), 13);
      fDataset->AddEntry(Form("%s  14", name.Data()), 14);
      fDataset->AddEntry(Form("%s  15", name.Data()), 15);
      fDataset->AddEntry(Form("%s  16", name.Data()), 16);
      fDataset->AddEntry(Form("%s  17", name.Data()), 17);
      fDataset->AddEntry(Form("%s  18", name.Data()), 18);
      fDataset->AddEntry(Form("%s  19", name.Data()), 19); 
      fDataset->AddEntry(Form("%s  20", name.Data()), 20);
      fDataset->AddEntry(Form("%s  Pb-Pb", name.Data()), 101);
        
          
		fDataset->Resize(100,20);
		fDataset->Select(1,kFALSE);
		hf->AddFrame(fDataset, new TGLayoutHints(kLHintsExpandX));

        if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0)
            name = "Exit";
        else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0)
            name = "Sortie";
		fExit = new TGTextButton(hf, name);
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
  	  instructions->SetText("\
Welcome to ALICE Masterclass!\n\n\
\
You see the window <MasterClass>.There are 4 buttons to your disposal\n\
Using these buttons, one can choose the mode:\n\n\
Demo - starts a demo of MasterClass exercise with 4 events to analyse\n\
\
Student - starts MasterClass with 100 real events for you to analyze\n\
\
Teacher - starts Masterclass with Teacher tools that allow to summarise\n\
the students results and draw conclusions from the analysis\n\n\
\
The button <Exit> quits MasterClass application\n\n\
\
NOTE: after you click <Demo> <Student> or <Teacher>, it takes about 30 seconds to start\n\
the application\n\n"
		  );
    else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0)
    	  instructions->SetText("\
Bienvenu.e a l'exercice Masterclass de ALICE !\n\n\
\
Dans la fenetre <MasterClass>, il y a 4 boutons a votre disposition\n\
En cliquant ces boutons, vous pouvez choisir l'un des modes suivant :\n\n\
\t Demo - demarre une demonstration de l'exercice MasterClass analysant 4 evenements\n\
\
\t Etudiant - demarre l'exercice MasterClass avec 100 evenemets reels que vous devrez analyser\n\
\
\t Tuteur - demarre l'exercice Masterclass avec des outils permettant de finaliser l'exercice\n\
\t a partir des resultats obtenus par les etudiants et de conclure\n\n\
\
Le bouton <Sortie> quitte l'application MasterClass\n\n\
\
NOTE: apres avoir cliquer <Demo> <Etudiant> ou <Tuteur>, l'application ne sera demarree qu'apres environ 30 secondes\n"
			  );
	instructions->Popup();

}
