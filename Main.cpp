//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Main.h"
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
    Dialog->Options << ofOldStyleDialog << ofNoLongNames << ofNoChangeDir << ofHideReadOnly;

    rbMode1->Checked = true;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::cmdBrowseClick(TObject *Sender)
{
    if(Dialog->Execute() == true)
    {
        if(FileExists(Dialog->FileName) == true)
        {
            txtSelected->Text = Dialog->FileName;
        }
        else
        {
            MessageBeep(0);
            MessageDlg("File not found." \
                    "\r\n" \
                    "The requested file does not exist.",
                    TMsgDlgType::mtWarning, TMsgDlgButtons() << TMsgDlgBtn::mbOK, 0);
        }
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::cmdCompressClick(TObject *Sender)
{
    HANDLE hInstance = NULL;
    int oldSize;
    int newSize;
    float ratio;
    String cmdLine;

    if(txtSelected->Text.IsEmpty() == true)
    {
        return;
    }

    cmdCompress->Enabled = false;

    FileSize(txtSelected->Text, &oldSize);

    // On fait le backup
    if(chkBackup->Checked == true)
    {
        if(CopyFile(txtSelected->Text.c_str(), String(txtSelected->Text + ".bak").c_str(), false) == false)
        {
            MessageBoxW(Handle, L"Backup failed", L"Error", MB_OK);
        }
    }

    // Launch UPX.exe
    if(rbMode1->Checked == true)
    {
        cmdLine = String("--best --crp-ms=999999 --nrv2d " + txtSelected->Text);
    }
    else
    {
        cmdLine = String("--best --crp-ms=999999 --nrv2b " + txtSelected->Text);
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
        //hInstance = ShellExecute(NULL, "open", "upx.exe", cmdLine.c_str(), NULL, SW_HIDE);
        hInstance = ExecuteProgramEx("upx.exe " + cmdLine);
        Wait(hInstance);

        if(!hInstance)
        {
            MessageBeep(0);
            MessageDlg("Error opening upx.exe."
                       "\r\n" \
                       "Program will not be compressed.",
                       TMsgDlgType::mtError, TMsgDlgButtons() << TMsgDlgBtn::mbOK, 0);
        }
    }

    FileSize(txtSelected->Text, &newSize);
    ratio = (float)(100.0f * newSize / oldSize);

    MessageDlg("Original file size: " + System::Sysutils::IntToStr(oldSize) + " bytes"
              "\r\n" \
              "Compress file size: " + System::Sysutils::IntToStr(newSize) + " bytes"
              "\r\n" \
              "Ratio: " + FormatFloat("0.00", ratio) + "%",
              TMsgDlgType::mtInformation, TMsgDlgButtons() << TMsgDlgBtn::mbOK, 0);

    cmdCompress->Enabled = true;
    //CloseHandle(hInstance);
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
    bool bres = CreateProcess(NULL, ACmd.c_str(), NULL, NULL, true, 0, NULL, NULL, &si, &pi);
    if(bres == true)
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
    // In this wait procedure, we set the cursor to the hourglass
    // and disable the form. Then we repeat a loop:
    // Then restore the form and cursor.
    TCursor Oldcursor = Screen->Cursor;
    Screen->Cursor = crHourGlass;

    Enabled = false;
    bool done = false;
    while(done == false)
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
            done = true;
        }

        /*
        DWORD res = WaitForSingleObject(AHandle,50);
        if (res==WAIT_TIMEOUT)
        {
            Application->ProcessMessages();
        }
        else
        {
            done=true;
        }
        */
    }
    CloseHandle(AHandle);

    Screen->Cursor = Oldcursor;
    Enabled = true;
}
//---------------------------------------------------------------------------

bool __fastcall TForm1::FileSize(const String AName, int * ASize)
{
    int FileSize;
    int FHandle = FileOpen(AName, fmOpenRead);
    if(FHandle != -1)
    {
        FileSize = FileSeek(FHandle, 0, 2);
        FileClose(FHandle);
        *ASize = FileSize;
        return true;
    }

    return false;
}
//---------------------------------------------------------------------------

