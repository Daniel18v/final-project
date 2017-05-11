program ArcadeH5;

uses
  Forms,
  ceflib,
  uMain in 'uMain.pas' {frmMain},
  u_server_management in 'u_server_management.pas',
  ceffilescheme in 'filescheme\ceffilescheme.pas',
  u_str_utils in 'u_str_utils.pas',
  uUserManagement in 'uUserManagement.pas' {frmUsers},
  pc1cipher in 'pc1cipher.pas',
  uAbout in 'uAbout.pas' {frmAbout},
  u_first_run in 'u_first_run.pas',
  uSettings in 'uSettings.pas' {frmSettings},
  mahUSB in 'mahUSB.pas',
  gamepad in 'gamepad.pas',
  u_arcade_functions in 'u_arcade_functions.pas';

{$R *.res}

procedure RegisterSchemes(const registrar: ICefSchemeRegistrar);
begin
  registrar.AddCustomScheme('local', True, True, False);
end;


begin
  CefOnRegisterCustomSchemes := RegisterSchemes;
  CefSingleProcess := False;
  if not CefLoadLibDefault then
    Exit;

  Application.Initialize;
  Application.MainFormOnTaskbar := True;
  Application.Title := 'Arcade H5 Frontend';
  Application.CreateForm(TfrmMain, frmMain);
  Application.CreateForm(TfrmUsers, frmUsers);
  Application.CreateForm(TfrmAbout, frmAbout);
  Application.CreateForm(TfrmSettings, frmSettings);
  Application.Run;
end.
