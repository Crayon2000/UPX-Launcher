//---------------------------------------------------------------------------
#ifndef MainH
#define MainH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Dialogs.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:    // IDE-managed Components
    TGroupBox *GroupBox1;
    TRadioButton *rbBestNRV2D;
    TRadioButton *rbBestNRV2B;
    TLabel *Label1;
    TEdit *txtSelected;
    TOpenDialog *Dialog;
    TButton *cmdBrowse;
    TButton *cmdCompress;
    TGroupBox *GroupBox2;
    TCheckBox *chkBackup;
    TRadioButton *rbUltraBrute;
    TRadioButton *rbBrute;
    void __fastcall cmdBrowseClick(TObject *Sender);
    void __fastcall cmdCompressClick(TObject *Sender);
private:    // User declarations
    HANDLE __fastcall ExecuteProgramEx(const String ACmd);
    void __fastcall Wait(HANDLE AHandle);
    bool __fastcall FileSize(const String AName, int& ASize);
    void __fastcall EnableControls(bool AEnabled);
public:     // User declarations
    __fastcall TForm1(TComponent* Owner);
    __fastcall virtual ~TForm1() {};
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
