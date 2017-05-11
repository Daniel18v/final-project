object frmSettings: TfrmSettings
  Left = 0
  Top = 0
  Caption = 'Settings'
  ClientHeight = 466
  ClientWidth = 630
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
    Width = 630
    Height = 436
    Align = alClient
    DefaultUrl = 'about:blank'
    TabOrder = 0
    OnBeforeBrowse = Chromium1BeforeBrowse
    Options.JavascriptOpenWindows = STATE_DISABLED
    Options.JavascriptCloseWindows = STATE_DISABLED
    Options.WebSecurity = STATE_DISABLED
    ExplicitWidth = 640
    ExplicitHeight = 450
  end
  object debug_Toolbar: TToolBar
    Left = 0
    Top = 0
    Width = 630
    Height = 30
    ButtonHeight = 21
    ButtonWidth = 43
    Caption = 'debug_Toolbar'
    DoubleBuffered = True
    ParentDoubleBuffered = False
    ShowCaptions = True
    TabOrder = 1
    Visible = False
    ExplicitWidth = 640
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
