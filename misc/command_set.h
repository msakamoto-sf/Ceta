//---------------------------------------------------------------------------
#ifndef command_setH
#define command_setH
//---------------------------------------------------------------------------


class CommandSet {
public:
	CommandSet(
		AnsiString CommandName = "",
		AnsiString CommandFile = "",
		AnsiString CommandLine = "",
		AnsiString CommandEnv = "",
		int CommandErr = 0,
        int BracketDQ = 1)
        {
			SetCommandName(CommandName);
			SetCommandFile(CommandFile);
			SetCommandLine(CommandLine);
			SetCommandEnv(CommandEnv);
			SetCommandErr(CommandErr);
            SetBracketDQ(BracketDQ);
        }

    AnsiString GetCommandName() { return _CommandName; }
    void SetCommandName(AnsiString buf) { _CommandName = buf; }
    AnsiString GetCommandFile() { return _CommandFile; }
    void SetCommandFile(AnsiString buf) { _CommandFile = buf; }
    AnsiString GetCommandLine() { return _CommandLine; }
    void SetCommandLine(AnsiString buf) { _CommandLine = buf; }
    AnsiString GetCommandEnv() { return _CommandEnv; }
    void SetCommandEnv(AnsiString buf) { _CommandEnv = buf; }
    bool GetCommandErr() { return _CommandErr; }
    void SetCommandErr(bool buf) { _CommandErr = buf; }
    bool GetBracketDQ() { return _BracketDQ; }
    void SetBracketDQ(bool buf) { _BracketDQ = buf; }

    CommandSet *next;

private:
	AnsiString _CommandName;
    AnsiString _CommandFile;
    AnsiString _CommandLine;
    AnsiString _CommandEnv;
    bool _CommandErr;
    bool _BracketDQ;
};

#endif
