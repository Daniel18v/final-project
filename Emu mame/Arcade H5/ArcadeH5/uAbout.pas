unit uAbout;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, AdvSmoothLabel, pngimage, ExtCtrls, AdvSmoothPanel, AdvGlowButton,
  StdCtrls, HTMLabel, AdvGlassButton;

type
  TfrmAbout = class(TForm)
    Image1: TImage;
    lblCaption: TAdvSmoothLabel;
    lblVersion: TAdvSmoothLabel;
    AdvSmoothLabel4: TAdvSmoothLabel;
    btnClose: TAdvGlassButton;
    procedure FormCreate(Sender: TObject);
    procedure AdvGlowButton1Click(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  frmAbout: TfrmAbout;

implementation

{$R *.dfm}

uses uMain;

procedure TfrmAbout.AdvGlowButton1Click(Sender: TObject);
begin
Close();
end;

procedure TfrmAbout.FormCreate(Sender: TObject);
var s, client, company, serial, license_type:string;
    key:integer;
begin
lblCaption.Caption.Text := uMain.APP_NAME;
lblCaption.CaptionShadow.Text := lblCaption.Caption.Text;

lblVersion.Caption.Text := 'v '+uMain.APP_VER_MAJOR+'.'+uMain.APP_VER_MINOR+'.'+uMain.APP_VER_BUILD;
lblVersion.CaptionShadow.Text := lblVersion.Caption.Text;

end;

end.
