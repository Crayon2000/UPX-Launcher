object Form1: TForm1
  Left = 442
  Top = 352
  BorderIcons = [biSystemMenu, biMinimize]
  BorderStyle = bsSingle
  ClientHeight = 321
  ClientWidth = 417
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 16
    Top = 16
    Width = 49
    Height = 13
    Caption = 'Select file:'
  end
  object GroupBox1: TGroupBox
    Left = 8
    Top = 64
    Width = 385
    Height = 145
    Caption = 'Compression methods'
    TabOrder = 0
    object rbBestNRV2D: TRadioButton
      Left = 16
      Top = 78
      Width = 249
      Height = 25
      Caption = 'Best with NRV2D'
      TabOrder = 0
    end
    object rbBestNRV2B: TRadioButton
      Left = 16
      Top = 109
      Width = 249
      Height = 25
      Caption = 'Best with NRV2B'
      TabOrder = 1
    end
    object rbUltraBrute: TRadioButton
      Left = 16
      Top = 16
      Width = 249
      Height = 25
      Caption = 'Ultra Brute'
      TabOrder = 2
    end
    object rbBrute: TRadioButton
      Left = 16
      Top = 47
      Width = 249
      Height = 25
      Caption = 'Brute'
      TabOrder = 3
    end
  end
  object txtSelected: TEdit
    Left = 16
    Top = 32
    Width = 297
    Height = 21
    TabOrder = 1
  end
  object cmdBrowse: TButton
    Left = 320
    Top = 32
    Width = 81
    Height = 25
    Caption = 'Browse...'
    TabOrder = 2
    OnClick = cmdBrowseClick
  end
  object cmdCompress: TButton
    Left = 16
    Top = 278
    Width = 385
    Height = 33
    Caption = 'Compress'
    TabOrder = 3
    OnClick = cmdCompressClick
  end
  object GroupBox2: TGroupBox
    Left = 16
    Top = 215
    Width = 385
    Height = 57
    Caption = 'Option'
    TabOrder = 4
    object chkBackup: TCheckBox
      Left = 16
      Top = 16
      Width = 105
      Height = 25
      Caption = 'Make Backup'
      TabOrder = 0
    end
  end
  object Dialog: TOpenDialog
    Left = 368
    Top = 64
  end
end
