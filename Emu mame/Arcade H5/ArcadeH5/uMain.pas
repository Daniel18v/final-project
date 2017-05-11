unit uMain;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, IniFiles, AdvSmoothSplashScreen, cefgui, ceflib, cefvcl, Buttons,
  StdCtrls, ExtCtrls, ceffilescheme, u_str_utils, u_first_run, idURI, ComCtrls,
  Menus, pc1cipher, ShellApi, mahUSB,
  gamepad, u_arcade_functions;

type
  TfrmMain = class(TForm)
    splash: TAdvSmoothSplashScreen;
    panel_Debug: TPanel;
    debug_edAddress: TEdit;
    debug_btn_Go: TSpeedButton;
    debug_btnReload: TSpeedButton;
    debug_btnHome: TSpeedButton;
    debug_goForward: TSpeedButton;
    debug_goBack: TSpeedButton;
    crm: TChromium;
    debug_log: TMemo;
    MainMenu1: TMainMenu;
    File1: TMenuItem;
    ools1: TMenuItem;
    Help1: TMenuItem;
    Users1: TMenuItem;
    Exit1: TMenuItem;
    debug_Menu: TMenuItem;
    debug_ShowInspector: TMenuItem;
    About1: TMenuItem;
    StatusBar1: TStatusBar;
    Settings1: TMenuItem;
    N2: TMenuItem;
    ShutdownTimer: TTimer;
    GroupBox1: TGroupBox;
    btn_gamepad_left: TButton;
    btn_gamepad_right: TButton;
    btn_gamepad_up: TButton;
    btn_gamepad_down: TButton;
    JoyTimer_Left: TTimer;
    JoyTimer_Right: TTimer;
    JoyTimer_Up: TTimer;
    JoyTimer_Down: TTimer;
    GamepadTimer: TTimer;
    GamepadDummy: TGamepad;
    GameEndTimer: TTimer;
    GameStartTimer: TTimer;
    N1: TMenuItem;
    Sendexecpregame1: TMenuItem;
    Sendexecgame1: TMenuItem;
    execpostgame1: TMenuItem;
    Sendexecendgame1: TMenuItem;
    procedure FormCreate(Sender: TObject);
    procedure FormClose(Sender: TObject; var Action: TCloseAction);
    procedure crmLoadStart(Sender: TObject; const browser: ICefBrowser;
      const frame: ICefFrame);
//    procedure crmLoadError(Sender: TObject; const browser: ICefBrowser;
//      const frame: ICefFrame; errorCode: Integer; const failedUrl: ustring;
//      var errorText: ustring; out Result: Boolean);
    procedure debug_AddressKeyPress(Sender: TObject; var Key: Char);
    procedure debug_btn_GoClick(Sender: TObject);
    procedure debug_btnHomeClick(Sender: TObject);
    procedure debug_btnReloadClick(Sender: TObject);
    procedure debug_goForwardClick(Sender: TObject);
    procedure debug_goBackClick(Sender: TObject);
    procedure crmConsoleMessage(Sender: TObject; const browser: ICefBrowser;
      message, source: ustring; line: Integer; out Result: Boolean);
 //   procedure crmStatusMessage(Sender: TObject; const browser: ICefBrowser;
 //     const value: ustring; StatusType: TCefHandlerStatusType;
 //     out Result: Boolean);
    procedure debug_ShowInspectorClick(Sender: TObject);
    procedure OpenUserManagementWindow(Sender: TObject);
    procedure About1Click(Sender: TObject);
    procedure Exit1Click(Sender: TObject);
    procedure FormShow(Sender: TObject);
    procedure crmAddressChange(Sender: TObject; const browser: ICefBrowser;
      const frame: ICefFrame; const url: ustring);
    procedure Settings1Click(Sender: TObject);
    procedure userdata_backupClick(Sender: TObject);
    procedure userdata_restoreClick(Sender: TObject);
    procedure FormCloseQuery(Sender: TObject; var CanClose: Boolean);
    procedure ShutdownTimerTimer(Sender: TObject);
    procedure crmBeforeBrowse(Sender: TObject; const browser: ICefBrowser;
      const frame: ICefFrame; const request: ICefRequest; isRedirect: Boolean;
      out Result: Boolean);
    procedure crmBeforePopup(Sender: TObject; const browser: ICefBrowser;
      const frame: ICefFrame; const targetUrl, targetFrameName: ustring;
      var popupFeatures: TCefPopupFeatures; var windowInfo: TCefWindowInfo;
      var client: ICefClient; var settings: TCefBrowserSettings;
      var noJavascriptAccess: Boolean; out Result: Boolean);
    procedure crmLoadEnd(Sender: TObject; const browser: ICefBrowser;
      const frame: ICefFrame; httpStatusCode: Integer);
    procedure crmLoadError(Sender: TObject; const browser: ICefBrowser;
      const frame: ICefFrame; errorCode: Integer; const errorText,
      failedUrl: ustring);
    procedure crmStatusMessage(Sender: TObject; const browser: ICefBrowser;
      const value: ustring);
    procedure crmBeforeContextMenu(Sender: TObject; const browser: ICefBrowser;
      const frame: ICefFrame; const params: ICefContextMenuParams;
      const model: ICefMenuModel);
    procedure crmContextMenuCommand(Sender: TObject; const browser: ICefBrowser;
      const frame: ICefFrame; const params: ICefContextMenuParams;
      commandId: Integer; eventFlags: TCefEventFlags; out Result: Boolean);

    procedure Gamepad_Sendkey(keycode:integer; down:boolean = TRUE; up:boolean = TRUE);
    procedure Gamepad_Move_X(Sender: TObject; value: Integer; Gamepad: TGamepadState);    // left right
    procedure Gamepad_Move_Y(Sender: TObject; value: Integer; Gamepad: TGamepadState);
    procedure GamepadTimerTimer(Sender: TObject);    // top down
//    procedure Gamepad_Move_Z(Sender: TObject; value: Integer; Gamepad: TGamepadState);
//    procedure Gamepad_Move_R(Sender: TObject; value: Integer; Gamepad: TGamepadState);
//    procedure Gamepad_Move_U(Sender: TObject; value: Integer; Gamepad: TGamepadState);
//    procedure Gamepad_Move_V(Sender: TObject; value: Integer; Gamepad: TGamepadState);
    procedure Gamepad_Joystick_X(Sender: TObject; value: Integer; Gamepad: TGamepadState);
    procedure Gamepad_Joystick_Y(Sender: TObject; value: Integer; Gamepad: TGamepadState);
    procedure Gamepad_Joystick_Z(Sender: TObject; value: Integer; Gamepad: TGamepadState);
    procedure Gamepad_Button_Down(Sender: TObject; button: Integer; Gamepad: TGamepadState);
    procedure Gamepad_Button_Press(Sender: TObject; button: Integer; Gamepad: TGamepadState);
    procedure Gamepad_Button_Up(Sender: TObject; button: Integer; Gamepad: TGamepadState);
    procedure JoyTimer_UpTimer(Sender: TObject);
    procedure JoyTimer_DownTimer(Sender: TObject);
    procedure JoyTimer_LeftTimer(Sender: TObject);
    procedure JoyTimer_RightTimer(Sender: TObject);
    procedure FormDeactivate(Sender: TObject);
    procedure FormActivate(Sender: TObject);
    procedure GameEndTimerTimer(Sender: TObject);
    procedure GameStartTimerTimer(Sender: TObject);
    procedure crmJsdialog(Sender: TObject; const browser: ICefBrowser;
      const originUrl, acceptLang: ustring; dialogType: TCefJsDialogType;
      const messageText, defaultPromptText: ustring;
      callback: ICefJsDialogCallback; out suppressMessage, Result: Boolean);
    procedure crmProcessMessageReceived(Sender: TObject;
      const browser: ICefBrowser; sourceProcess: TCefProcessId;
      const message: ICefProcessMessage; out Result: Boolean);
    procedure Sendexecpregame1Click(Sender: TObject);
    procedure Sendexecgame1Click(Sender: TObject);
    procedure execpostgame1Click(Sender: TObject);
    procedure Sendexecendgame1Click(Sender: TObject);

  private
    { Private declarations }
    httpd : cardinal;
    httpdpid : cardinal;
    mysqld : cardinal;
    mysqlpid : cardinal;

    FLoading: Boolean;
    FUrl: TidURI;

    FUsb : TUsbClass;
    procedure UsbIN(ASender : TObject; const ADevType,ADriverName,
                     AFriendlyName : string);
    procedure UsbOUT(ASender : TObject; const ADevType,ADriverName,
                      AFriendlyName : string);

  public
    { Public declarations }
    debug : boolean;

    backend_start:boolean;
    backend_wait :boolean;

    backend_dir :string;
    http_bin : string;
    http_www : string;
    http_exe : string;
    mysql_bin: string;
    mysql_exe: string;

    cfg:TIniFile;

    cabinet_name: string;

    basepath : string;

    allow_close:boolean;                 // if safe to close (or timeout reached)
    disable_timers:boolean;
    shutdownPID:cardinal;                 // pid for shutdown command
//    sql_root_pwd:string;                 // known mysql root password. set on form creation

    gs_sendjs  :boolean;                 // send javascript commands to page
    gs_version :string;                  // game start - emulator version
    gs_emulator:string;                  // emulator
    gs_params  :string;                  // params

    // wait for exe name to quit before sending endgame command to page
    check4exe:string;
    check4exe_post_cmd:string;
    check4exe_post_dir:string;


    gamepads: array[0..1] of TGamepad;
    last_x, last_y, last_z: array[0..1] of integer;
    procedure InitGamepads(max:integer);
  end;

const
     APP_NAME = 'Arcade H5 Frontend';
     APP_VER_MAJOR = '0';
     APP_VER_MINOR = '1';
     APP_VER_BUILD = '0';

     COPYRIGHT = 'Mythweb.ro';

   //  BACKENDDIR = '..\backend\';
   //  SERVER_DIR = '..\backend\bin\';//library\bin\';
   //  HTTPD__EXE = 'project.sakura.httpd.exe';
   //  MYSQLD_EXE = 'project.sakura.mysqld.exe';

     SHUTDOWN_SQL_EXE = 'mysqladmin.exe -u root shutdown -p<rootpassword>';

     //URL_DEFAULT        = See TChromium "crm.DefaultUrl" for active url
     URL_BACKUP_USERDATA  = 'http://127.0.0.1:9999/org.userdata/org.service.userdata.backup.sqr';
     URL_RESTORE_USERDATA = 'http://127.0.0.1:9999/org.userdata/org.service.userdata.restore.sqr';

     URL_SHUTDOWN_PAGE    = 'http://127.0.0.1:9999/arcade.OS_Shutdown';


var
  frmMain: TfrmMain;

implementation

{$R *.dfm}
uses u_server_management, uUserManagement, uAbout, uSettings;

// repeat direction timers
var key_left, key_right, key_up, key_down:array[0..1] of integer;

// --------------------------------------
// --     USB IN/OUT EVENTS            --
// --------------------------------------

procedure TfrmMain.UsbIN(ASender : TObject; const ADevType,ADriverName,
                       AFriendlyName : string);
begin
  if debug then debug_log.Lines.Add('USB Inserted - Device Type = ' + ADevType + #13#10 +
              #9+'Driver Name = ' + ADriverName + #13+#10 +
              #9+'Friendly Name = ' + AFriendlyName);

   GamepadDummy.Update;
   GamepadDummy.MaxDevices;
   if debug then debug_log.Lines.Add(#13#10+'Max gamepads: '+IntToStr(GamepadDummy.MaxDevices));

   InitGamepads(GamepadDummy.MaxDevices);
end;


procedure TfrmMain.UsbOUT(ASender : TObject; const ADevType,ADriverName,
                         AFriendlyName : string);
begin
  if debug then debug_log.Lines.Add('USB Removed - Device Type = ' + ADevType + #13#10 +
              #9+'Driver Name = ' + ADriverName + #13+#10 +
              #9+'Friendly Name = ' + AFriendlyName);

   GamepadDummy.Update;
   GamepadDummy.MaxDevices;
   if debug then debug_log.Lines.Add(#13#10+'Max gamepads: '+IntToStr(GamepadDummy.MaxDevices));

   InitGamepads(GamepadDummy.MaxDevices);
end;

// --------------------------------------
// --     GAMEPAD DETECT AND INIT      --
// --------------------------------------

procedure TfrmMain.InitGamepads(max:integer);
begin
GamepadTimer.Enabled := false;
if Assigned(gamepads[0]) then begin FreeAndNil(gamepads[0]); gamepads[0] := nil; end;
if Assigned(gamepads[1]) then begin FreeAndNil(gamepads[1]); gamepads[1] := nil; end;

if (max > 0) then
   begin
   // init first gamepad
   gamepads[0] := TGamepad.Create;
   gamepads[0].Update;
   gamepads[0].DeviceNr := 0;
   gamepads[0].Update;
   gamepads[0].Tag := 1;

   gamepads[0].OnX := Gamepad_Joystick_X;
   gamepads[0].OnY := Gamepad_Joystick_Y;
   gamepads[0].OnZ := Gamepad_Joystick_Z;
   gamepads[0].OnButtonDown  := Gamepad_Button_Down;
   gamepads[0].OnButtonPress := Gamepad_Button_Press;
   gamepads[0].OnButtonUp    := Gamepad_Button_Up;


   if debug then debug_log.Lines.Add('USB: Gamepad 1 ready');
   end;
if (max > 1) then
   begin
   // init first gamepad
   gamepads[1] := TGamepad.Create;
   gamepads[1].Update;
   gamepads[1].DeviceNr := 1;
   gamepads[1].Update;
   gamepads[1].Tag := 2;

   gamepads[1].OnX := Gamepad_Joystick_X;
   gamepads[1].OnY := Gamepad_Joystick_Y;
   gamepads[1].OnZ := Gamepad_Joystick_Z;
   gamepads[1].OnButtonDown  := Gamepad_Button_Down;
   gamepads[1].OnButtonPress := Gamepad_Button_Press;
   gamepads[1].OnButtonUp    := Gamepad_Button_Up;

   if debug then debug_log.Lines.Add('USB: Gamepad 2 ready');
   end;

if (max > 0) then
   GamepadTimer.Enabled := true;
end;


// --------------------------------------
// --      GAMEPAD HANDLING            --
// --------------------------------------
procedure TfrmMain.Gamepad_Joystick_X(Sender: TObject; value: Integer;
  Gamepad: TGamepadState);
var pid,dir, keychar:string;
    ipid, keycode: integer;
begin
if disable_timers then Exit;

ipid := (Sender as TGamepad).Tag;
pid := IntToStr(ipid);

if (value = 0) then dir := 'Release'
               else
if (value > 0) then dir := 'right'
               else dir := 'left';

if debug then
   begin
   if (value <> 0) then
       debug_log.Lines.Add('Player '+pid+': '+#13#10+
                            #9+ 'Joystick move: '+cfg.ReadString('Player '+pid, 'Joystick.'+dir, 'UNASSIGNED'))
   else
       debug_log.Lines.Add('Player '+pid+': '+#13#10+
                            #9+ 'Joystick move: '+dir);
   end;

if (value <> 0) then
   begin
   keychar := cfg.ReadString('Player '+pid, 'Joystick.'+dir, ' ');
   keycode := Key2Keycode(keychar);
   Gamepad_Sendkey(keycode,true,false);
   if value < 0 then
      begin
      // left
      key_left[ ipid ] := 0;
      JoyTimer_Left.Interval := 400;
      JoyTimer_Left.Enabled := true;
      end
   else
      begin
      // right
      key_right[ ipid ] := 0;
      JoyTimer_Right.Interval := 400;
      JoyTimer_Right.Enabled := true;
      end;
   end
else
   begin
   // get direction for last read value
   if (last_x[ipid] > 0) then dir := 'right'
                         else dir := 'left';
   keychar := cfg.ReadString('Player '+pid, 'Joystick.'+dir, ' ');
   keycode := Key2Keycode(keychar);
   Gamepad_Sendkey(keycode,false,true);

   {if OtherPid(ipid) < 0 then} JoyTimer_Left.Enabled := false;
   {if OtherPid(ipid) < 0 then} JoyTimer_Right.Enabled := false;
   key_left[ipid]  := -1;
   key_right[ipid] := -1;
   end;

last_x[ipid] := value;
end;

procedure TfrmMain.Gamepad_Joystick_Y(Sender: TObject; value: Integer;
  Gamepad: TGamepadState);
var pid,dir, keychar:string;
    ipid, keycode: integer;
begin
if disable_timers then Exit;

ipid := (Sender as TGamepad).Tag;
pid := IntToStr(ipid);

if (value = 0) then dir := 'Release'
               else
if (value > 0) then dir := 'down'
               else dir := 'up';

if debug then
   begin
   if (value <> 0) then
      debug_log.Lines.Add('Player '+pid+': '+#13#10+
                        #9+ 'Joystick move: '+cfg.ReadString('Player '+pid, 'Joystick.'+dir, 'UNASSIGNED'))
   else
      debug_log.Lines.Add('Player '+pid+': '+#13#10+
                        #9+ 'Joystick move: '+dir);
   end;


if (value <> 0) then
   begin
   keychar := cfg.ReadString('Player '+pid, 'Joystick.'+dir, ' ');
   keycode := Key2Keycode(keychar);
   Gamepad_Sendkey(keycode,true,false);
   //if (dir = 'down') then Gamepad_Sendkey(VK_DOWN, true, false)
   //else                   Gamepad_SendKey(VK_UP, true, false);
   if value < 0 then
      begin
      // up
      key_up[ ipid ] := 0;
      JoyTimer_Up.Interval := 400;
      JoyTimer_Up.Enabled := true;
      end
   else
      begin
      // down
      key_down[ ipid ] := 0;
      JoyTimer_Down.Interval := 400;
      JoyTimer_Down.Enabled := true;
      end;
   end
else
   begin
   // get direction for last read value
   if (last_y[ipid] > 0) then dir := 'down'
                         else dir := 'up';
   keychar := cfg.ReadString('Player '+pid, 'Joystick.'+dir, ' ');
   keycode := Key2Keycode(keychar);
   Gamepad_Sendkey(keycode,false,true);
  // if (dir = 'down') then Gamepad_Sendkey(VK_DOWN, false,true)
  // else                   Gamepad_SendKey(VK_UP, false,true);

   {if OtherPid(ipid) < 0 then} JoyTimer_Up.Enabled := false;
   {if OtherPid(ipid) < 0 then} JoyTimer_Down.Enabled := false;
   key_up[ipid]   := -1;
   key_down[ipid] := -1;
   end;

last_y[ipid] := value;
end;

procedure TfrmMain.Gamepad_Joystick_Z(Sender: TObject; value: Integer;
  Gamepad: TGamepadState);
var pid,dir, keychar:string;
    ipid, keycode: integer;
begin
if disable_timers then Exit;

ipid := (Sender as TGamepad).Tag;
pid := IntToStr(ipid);

if (value = 0) then dir := 'Release'
               else
if (value > 0) then dir := 'z-down'
               else dir := 'z-up';

if debug then
   begin
   if (value <> 0) then
      debug_log.Lines.Add('Player '+pid+': '+#13#10+
                        #9+ 'Joystick move: '+cfg.ReadString('Player '+pid, 'Joystick.'+dir, 'UNASSIGNED'))
   else
      debug_log.Lines.Add('Player '+pid+': '+#13#10+
                        #9+ 'Joystick move: '+dir);
   end;


if (value <> 0) then
   begin
   keychar := cfg.ReadString('Player '+pid, 'Joystick.'+dir, ' ');
   keycode := Key2Keycode(keychar);
   Gamepad_Sendkey(keycode,true,false);
   end
else
   begin
   // get direction for last read value
   if (last_z[ipid] > 0) then dir := 'z-down'
                         else dir := 'z-up';
   keychar := cfg.ReadString('Player '+pid, 'Joystick.'+dir, ' ');
   keycode := Key2Keycode(keychar);
   Gamepad_Sendkey(keycode,false,true);
   end;

last_z[ipid] := value;
end;

procedure TfrmMain.Gamepad_Button_Down(Sender: TObject; button: Integer;
  Gamepad: TGamepadState);
var pid, keychar:string;
    ipid, keycode: integer;
begin
if disable_timers then Exit;

pid := IntToStr((Sender as TGamepad).Tag);

keychar := cfg.ReadString('Player '+pid, 'Joystick.button.'+IntToStr(button), ' ');
keycode := Key2Keycode(keychar);
Gamepad_Sendkey(keycode,true,false);

if debug then
   debug_log.Lines.Add('Player '+pid+': '+#13#10+
                        #9+ 'Joystick button '+IntToStr(button)+' down: '+cfg.ReadString('Player '+pid, 'Joystick.button.'+IntToStr(button), 'BTN '+IntToStr(button)+' UNASSIGNED'));
end;

procedure TfrmMain.Gamepad_Button_Press(Sender: TObject; button: Integer;
  Gamepad: TGamepadState);
var pid, keychar:string;
    keycode: integer;
begin
if disable_timers then Exit;

// we don't use button press events as they are handled by button up + button down events
Exit;

pid := IntToStr((Sender as TGamepad).Tag);

keychar := cfg.ReadString('Player '+pid, 'Joystick.button.'+IntToStr(button), ' ');
keycode := Key2Keycode(keychar);
Gamepad_Sendkey(keycode,true,true);

if debug then
   debug_log.Lines.Add('Player '+pid+': '+#13#10+
                        #9+ 'Joystick button '+IntToStr(button)+' press: '+cfg.ReadString('Player '+pid, 'Joystick.button.'+IntToStr(button), 'BTN '+IntToStr(button)+' UNASSIGNED'));

end;

procedure TfrmMain.Gamepad_Button_Up(Sender: TObject; button: Integer;
  Gamepad: TGamepadState);
var pid, keychar:string;
    keycode: integer;
begin
if disable_timers then Exit;

pid := IntToStr((Sender as TGamepad).Tag);

keychar := cfg.ReadString('Player '+pid, 'Joystick.button.'+IntToStr(button), ' ');
keycode := Key2Keycode(keychar);
Gamepad_Sendkey(keycode,false,true);

if debug then
   debug_log.Lines.Add('Player '+pid+': '+#13#10+
                        #9+ 'Joystick button '+IntToStr(button)+' up: '+cfg.ReadString('Player '+pid, 'Joystick.button.'+IntToStr(button), 'BTN '+IntToStr(button)+' UNASSIGNED'));

end;


// --------------------------------------
// --        MAIN FORM CREATE          --
// --------------------------------------
procedure TfrmMain.FormCreate(Sender: TObject);
var s:string;
    key:integer;
    b:boolean;
    backend:TIniFile;
begin
// set form activate / deactivate events to application as well so we know we lost app focus
Application.OnActivate := FormActivate;
Application.OnDeactivate := FormDeactivate;

allow_close:= false;
disable_timers := false;

key_left[0]  := -1; key_left[1]  := -1;
key_right[0] := -1; key_right[1] := -1;
key_up[0]    := -1; key_up[1]    := -1;
key_down[0]  := -1; key_down[1]  := -1;

//sql_root_pwd := 'rootpassword';      // see below for base.settings override

basepath := ExtractFilePath(Application.ExeName);
Caption := APP_NAME+' v'+APP_VER_MAJOR+'.'+APP_VER_MINOR+'.'+APP_VER_BUILD+' - '+COPYRIGHT;
Application.Title := Caption;

// load backend settings before base settings
backend := TIniFile.Create(basepath+'backend.settings');
backend_dir := ExpandFileNameRelBaseDir(backend.ReadString('BACKEND', 'base.path', '..\backend\'), basepath);
http_bin    := ExpandFileNameRelBaseDir(backend.ReadString('BACKEND', 'http.bin',  '..\backend\http\'), basepath);
http_www    := ExpandFileNameRelBaseDir(backend.ReadString('BACKEND', 'http.www',  '..\backend\http\htdocs'), basepath);
mysql_bin   := ExpandFileNameRelBaseDir(backend.ReadString('BACKEND', 'mysql.bin', '..\backend\mysql\'), basepath);

http_exe    := backend.ReadString('BACKEND', 'http.exe',  'ro.mythweb.httpd.exe');
mysql_exe   := backend.ReadString('BACKEND', 'mysql.exe', 'ro.mythweb.mysqld.exe');

backend.Free;

// load base settings
cfg := TIniFile.Create(basepath+'base.settings');
splash.Show();
splash.BeginUpdate;
//splash.BasicProgramInfo.ProgramName.Text := APP_NAME;
splash.EndUpdate;
sleep(100);


cabinet_name := cfg.ReadString('ARCADE', 'cabinet.name', 'Arcade Cabinet');

backend_start := cfg.ReadBool('BACKEND', 'builtin.start', true);
// backend_wait is used if backend_start is false for the purpose of waiting for the
// specified processes to be initialized by the OS
backend_wait  := cfg.ReadBool('BACKEND', 'wait.daemons', false);

// preference - send js commands to browser
gs_sendjs := cfg.ReadBool('ARCADE', 'browser.send_js', false);
if gs_sendjs then debug_log.Lines.Add('Send JS commands active!');


{
s:= cfg.ReadString('SQL', 'root.password', '');
if s <> '' then
   begin
   sql_root_pwd := pc1decode(s, 'MYSQL_ROOT_PASSWORD');
   end;
}

with splash.ListItems.Add do
     begin
     BeginUpdate;
     HTMLText := '<IMG src="0"><FONT color="clWhite">Initializing...</FONT><br>';
     EndUpdate;
     end;

debug := cfg.ReadBool('MAIN','ro.mythweb.arcadeh5.debug.enabled',false);
panel_Debug.Visible := debug;
debug_log.Visible := debug;
debug_Menu.Visible := debug; debug_Menu.Enabled := debug;
StatusBar1.Visible := debug;
if (debug) then
   begin
   splash.ProgressBar.Position := splash.ProgressBar.Position + 5;
   with splash.ListItems.Add do
        begin
        BeginUpdate;
        HtmlText := '<img src="0"><FONT color="clRed"><B>DEBUG MODE ACTIVATED!</B></FONT><br>';
        EndUpdate;
        end;
   end;

b:= cfg.ReadBool('MAIN', 'ro.mythweb.arcadeh5.borders.enabled', false);
if b then frmMain.BorderStyle := bsSizeable
     else
          begin
          frmMain.Menu := nil; // we don't show the menu if full screen
          frmMain.BorderStyle := bsNone;
          end;

// check setup status
if backend_start then
   begin
   if not CheckConfigFiles() then
      begin
      splash.ProgressBar.Position := splash.ProgressBar.Position + 5;
      with splash.ListItems.Add do
          begin
          BeginUpdate;
          HtmlText := '<img src="0"><FONT color="clFuchsia">Configuring runtime services...</FONT><br>';
          EndUpdate;
          end;
      if not SetupConfigFiles() then
         begin
         ShowMessage('Unable to write service configuration files...');
         allow_close := true;
         Close();
         exit;
         end;
      end;
   end;

sleep(5);
splash.ProgressBar.Position := splash.ProgressBar.Position + 10;
with splash.ListItems.Add do
     begin
     BeginUpdate;
     HTMLText := '<IMG src="0"><FONT color="clWhite">Initializing hardware services...</FONT><br>';
     EndUpdate;
     end;

FUsb := TUsbClass.Create;
FUsb.OnUsbInsertion := UsbIN;
FUsb.OnUsbRemoval := UsbOUT;


sleep(5);
splash.ProgressBar.Position := splash.ProgressBar.Position + 10;
with splash.ListItems.Add do
     begin
     BeginUpdate;
     HTMLText := '<IMG src="0"><FONT size="10" color="clWhite">Starting arcade services...</FONT><br>';
     EndUpdate;
     end;

sleep(5);
splash.ProgressBar.Position := splash.ProgressBar.Position + 5;
with splash.ListItems.Add do
     begin
     BeginUpdate;
     HTMLText := '<IMG src="0"><FONT size="10" color="clWhite">Starting services... [Apache Daemon]</FONT><br>';
     EndUpdate;
     end;

if backend_start then
   begin
  // while processExists(HTTPD__EXE, httpd) do KillProcess(httpd);
   while processExists(http_exe, httpd) do KillProcess(httpd);
   // start server
  // ExecBackgroundProcess(basepath+SERVER_DIR+HTTPD__EXE, 1000, basepath+SERVER_DIR, httpdpid);
   ExecBackgroundProcess(http_bin+'bin\'+http_exe, 1000, http_bin+'bin\', httpdpid);
   end
else
if backend_wait then
   begin
  // s := cfg.ReadString('BACKEND', 'wait.http', 'ro.mythweb.ro.httpd.exe');
   s := http_exe;
   if trim(s) <> '' then
      begin
      while not processExists(s, httpd) do begin sleep(5); Application.ProcessMessages; end;
      end;
   end;

sleep(5);
splash.ProgressBar.Position := splash.ProgressBar.Position + 5;
with splash.ListItems.Add do
     begin
     BeginUpdate;
     HTMLText := '<IMG src="0"><FONT size="10" color="clWhite">Starting services... [mySql Daemon]</FONT><br>';
     EndUpdate;
     end;

if backend_start then
   begin
 //  while processExists(MYSQLD_EXE, mysqld) do KillProcess(mysqld);
   while processExists(mysql_exe, mysqld) do KillProcess(mysqld);
   ExecBackgroundProcess(mysql_bin+'bin\'+mysql_exe,1000, mysql_bin+'bin\', mysqlpid);
   end
else
if backend_wait then
   begin
//   s := cfg.ReadString('BACKEND', 'wait.mysql', 'ro.mythweb.ro.httpd.exe');
   s := mysql_exe;
   if trim(s) <> '' then
      begin
      while not processExists(s, mysqld) do begin sleep(5); Application.ProcessMessages; end;
      end;
   end;


sleep(5);
splash.ProgressBar.Position := splash.ProgressBar.Position + 5;
with splash.ListItems.Add do
     begin
     BeginUpdate;
     HTMLText := '<IMG src="0"><FONT size="10" color="clWhite">Activating joysticks and buttons...</FONT><br>';
     EndUpdate;
     end;

GamepadDummy.Update;
GamepadDummy.MaxDevices;
if debug then debug_log.Lines.Add(#13#10+'Max gamepads: '+IntToStr(GamepadDummy.MaxDevices)+#13#10);
InitGamepads(GamepadDummy.MaxDevices);

GamepadTimer.Enabled := true;

sleep(5);
with splash.ListItems.Add do
     begin
     BeginUpdate;
     HTMLText := '<IMG src="0"><FONT size="10" color="clWhite">Starting <B>'+cabinet_name+'</B>...</FONT><br>';
     EndUpdate;
     end;
sleep(5);

splash.ProgressBar.Position := splash.ProgressBar.Position + 5;
sleep(5);
splash.ProgressBar.Position := splash.ProgressBar.Position + 5;


while splash.ProgressBar.Position < 100 do
      begin
      splash.ProgressBar.Position := splash.ProgressBar.Position + 1;
      sleep(5);
      end;

sleep(1000);

//crm.Browser.MainFrame.LoadUrl('http://127.0.0.1:9999/');

end;


procedure TfrmMain.FormShow(Sender: TObject);
var s:string;
    w,h:integer;
begin

w := cfg.ReadInteger('MAIN', 'app.window.min_width', 0);
h := cfg.ReadInteger('MAIN', 'app.window.min_height', 0);

Constraints.MinWidth := w;
Constraints.MinHeight := h;


w := cfg.ReadInteger('MAIN', 'app.window.width', frmMain.ClientWidth);
h := cfg.ReadInteger('MAIN', 'app.window.height', frmMain.ClientHeight);

ClientWidth := w;
ClientHeight := h;

s := cfg.ReadString('MAIN', 'app.window.state', 'normal');
if s = 'maximized' then frmMain.WindowState := wsMaximized;

// if borders are not shown, we always maximize
if frmMain.BorderStyle = bsNone then frmMain.WindowState := wsMaximized;

end;

// -----------------------------------------
// --        Open User Management         --
// -----------------------------------------
procedure TfrmMain.OpenUserManagementWindow(Sender: TObject);
begin
//frmUsers.ShowModal;
end;

// -----------------------------------------
// --           DEBUG - SEND JS COMMANDS  --
// -----------------------------------------
procedure TfrmMain.Sendexecendgame1Click(Sender: TObject);
begin
if not gs_sendjs then debug_log.Lines.Add('SEND JS NOT ACTIVE IN SETTINGS');

if gs_sendjs then crm.Browser.MainFrame.ExecuteJavaScript('ArcadeStart.exec_endgame();', crm.Browser.MainFrame.Url, 0 );
end;

procedure TfrmMain.Sendexecgame1Click(Sender: TObject);
begin
if not gs_sendjs then debug_log.Lines.Add('SEND JS NOT ACTIVE IN SETTINGS');

if gs_sendjs then crm.Browser.MainFrame.ExecuteJavaScript('ArcadeStart.exec_game();', crm.Browser.MainFrame.Url, 0 );
end;

procedure TfrmMain.Sendexecpregame1Click(Sender: TObject);
begin
if not gs_sendjs then debug_log.Lines.Add('SEND JS NOT ACTIVE IN SETTINGS');

if gs_sendjs then crm.Browser.MainFrame.ExecuteJavaScript('ArcadeStart.exec_pregame();', crm.Browser.MainFrame.Url, 0 );
end;


procedure TfrmMain.execpostgame1Click(Sender: TObject);
begin
if not gs_sendjs then debug_log.Lines.Add('SEND JS NOT ACTIVE IN SETTINGS');

if gs_sendjs then crm.Browser.MainFrame.ExecuteJavaScript('ArcadeStart.exec_postgame();', crm.Browser.MainFrame.Url, 0 );
end;

// -----------------------------------------
// --           Open Settings             --
// -----------------------------------------
procedure TfrmMain.Settings1Click(Sender: TObject);
begin
frmSettings.ShowModal;
end;

procedure TfrmMain.debug_ShowInspectorClick(Sender: TObject);
begin
if debug then debug_log.Lines.Add('Opening Dev Tools...');

//if crm.Browser <> nil then
//    crm.Browser.ShowDevTools;
end;

procedure TfrmMain.Exit1Click(Sender: TObject);
begin
Close;
end;

// --------------------------------------
// -- MAIN FORM  ACTIVED / DEACTIVATED --
// --------------------------------------
procedure TfrmMain.FormActivate(Sender: TObject);
begin
disable_timers := false;
if debug then debug_log.Lines.Add('WINDOW ACTIVATED: Enabling timers...');

end;

procedure TfrmMain.FormDeactivate(Sender: TObject);
begin
disable_timers := true;
if debug then debug_log.Lines.Add('WINDOW DEACTIVATED: Disabling timers...');
end;


// --------------------------------------
// --        MAIN FORM DESTROY         --
// --------------------------------------
procedure TfrmMain.FormClose(Sender: TObject; var Action: TCloseAction);
begin
// release FUsb object
FUsb.Free;
//FreeAndNil(FUsb);
end;


procedure TfrmMain.FormCloseQuery(Sender: TObject; var CanClose: Boolean);
begin
if not allow_close then
   begin
   CanClose := false;
 //  crm.Browser.MainFrame.LoadUrl(URL_SHUTDOWN_PAGE);
   crm.Load(URL_SHUTDOWN_PAGE);
   sleep(100);  // allow time for browser to load shutdown page

   //attempt gracefull shutdown for mysql daemon
//   ExecBackgroundProcess(basepath+SERVER_DIR+SHUTDOWN_SQL_EXE,3000, basepath+SERVER_DIR, shutdownPID);
   ExecBackgroundProcess(mysql_bin+'bin\'+SHUTDOWN_SQL_EXE,3000, mysql_bin+'bin\', shutdownPID);

   // enable shutdown timer
   ShutdownTimer.Enabled := true;
   end
else
   begin
   CanClose := true;

   cfg.Free;
   end;
end;

procedure TfrmMain.ShutdownTimerTimer(Sender: TObject);
begin
ShutdownTimer.Enabled := false;

if backend_start then
   begin
//   if processExists(HTTPD__EXE,httpd) then KillProcess(httpdpid);
//   if processExists(MYSQLD_EXE,mysqld) then KillProcess(mysqlpid);
   if processExists(http_exe,httpd) then KillProcess(httpdpid);
   if processExists(mysql_exe,mysqld) then KillProcess(mysqlpid);
   // make sure all child processes are killed
//   while processExists(HTTPD__EXE, httpd) do KillProcess(httpd);
//   while processExists(MYSQLD_EXE, mysqld) do KillProcess(mysqld);
   while processExists(http_exe, httpd) do KillProcess(httpd);
   while processExists(mysql_exe, mysqld) do KillProcess(mysqld);
   end;

cfg.WriteString('MAIN','log.last_run.date',DateToStr(Date));
cfg.WriteString('MAIN','log.last_run.time',TimeToStr(Time));

if frmMain.WindowState = wsNormal then cfg.WriteString('MAIN', 'app.window.state', 'normal')
                                  else cfg.WriteString('MAIN', 'app.window.state', 'maximized');

if frmMain.WindowState = wsMaximized then frmMain.WindowState := wsNormal;

if debug then debug_log.Lines.SaveToFile(ChangeFileExt(Application.ExeName,'.log'));

cfg.WriteInteger('MAIN', 'app.window.width', frmMain.ClientWidth);
cfg.WriteInteger('MAIN', 'app.window.height', frmMain.ClientHeight);

// allow shutdown
allow_close := true;
Close();
end;


// --------------------------------------------
// --        BEGIN BROWSER LOAD PAGE         --
// --------------------------------------------
procedure TfrmMain.crmLoadStart(Sender: TObject; const browser: ICefBrowser;
  const frame: ICefFrame);
begin
//  if (browser <> nil) and (browser.GetWindowHandle = crm.BrowserHandle) and ((frame = nil) or (frame.IsMain)) then
    FLoading := True;
end;


procedure TfrmMain.crmProcessMessageReceived(Sender: TObject;
  const browser: ICefBrowser; sourceProcess: TCefProcessId;
  const message: ICefProcessMessage; out Result: Boolean);
begin
if debug then
   begin
   debug_log.Lines.Add('Process Message: '+message.Name);
   end;
end;

procedure TfrmMain.crmBeforeBrowse(Sender: TObject; const browser: ICefBrowser;
  const frame: ICefFrame; const request: ICefRequest; isRedirect: Boolean;
  out Result: Boolean);
begin
if not frame.IsMain then Exit; // do not do anything if not in main frame

//if navType = NAVTYPE_LINKCLICKED then debug_log.Lines.Add('bef browse: ' + request.Url);
if debug then debug_log.Lines.Add('frame url: '+frame.Url);

FUrl := TidUri.Create(request.Url);
if debug then
   begin
   debug_log.Lines.Add('protocol: '+Furl.Protocol);
   debug_log.Lines.Add('host: '+Furl.Host);
   debug_log.Lines.Add('path: '+Furl.Path);
   debug_log.Lines.Add('param: '+Furl.Params);
   debug_log.Lines.Add('document: '+Furl.Document);
   end;

{
if debug AND (Furl.Params = 'help=1') then
   begin
   Result := true;
   if debug then debug_log.Lines.Add('handled url detected. canceled default handling');
   end;

if FUrl.Protocol = 'arcade' then
   begin
   // ARCADE H5 CONTROL LINKS
   if debug then debug_log.Lines.Add('canceled default handling. Running arcade handler...');

   // arcade protocol is always handled. if command (hostname) is unknown nothing will happen
   Result := true;

   if  Furl.Host = 'close' then
       begin
       // shutdown
       if debug then debug_log.Lines.Add('Arcade handler: close application!');
       Exit1Click(Sender);
       end;

   if Furl.Host = 'start' then
      begin
      // start game
      if debug then debug_log.Lines.Add('Arcade handler: start game: '+Furl.Path+' , '+Furl.Document+' , '+Furl.Params);
      gs_version  := Furl.Path;
      gs_emulator := Furl.Document;
      gs_params   := Furl.Params;

      GameStartTimer.Enabled := true;

      //ArcadeH5_StartGame(Furl.Path, Furl.Document, Furl.Params);
      end;

   end;
}

{if Furl.Protocol = 'print' then
   begin
   Result := true;
   if debug then debug_log.Lines.Add('canceled default handling. Running print handler...');

   frmPrint.url_for_print := StringReplace(request.Url,'print://','http://',[rfReplaceAll,rfIgnoreCase]);
   frmPrint.ShowModal;
   end;    }

FUrl.Free;
//if Result then crm.Browser.StopLoad;
end;


procedure TfrmMain.crmBeforeContextMenu(Sender: TObject;
  const browser: ICefBrowser; const frame: ICefFrame;
  const params: ICefContextMenuParams; const model: ICefMenuModel);
begin
model.Clear;
model.AddItem(26500, '© '+COPYRIGHT);
end;

procedure TfrmMain.crmContextMenuCommand(Sender: TObject;
  const browser: ICefBrowser; const frame: ICefFrame;
  const params: ICefContextMenuParams; commandId: Integer;
  eventFlags: TCefEventFlags; out Result: Boolean);
begin
if commandId = 26500 then
  begin
    Result := True;
    frmAbout.ShowModal;
  end;

end;

procedure TfrmMain.crmJsdialog(Sender: TObject; const browser: ICefBrowser;
  const originUrl, acceptLang: ustring; dialogType: TCefJsDialogType;
  const messageText, defaultPromptText: ustring; callback: ICefJsDialogCallback;
  out suppressMessage, Result: Boolean);
begin
if debug then
   begin
   debug_log.Lines.Add('JS DIALOG REQUEST');
   debug_log.Lines.Add(#9+'Origin URL: '+originUrl);
   if (dialogType = JSDIALOGTYPE_ALERT) then debug_log.Lines.Add(#9+'Dialog type: ALERT!')
      else
   if (dialogType = JSDIALOGTYPE_CONFIRM) then debug_log.Lines.Add(#9+'Dialog type: CONFIRMATION')
      else debug_log.Lines.Add(#9+'Dialog type: PROMPT');

   debug_log.Lines.Add(#9+'Message: '+messageText);
   if (dialogType = JSDIALOGTYPE_PROMPT) then debug_log.Lines.Add(#9+'Message: '+defaultPromptText);
   end;

end;

procedure TfrmMain.crmBeforePopup(Sender: TObject; const browser: ICefBrowser;
  const frame: ICefFrame; const targetUrl, targetFrameName: ustring;
  var popupFeatures: TCefPopupFeatures; var windowInfo: TCefWindowInfo;
  var client: ICefClient; var settings: TCefBrowserSettings;
  var noJavascriptAccess: Boolean; out Result: Boolean);
begin
Result:=true;
end;

procedure TfrmMain.crmStatusMessage(Sender: TObject; const browser: ICefBrowser;
  const value: ustring);
begin
StatusBar1.SimpleText := value;
end;

procedure TfrmMain.crmConsoleMessage(Sender: TObject;
  const browser: ICefBrowser; message, source: ustring; line: Integer;
  out Result: Boolean);
var s, s1,s2:string;
begin
if debug then debug_log.Lines.Add('con '+IntToStr(line)+': '+message);
s := Copy(message,0,9);
if s = 'ArcadeH5:' then
   begin
   s := Copy(message,10,Length(message));
   if debug then debug_log.Lines.Add('con '+IntToStr(line)+': '+s);
   FUrl := TidUri.Create(s);

   if Furl.Host = 'close' then
       begin
       // shutdown
       if debug then debug_log.Lines.Add('Arcade handler: close application!');
       Exit1Click(Sender);
       end;

   if Furl.Host = 'restart' then
      begin
      // requested system restart
      if debug then debug_log.Lines.Add('Arcade handler: restart machine!');

      s1 := cfg.ReadString('MAIN', 'app.restart.cmd', 'ShutdownH5.exe');  // program
      s2 := cfg.ReadString('MAIN', 'app.restart.param', '-restart');        // params
      ShellExecute(0,'open',PChar(s1), PChar(s2),'',SW_SHOWNORMAL);
      Exit1Click(Sender);
      end;

   if Furl.Host = 'shutdown' then
      begin
      // requested system restart
      if debug then debug_log.Lines.Add('Arcade handler: shutdown machine!');

      s1 := cfg.ReadString('MAIN', 'app.shutdown.cmd', 'ShutdownH5.exe');  // program
      if (FileExists(basepath+s1)) then s1 := basepath + s1;
      s2 := cfg.ReadString('MAIN', 'app.shutdown.param', '-shutdown');        // params
      ShellExecute(0,'open',PChar(s1), PChar(s2),'',SW_SHOWNORMAL);
      Exit1Click(Sender);
      end;

   if Furl.Host = 'start' then
      begin
      // start game
      //if debug then debug_log.Lines.Add('Arcade handler: start game: '+Furl.Path+' , '+Furl.Document+' , '+Furl.Params);
      if debug then debug_log.Lines.Add('Console log: start game: '+Furl.Path+' , '+Furl.Document+' , '+Furl.Params);
      gs_version  := Furl.Path;
      gs_emulator := Furl.Document;
      gs_params   := Furl.Params;

      GameStartTimer.Enabled := true;

      //ArcadeH5_StartGame(Furl.Path, Furl.Document, Furl.Params);
      end;

   if debug then
      begin
      debug_log.Lines.Add('protocol: '+Furl.Protocol);
      debug_log.Lines.Add('host: '+Furl.Host);
      debug_log.Lines.Add('path: '+Furl.Path);
      debug_log.Lines.Add('param: '+Furl.Params);
      debug_log.Lines.Add('document: '+Furl.Document);
      end;


   FUrl.Free;
   end;
end;

// ------------------------------------------
// --        END BROWSER LOAD PAGE         --
// ------------------------------------------

procedure TfrmMain.crmLoadEnd(Sender: TObject; const browser: ICefBrowser;
  const frame: ICefFrame; httpStatusCode: Integer);
begin
// if (browser <> nil) and (browser.GetWindowHandle = crm.BrowserHandle) and ((frame = nil) or (frame.IsMain)) then
    FLoading := False;

end;

// ------------------------------------------
// --         BROWSER ERROR PAGES          --
// ------------------------------------------
procedure TfrmMain.crmLoadError(Sender: TObject; const browser: ICefBrowser;
  const frame: ICefFrame; errorCode: Integer; const errorText,
  failedUrl: ustring);

var S, ef:String;
begin
if (browser <> nil) then
    begin
    FLoading := False;

    ef := 'error.base';
    if errorCode = -102 then ef:='httpd.daemon';


    S := file_get_contents(http_www+'org.error\'+ef);
    S := stringReplace(S, '[ERRORCODE]', IntToStr(errorCode), [rfReplaceAll]);
    S := stringReplace(S, '[ERRORTEXT]', errorText, [rfReplaceAll]);
    S := stringReplace(S, '[FAILEDURL]', failedUrl, [rfReplaceAll]);

  //  frame.LoadString(S,path_to_uri(BACKENDDIR+'htdocs\org.error\'));

 //   Result := true;

    if debug then
       begin
    debug_log.Lines.Add('ERROR #'+IntToStr(errorCode)+': '+errorText);
    debug_log.Lines.Add('       Failed URL: '+failedUrl);
     end;

   // S := file_get_contents(basepath+'error.html');
   // frame.LoadString(S,failedUrl);
    end;

end;

procedure TfrmMain.About1Click(Sender: TObject);
begin
frmAbout.ShowModal;
end;

procedure TfrmMain.userdata_backupClick(Sender: TObject);
begin
crm.Browser.MainFrame.LoadUrl(URL_BACKUP_USERDATA);
end;

procedure TfrmMain.userdata_restoreClick(Sender: TObject);
begin
crm.Browser.MainFrame.LoadUrl(URL_RESTORE_USERDATA);
end;

// ----------------------------------------------
// --         CHANGE ADDRESS IN URLBAR         --
// ----------------------------------------------
procedure TfrmMain.crmAddressChange(Sender: TObject;
  const browser: ICefBrowser; const frame: ICefFrame; const url: ustring);
begin
  if (browser.Host.WindowHandle = crm.Browser.Host.WindowHandle) and ((frame = nil) or (frame.IsMain)) then
    begin
    debug_edAddress.Text := url;
    if debug then debug_log.Lines.Add('ADDRESS CHANGE: '+url);

    end;
end;

// -----------------------------------------------------
// --         RESPOND TO ENTER IN ADDRESS BAR         --
// -----------------------------------------------------
procedure TfrmMain.debug_AddressKeyPress(Sender: TObject; var Key: Char);
begin
  if Key = #13 then
  begin
    if crm.Browser <> nil then
    begin
      crm.Browser.MainFrame.LoadUrl(debug_edAddress.Text);
      Abort;
    end;
  end;
end;

// -----------------------------------------------
// --         GO TO ENTERED ADDRESS BAR         --
// -----------------------------------------------
procedure TfrmMain.debug_btn_GoClick(Sender: TObject);
begin
if crm.Browser <> nil then
   crm.Browser.MainFrame.LoadUrl(debug_edAddress.Text);
end;

// --------------------------------------
// --         GO TO HOME PAGE          --
// --------------------------------------
procedure TfrmMain.debug_btnHomeClick(Sender: TObject);
begin
if crm.Browser <> nil then
   crm.Browser.MainFrame.LoadUrl(crm.DefaultUrl);
end;

// --------------------------------------
// --       RELOAD CURRENT PAGE        --
// --------------------------------------
procedure TfrmMain.debug_btnReloadClick(Sender: TObject);
begin
if crm.Browser <> nil then
   if FLoading then crm.Browser.StopLoad
               else crm.Browser.Reload;
end;

// --------------------------------------
// --      GO BACK TO PREV PAGE        --
// --------------------------------------
procedure TfrmMain.debug_goBackClick(Sender: TObject);
begin
if crm.Browser.CanGoBack then
   crm.Browser.GoBack;
end;

// --------------------------------------
// --     GO FORWARD TO NEXTPAGE       --
// --------------------------------------
procedure TfrmMain.debug_goForwardClick(Sender: TObject);
begin
if crm.Browser.CanGoForward then
   crm.Browser.GoForward;
end;



// --------------------------------------
// -- TIMER TO EXECUTE EMULATOR & GAME --
// --------------------------------------
procedure TfrmMain.GameStartTimerTimer(Sender: TObject);
begin
GameStartTimer.Enabled := false;
if debug then debug_log.Lines.Add('GAME START TIMER: Starting game '+gs_version+' | '+ gs_emulator + ' | ' + gs_params);

ArcadeH5_StartGame(gs_version, gs_emulator, gs_params);

gs_version  := '';
gs_emulator := '';
gs_params   := '';
end;


// --------------------------------------
// -- TIMER TO CHECK FOR EMULATOR EXIT --
// --------------------------------------
procedure TfrmMain.GameEndTimerTimer(Sender: TObject);
var exePID:Cardinal;
begin
// if no exe name to check for do nothing
if (check4exe = '') then
   begin
   if debug then debug_log.Lines.Add('Check4Exe - No EXE Name set! Unable to wait for exit');

   GameEndTimer.Enabled := false;
   Exit;
   end;

// this timer is activated when a game is started
if not processExists(check4exe, exePID) then
   begin
   // process no longer exists - game end
   GameEndTimer.Enabled := false;

   // postgame start
   if Trim(check4exe_post_cmd) <> '' then
         begin
         if debug then debug_log.Lines.Add('Check4Exe - Starting post game command');
         if gs_sendjs then crm.Browser.MainFrame.ExecuteJavaScript('ArcadeStart.exec_postgame();', crm.Browser.MainFrame.Url, 0 );
         ExecuteAndWait(check4exe_post_cmd, check4exe_post_dir);
         end;

   if debug then debug_log.Lines.Add('Check4Exe - Sending end game signal');
   if gs_sendjs then crm.Browser.MainFrame.ExecuteJavaScript('ArcadeStart.exec_endgame();', crm.Browser.MainFrame.Url, 0 );

   check4exe := '';
   check4exe_post_cmd := '';
   check4exe_post_dir := '';
   end;

end;

// --------------------------------------
// --          GAMEPAD TIMERS          --
// --------------------------------------

procedure TfrmMain.GamepadTimerTimer(Sender: TObject);
begin
if not disable_timers then
   begin
   if Assigned(gamepads[0]) then begin gamepads[0].Update; end;
   if Assigned(gamepads[1]) then begin gamepads[1].Update; end;
   end;
end;

procedure TfrmMain.JoyTimer_UpTimer(Sender: TObject);
var pid, keychar:string;
    ipid, keycode: integer;
begin

if key_up[0] >= 0 then ipid := 0 else ipid := 1;
pid := IntToStr(ipid);

keychar := cfg.ReadString('Player '+pid, 'Joystick.up', ' ');
keycode := Key2Keycode(keychar);


Gamepad_SendKey(keycode);

key_up[ipid] := key_up[ipid]+1;
if key_up[ipid] = 1 then JoyTimer_Up.Interval := 200
   else
if key_up[ipid] = 5 then JoyTimer_Up.Interval := 100
   else
if key_up[ipid] = 15 then JoyTimer_Up.Interval := 50
   else
if key_up[ipid] = 30 then JoyTimer_Up.Interval := 20
   else
if key_up[ipid] > 100 then key_up[ipid] := 31;

end;

procedure TfrmMain.JoyTimer_DownTimer(Sender: TObject);
var pid, keychar:string;
    ipid, keycode: integer;
begin

if key_down[0] >= 0 then ipid := 0 else ipid := 1;
pid := IntToStr(ipid);

keychar := cfg.ReadString('Player '+pid, 'Joystick.down', ' ');
keycode := Key2Keycode(keychar);


Gamepad_SendKey(keycode);

key_down[ipid] := key_down[ipid]+1;
if key_down[ipid] = 1 then JoyTimer_Down.Interval := 200
   else
if key_down[ipid] = 5 then JoyTimer_Down.Interval := 100
   else
if key_down[ipid] = 15 then JoyTimer_Down.Interval := 50
   else
if key_down[ipid] = 30 then JoyTimer_Down.Interval := 20
   else
if key_down[ipid] > 100 then key_down[ipid] := 31;

end;



procedure TfrmMain.JoyTimer_LeftTimer(Sender: TObject);
var pid, keychar:string;
    ipid, keycode: integer;
begin

if key_left[0] >= 0 then ipid := 0 else ipid := 1;
pid := IntToStr(ipid);

keychar := cfg.ReadString('Player '+pid, 'Joystick.left', ' ');
keycode := Key2Keycode(keychar);


Gamepad_SendKey(keycode);

key_left[ipid] := key_left[ipid]+1;
if key_left[ipid] = 1 then JoyTimer_Left.Interval := 200
   else
if key_left[ipid] = 5 then JoyTimer_Left.Interval := 100
   else
if key_left[ipid] = 15 then JoyTimer_Left.Interval := 50
   else
if key_left[ipid] = 30 then JoyTimer_Left.Interval := 20
   else
if key_left[ipid] > 100 then key_left[ipid] := 31;

end;

procedure TfrmMain.JoyTimer_RightTimer(Sender: TObject);
var pid, keychar:string;
    ipid, keycode: integer;
begin

if key_right[0] >= 0 then ipid := 0 else ipid := 1;
pid := IntToStr(ipid);

keychar := cfg.ReadString('Player '+pid, 'Joystick.right', ' ');
keycode := Key2Keycode(keychar);


Gamepad_SendKey(keycode);

key_right[ipid] := key_right[ipid]+1;
if key_right[ipid] = 1 then JoyTimer_Right.Interval := 200
   else
if key_right[ipid] = 5 then JoyTimer_Right.Interval := 100
   else
if key_right[ipid] = 15 then JoyTimer_Right.Interval := 50
   else
if key_right[ipid] = 30 then JoyTimer_Right.Interval := 20
   else
if key_right[ipid] > 100 then key_right[ipid] := 31;
end;

// ----------------------------------------
// --     GAMEPAD - SEND KEY TO BROWSER  --
// ----------------------------------------
procedure TfrmMain.Gamepad_Sendkey(keycode: integer; down:boolean = TRUE; up:boolean = TRUE);
var event: TCefKeyEvent;
begin
if crm.Browser <> nil then
   begin
   if debug then debug_log.Lines.Add('BROWSER: SEND KEY "'+AnsiChar(keycode)+'"'+IntToStr(keycode)+' '+BoolToStr(down)+' '+BoolToStr(up) );

   if down and up then
      begin
      event.kind := KEYEVENT_KEYDOWN;
      event.windows_key_code := keycode;
      event.native_key_code := keycode;
    {  case keycode of
           37:     begin event.windows_key_code := VK_LEFT;  event.native_key_code := VK_LEFT;  end;
           38:     begin event.windows_key_code := VK_UP;    event.native_key_code := VK_UP;    end;
           39:     begin event.windows_key_code := VK_RIGHT; event.native_key_code := VK_RIGHT; end;
           40:     begin event.windows_key_code := VK_DOWN;  event.native_key_code := VK_DOWN;  end;
      end;
    }
      crm.Browser.Host.SendKeyEvent(@event);
      event.kind := KEYEVENT_KEYUP;
      crm.Browser.Host.SendKeyEvent(@event);
      end
   else
   if down and not up then
      begin
      event.kind := KEYEVENT_KEYDOWN;
      event.windows_key_code := keycode;
      event.native_key_code := keycode;
    {  case keycode of
           37:     begin event.windows_key_code := VK_LEFT;  event.native_key_code := VK_LEFT;  end;
           38:     begin event.windows_key_code := VK_UP;    event.native_key_code := VK_UP;    end;
           39:     begin event.windows_key_code := VK_RIGHT; event.native_key_code := VK_RIGHT; end;
           40:     begin event.windows_key_code := VK_DOWN;  event.native_key_code := VK_DOWN;  end;
      end;
    }
      crm.Browser.Host.SendKeyEvent(@event);
      end
    else
    if not down and up then
      begin
      event.kind := KEYEVENT_KEYUP;
      event.windows_key_code := keycode;
      event.native_key_code := keycode;
   {   case keycode of
           37:     begin event.windows_key_code := VK_LEFT;  event.native_key_code := VK_LEFT;  end;
           38:     begin event.windows_key_code := VK_UP;    event.native_key_code := VK_UP;    end;
           39:     begin event.windows_key_code := VK_RIGHT; event.native_key_code := VK_RIGHT; end;
           40:     begin event.windows_key_code := VK_DOWN;  event.native_key_code := VK_DOWN;  end;
      end;
    }
      crm.Browser.Host.SendKeyEvent(@event);
      end;

   end;
end;


// --------------------------------------
// --     GAMEPAD - MOVE LEFT / RIGHT  --
// --------------------------------------
procedure TfrmMain.Gamepad_Move_X(Sender: TObject; value: Integer; Gamepad: TGamepadState);
begin
//Caption := IntToStr(value);
{
if value < 0 then
  begin
  // MOVE LEFT
  key_left := 0;
  key_right := 0;
  btn_gamepad_leftMouseDown(Sender, mbLeft,[],0,0);
  timer_gp_left.Interval := 200;
  timer_gp_left.Enabled := true;
  end
else
if value > 0 then
  begin
  // MOVE RIGHT
  key_left := 0;
  key_right := 0;
  btn_gamepad_rightMouseDown(Sender, mbLeft,[],0,0);
  timer_gp_right.Interval := 200;
  timer_gp_right.Enabled := true;
  end
else
  begin
  timer_gp_left.Enabled := false;
  timer_gp_right.Enabled := false;
  if key_left  > 0 then btn_gamepad_leftMouseUp(Sender, mbLeft,[],0,0);
  if key_right > 0 then btn_gamepad_rightMouseUp(Sender, mbLeft,[],0,0);
  end;
}
end;


// --------------------------------------
// --     GAMEPAD - MOVE UP / DOWN     --
// --------------------------------------
procedure TfrmMain.Gamepad_Move_Y(Sender: TObject; value: Integer; Gamepad: TGamepadState);
begin
{
//Caption := IntToStr(value);
if value < 0 then
  begin
  // MOVE UP
  key_up := 0;
  key_down := 0;
  btn_gamepad_upMouseDown(Sender, mbLeft,[],0,0);
  timer_gp_up.Interval := 200;
  timer_gp_up.Enabled := true;
  end
else
if value > 0 then
  begin
  // MOVE DOWN
  key_up := 0;
  key_down := 0;
  btn_gamepad_downMouseDown(Sender, mbLeft,[],0,0);
  timer_gp_down.Interval := 200;
  timer_gp_down.Enabled := true;
  end
else
  begin
  timer_gp_up.Enabled := false;
  timer_gp_down.Enabled := false;
  if key_down > 0 then btn_gamepad_downMouseUp(Sender, mbLeft,[],0,0);
  if key_up > 0 then btn_gamepad_upMouseUp(Sender, mbLeft,[],0,0);
  end;
}
end;













// ----------------------------------------------------
// --  UNIT INITIALIZATION - SET GLOBAL CEF OPTIONS  --
// ----------------------------------------------------
initialization
{$I cef_initialization.inc}
end.
