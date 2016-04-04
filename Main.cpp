//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Main.h"
#include <System.IOUtils.hpp>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------

__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
    Caption = "Ultimate Packer for eXecutables Launcher";

    Dialog->Title = "Choose file to compress";
    Dialog->Filter = "Execute file (*.exe)|*.exe";
    Dialog->Options.Clear();
    Dialog->Options << TOpenOption::ofFileMustExist << TOpenOption::ofNoChangeDir << TOpenOption::ofHideReadOnly;

    rbMode1->Checked = true;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::cmdBrowseClick(TObject *Sender)
{
    if(Dialog->Execute() == true)
    {
        txtSelected->Text = Dialog->FileName;
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::cmdCompressClick(TObject *Sender)
{
    int OldSize;
    int NewSize;

    EnableControls(false);

    if(txtSelected->Text.IsEmpty() == true)
    {
        return;
    }

    FileSize(txtSelected->Text, OldSize);

    // On fait le backup
    if(chkBackup->Checked == true)
    {
        try
        {
            TFile::Copy(txtSelected->Text, txtSelected->Text + ".bak", true);
        }
        catch(...)
        {
            MessageBoxW(Handle, L"Backup failed", L"Error", MB_OK);
        }
    }

    // Launch UPX.exe
    String cmdLine = "\"" + txtSelected->Text + "\"";
    if(rbMode1->Checked == true)
    {
        cmdLine = "--best --crp-ms=999999 --nrv2d " + cmdLine;
    }
    else
    {
        cmdLine = "--best --crp-ms=999999 --nrv2b " + cmdLine;
    }
    if(FileExists(txtSelected->Text) == false)
    {
        MessageBeep(0);
        MessageDlg("The program must be in the same folder as upx.exe."
                   "\r\n" \
                   "Program will not be compressed.",
                   TMsgDlgType::mtError, TMsgDlgButtons() << TMsgDlgBtn::mbOK, 0);
    }
    else
    {
        HANDLE LHandle = ExecuteProgramEx("upx.exe " + cmdLine);
        Wait(LHandle);

        if(LHandle == NULL)
        {
            MessageBeep(0);
            MessageDlg("Error opening upx.exe."
                       "\r\n" \
                       "Program will not be compressed.",
                       TMsgDlgType::mtError, TMsgDlgButtons() << TMsgDlgBtn::mbOK, 0);
        }
    }

    FileSize(txtSelected->Text, NewSize);
    float ratio = (float)(100.0f * NewSize / OldSize);

    MessageDlg("Original file size: " + System::Sysutils::IntToStr(OldSize) + " bytes"
              "\r\n" \
              "Compress file size: " + System::Sysutils::IntToStr(NewSize) + " bytes"
              "\r\n" \
              "Ratio: " + FormatFloat("0.00", ratio) + "%",
              TMsgDlgType::mtInformation, TMsgDlgButtons() << TMsgDlgBtn::mbOK, 0);

    EnableControls(true);
}
//---------------------------------------------------------------------------

HANDLE __fastcall TForm1::ExecuteProgramEx(const String ACmd)
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    SECURITY_ATTRIBUTES sa;

    si.cb = sizeof(si);
    si.lpReserved = NULL;
    si.lpDesktop = NULL;
    si.lpTitle = L"UPX",
    si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
    si.cbReserved2 = 0;
    si.lpReserved2 = NULL;
    si.wShowWindow = SW_HIDE;
    si.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
    si.hStdError = GetStdHandle(STD_ERROR_HANDLE);

    sa.nLength = sizeof sa;
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = true;

    //Pour mettre l'output dans un fichier
    /*
    si.hStdOutput = CreateFile(
               "log.txt",
               GENERIC_WRITE,
               FILE_SHARE_WRITE,
               &sa,
               CREATE_ALWAYS,
               FILE_ATTRIBUTE_NORMAL,
               NULL);
    */
    bool ProcResult = CreateProcess(NULL, ACmd.c_str(), NULL, NULL, true, 0, NULL, NULL, &si, &pi);
    if(ProcResult == true)
    {
        return pi.hProcess;
    }

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    CloseHandle(si.hStdOutput);
    return NULL;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Wait(HANDLE AHandle)
{
    DWORD ExitCode;

    if(AHandle == NULL || AHandle == INVALID_HANDLE_VALUE)
    {
        return;
    }
    // In this wait procedure, we set the cursor to the hourglass.
    // Then we repeat a loop: Then restore the cursor.
    TCursor Oldcursor = Screen->Cursor;
    Screen->Cursor = crHourGlass;

    bool Done = false;
    while(Done == false)
    {
        GetExitCodeProcess(
            AHandle,        // handle to the process
            &ExitCode       // address to receive termination status
        );

        if(ExitCode == STILL_ACTIVE)
        {
            Application->ProcessMessages();
        }
        else
        {
            Done = true;
        }

        /*
        DWORD res = WaitForSingleObject(AHandle,50);
        if(res==WAIT_TIMEOUT)
        {
            Application->ProcessMessages();
        }
        else
        {
            Done = true;
        }
        */
    }
    CloseHandle(AHandle);

    Screen->Cursor = Oldcursor;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::EnableControls(bool AEnabled)
{
    cmdCompress->Enabled = AEnabled;
    Label1->Enabled = AEnabled;
    txtSelected->Enabled = AEnabled;
    cmdBrowse->Enabled = AEnabled;
    rbMode1->Enabled = AEnabled;
    rbMode2->Enabled = AEnabled;
    chkBackup->Enabled = AEnabled;
}
//---------------------------------------------------------------------------

bool __fastcall TForm1::FileSize(const String AName, int& ASize)
{
    bool Result = false;

    NativeUInt FileHandle = Sysutils::FileOpen(AName, fmOpenRead);
    if(FileHandle > 0)
    {
        ASize = Sysutils::FileSeek(FileHandle, 0, 2);
        if(ASize > 0)
        {
            Result = true;
        }
        FileClose(FileHandle);
    }

    return Result;
}
//---------------------------------------------------------------------------

