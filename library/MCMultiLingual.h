// helper class to translate text
// Yves Schutz March 2019

#ifndef MCMultiLingual_h
#define MCMultiLingual_h

#include <string>
#include <vector>
#include <RQ_OBJECT.h>

class MCMultiLingual
{
RQ_OBJECT("MCMultiLingual")
enum E_Language
{
    L_FR,
    L_EN
};
private:
    std::vector<std::string> fFrancais; 
    std::vector<std::string> fEnglish;
    int                      fLanguage;   // the language id
    static MCMultiLingual   fInstance;    // unique instance of singleton

    MCMultiLingual();
    MCMultiLingual (const MCMultiLingual &) {}
    virtual ~MCMultiLingual(); 

public:  
    static MCMultiLingual    &Instance(); 
     char                *tr(const char *text); 
    void                      SetLanguage(const TString &lan);

ClassDef(MCMultiLingual, 1);
};
#endif