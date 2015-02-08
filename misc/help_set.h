//---------------------------------------------------------------------------
#ifndef help_setH
#define help_setH
//---------------------------------------------------------------------------

class HelpSet {

public:
	HelpSet(AnsiString HelpName = "", AnsiString HelpFile = "")
    	{ SetHelpName(HelpName);
          SetHelpFile(HelpFile); }

    AnsiString GetHelpName() { return _HelpName; }
    void SetHelpName(AnsiString buf) { _HelpName = buf; }
    AnsiString GetHelpFile() { return _HelpFile; }
    void SetHelpFile(AnsiString buf) { _HelpFile = buf; }

    HelpSet *next;

private:
	AnsiString _HelpName;
    AnsiString _HelpFile;
};

#endif
 