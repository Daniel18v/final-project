unit uUserManagement;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, cefvcl, ceflib, cefgui, ceffilescheme, ComCtrls, ToolWin, StdCtrls,
  idURI;

type
  TfrmUsers = class(TForm)
    Chromium1: TChromium;
    debug_Toolbar: TToolBar;
    ToolButton1: TToolButton;
    ToolButton2: TToolButton;
    Label1: TLabel;
    procedure ToolButton1Click(Sender: TObject);
    procedure ToolButton2Click(Sender: TObject);
    procedure FormShow(Sender: TObject);
    procedure Chromium1BeforeBrowse(Sender: TObject; const browser: ICefBrowser;
      const frame: ICefFrame; const request: ICefRequest; isRedirect: Boolean;
      out Result: Boolean);
  private
    { Private declarations }
    FUrl: TidURI;
  public
    { Public declarations }
  end;

var
  frmUsers: TfrmUsers;

implementation

{$R *.dfm}

uses uMain;













// ----------------------------------------------------
// --  UNIT INITIALIZATION - SET GLOBAL CEF OPTIONS  --
// ----------------------------------------------------
procedure TfrmUsers.Chromium1BeforeBrowse(Sender: TObject;
  const browser: ICefBrowser; const frame: ICefFrame;
  const request: ICefRequest; isRedirect: Boolean; out Result: Boolean);
begin
FUrl := TidUri.Create(request.Url);
if FUrl.Protocol = 'close' then
   begin
     FUrl.Free;
     Result := true;
     Close();
   end;
end;

procedure TfrmUsers.FormShow(Sender: TObject);
begin
if frmMain.debug then debug_Toolbar.Visible := true;

Chromium1.Load('http://127.0.0.1:9999/com.service.users.sqr');
end;

procedure TfrmUsers.ToolButton1Click(Sender: TObject);
begin
Chromium1.Browser.ReloadIgnoreCache;
end;

procedure TfrmUsers.ToolButton2Click(Sender: TObject);
begin
//Chromium1.Browser.ShowDevTools;
end;

initialization

{$I cef_initialization.inc}

end.
