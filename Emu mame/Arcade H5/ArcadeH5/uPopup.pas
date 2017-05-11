unit uPopup;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, ExtCtrls, cefvcl;

type
  TfrmPopup = class(TForm)
    crm: TChromium;
    Panel1: TPanel;
    procedure FormClose(Sender: TObject; var Action: TCloseAction);
    procedure FormShow(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
    url : string;
  end;

var
  frmPopup: TfrmPopup;

implementation

{$R *.dfm}

procedure TfrmPopup.FormClose(Sender: TObject; var Action: TCloseAction);
begin
FreeAndNil(Self);
end;

procedure TfrmPopup.FormShow(Sender: TObject);
begin
crm.Load(url);
end;

end.
