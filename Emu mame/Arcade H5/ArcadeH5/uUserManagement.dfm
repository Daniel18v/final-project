object frmUsers: TfrmUsers
  Left = 0
  Top = 0
  BorderStyle = bsToolWindow
  Caption = 'User management'
  ClientHeight = 320
  ClientWidth = 480
  Color = clBtnFace
  Constraints.MinHeight = 320
  Constraints.MinWidth = 480
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  GlassFrame.Enabled = True
  GlassFrame.SheetOfGlass = True
  OldCreateOrder = False
  Position = poOwnerFormCenter
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Chromium1: TChromium
    Left = 0
    Top = 30
    Width = 480
    Height = 290
    Align = alClient
    DefaultUrl = 'about:blank'
    TabOrder = 0
    OnBeforeBrowse = Chromium1BeforeBrowse
  end
  object debug_Toolbar: TToolBar
    Left = 0
    Top = 0
    Width = 480
    Height = 30
    ButtonHeight = 21
    ButtonWidth = 43
    Caption = 'debug_Toolbar'
    DoubleBuffered = True
    ParentDoubleBuffered = False
    ShowCaptions = True
    TabOrder = 1
    Visible = False
    object Label1: TLabel
      Left = 0
      Top = 0
      Width = 81
      Height = 21
      AutoSize = False
      Caption = 'Debug toolbar:   '
    end
    object ToolButton1: TToolButton
      Left = 81
      Top = 0
      Caption = 'Reload'
      ImageIndex = 0
      OnClick = ToolButton1Click
    end
    object ToolButton2: TToolButton
      Left = 124
      Top = 0
      Caption = 'Inspect'
      ImageIndex = 1
      OnClick = ToolButton2Click
    end
  end
end
