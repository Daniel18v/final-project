unit uMain;

interface

uses
  Winapi.Windows, Winapi.Messages, System.SysUtils, System.Variants, System.Classes, Vcl.Graphics,
  Vcl.Controls, Vcl.Forms, Vcl.Dialogs, Vcl.ExtCtrls, AdvSmoothLabel, mahUSB, iniFiles,
  gamepad, ShellApi;

type
  TForm1 = class(TForm)
    lblCountDown: TAdvSmoothLabel;
    lblHeader: TAdvSmoothLabel;
    lblFooter: TAdvSmoothLabel;
    Timer1: TTimer;
    Gamepad1: TGamepad;
    GamepadTimer: TTimer;
    procedure FormCreate(Sender: TObject);
    procedure FormShow(Sender: TObject);
    procedure Timer1Timer(Sender: TObject);
    procedure GamepadTimerTimer(Sender: TObject);
    procedure Gamepad_Joystick_X(Sender: TObject; value: Integer; Gamepad: TGamepadState);
    procedure Gamepad_Joystick_Y(Sender: TObject; value: Integer; Gamepad: TGamepadState);
    procedure Gamepad_Joystick_Z(Sender: TObject; value: Integer; Gamepad: TGamepadState);
    procedure Gamepad_Button_Down(Sender: TObject; button: Integer; Gamepad: TGamepadState);
    procedure Gamepad_Button_Press(Sender: TObject; button: Integer; Gamepad: TGamepadState);
    procedure Gamepad_Button_Up(Sender: TObject; button: Integer; Gamepad: TGamepadState);
    procedure FormClose(Sender: TObject; var Action: TCloseAction);
  private
    { Private declarations }
     FUsb : TUsbClass;
     function DoExitWindows(RebootParam: Longword): Boolean;

     procedure UsbIN(ASender : TObject; const ADevType,ADriverName,
                     AFriendlyName : string);
     procedure UsbOUT(ASender : TObject; const ADevType,ADriverName,
                      AFriendlyName : string);
  public
    { Public declarations }
    ini:TIniFile;
    gamepads: array[0..1] of TGamepad;

    do_action:string;
    action_app:string;    // action app
    action_par:string;    // action app params
    countdown:integer;

    cancel_app :string;   // cancel countdown app
    cancel_par :string;   // cancel countdown params

    procedure StartAction();
    procedure InitGamepads(max:integer);
  end;

var
  Form1: TForm1;

implementation

{$R *.dfm}

procedure TForm1.InitGamepads(max:integer);
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


//   Label1.Caption := 'Gamepad 1 ready';
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

//   Label2.Caption := 'Gamepad 2 ready';
   end;

if (max > 0) then
   GamepadTimer.Enabled := true;
end;

procedure TForm1.StartAction();
begin
  if do_action = 'cancel' then
     begin
     if cancel_app <> '' then
        begin
        ShellExecute(0,'open',PChar(cancel_app), PChar(cancel_par),'',SW_SHOWNORMAL);
        end;
     end
  else
  if do_action = 'shutdown' then
     begin
     // shutdown pc here
     DoExitWindows(EWX_POWEROFF or EWX_FORCE);
     end
  else
  if do_action = 'restart' then
     begin
     // restart pc here
     DoExitWindows(EWX_REBOOT or EWX_FORCE);
     end
  else
  if do_action = 'custom' then
     begin
     // custom action here
     if action_app <> '' then
        begin
        ShellExecute(0,'open',PChar(action_app), PChar(action_par),'',SW_SHOWNORMAL);
        end;
     end;

Close; // quit action
end;

function TForm1.DoExitWindows(RebootParam: Longword): Boolean;
var
  TTokenHd: THandle;
  TTokenPvg: TTokenPrivileges;
  cbtpPrevious: DWORD;
  rTTokenPvg: TTokenPrivileges;
  pcbtpPreviousRequired: DWORD;
  tpResult: Boolean;
const
  SE_SHUTDOWN_NAME = 'SeShutdownPrivilege';
begin
  if Win32Platform = VER_PLATFORM_WIN32_NT then
  begin
    tpResult := OpenProcessToken(GetCurrentProcess(),
      TOKEN_ADJUST_PRIVILEGES or TOKEN_QUERY,
      TTokenHd);
    if tpResult then
    begin
      tpResult := LookupPrivilegeValue(nil,
                                       SE_SHUTDOWN_NAME,
                                       TTokenPvg.Privileges[0].Luid);
      TTokenPvg.PrivilegeCount := 1;
      TTokenPvg.Privileges[0].Attributes := SE_PRIVILEGE_ENABLED;
      cbtpPrevious := SizeOf(rTTokenPvg);
      pcbtpPreviousRequired := 0;
      if tpResult then
        WinApi.Windows.AdjustTokenPrivileges(TTokenHd,
                                      False,
                                      TTokenPvg,
                                      cbtpPrevious,
                                      rTTokenPvg,
                                      pcbtpPreviousRequired);
    end;
  end;
  Result := ExitWindowsEx(RebootParam, 0);
end;

procedure TForm1.FormClose(Sender: TObject; var Action: TCloseAction);
begin
ini.Free;
FreeAndNil(FUsb);
end;

procedure TForm1.FormCreate(Sender: TObject);
var s:string;
begin
ini := TIniFile.Create(ChangeFileExt(Application.ExeName,'.ini'));

s := ini.ReadString('MAIN', 'border.enabled', '0');
if s = '1' then Form1.BorderStyle := bsSizeable
           else Form1.BorderStyle := bsNone;

s:= ParamStr(1);
if s = '-shutdown' then
   begin
     lblHeader.Caption.Text := ini.ReadString('MAIN', 'countdown.shutdown.text', 'KONKADE IS POWERING OFF!');
     countdown := ini.ReadInteger('MAIN', 'countdown.shutdown.time', 10);
     lblCountDown.Caption.Text := IntToStr(countdown);
     do_action := 'shutdown';
     cancel_app := ini.ReadString('MAIN', 'countdown.cancel.app', '');
     cancel_par := ini.ReadString('MAIN', 'countdown.cancel.param', '');
   end
else
if s = '-restart' then
   begin
     lblHeader.Caption.Text := ini.ReadString('MAIN', 'countdown.restart.text', 'KONKADE IS RESTARTING!');
     countdown :=  ini.ReadInteger('MAIN', 'countdown.restart.time', 10);
     lblCountDown.Caption.Text := IntToStr(countdown);
     do_action := 'restart';
     cancel_app := ini.ReadString('MAIN', 'countdown.cancel.app', '');
     cancel_par := ini.ReadString('MAIN', 'countdown.cancel.param', '');
   end
else
if ini.SectionExists('ACTION'+s) then
   begin
     lblHeader.Caption.Text := ini.ReadString('ACTION'+s, 'action.text', 'ACTION TEXT NOT SET!');
     countdown := ini.ReadInteger('ACTION'+s, 'action.time', 10);
     lblCountDown.Caption.Text := IntToStr(countdown);

     do_action := 'custom';
     action_app := ini.ReadString('ACTION'+s, 'action.app', '');
     action_par := ini.ReadString('ACTION'+s, 'action.param', '');
     cancel_app := ini.ReadString('MAIN', 'action.cancel.app', '');
     cancel_par := ini.ReadString('MAIN', 'action.cancel.param', '');
   end
else
    begin
    lblHeader.Caption.Text := 'KONKADE IS NOT DOYING ANYTHING NOW!';
    lblCountDown.Caption.Text := '^__^';
    countdown := 10;
    do_action := 'quit';
    action_app := '';
    action_par := '';
    cancel_app := '';
    cancel_par := '';
    end;
end;

procedure TForm1.FormShow(Sender: TObject);
begin
   FUsb := TUsbClass.Create;
   FUsb.OnUsbInsertion := UsbIN;
   FUsb.OnUsbRemoval := UsbOUT;

   Gamepad1.Update;
   Gamepad1.MaxDevices;

   Caption := 'Max gamepads: '+IntToStr(Gamepad1.MaxDevices);
   InitGamepads(Gamepad1.MaxDevices);
Timer1.Enabled := true;
end;

procedure TForm1.GamepadTimerTimer(Sender: TObject);
begin
if Assigned(gamepads[0]) then begin gamepads[0].Update; end;
if Assigned(gamepads[1]) then begin gamepads[1].Update; end;
end;

procedure TForm1.Timer1Timer(Sender: TObject);
begin
if (countdown > 1) then
   begin
   countdown := countdown - 1;
   lblCountDown.Caption.Text := IntToStr(countdown);
   end
else
if (countdown = 1) then
   begin
   countdown := countdown - 1;
   lblCountDown.Caption.Text := 'GO!';
   end
else
if (countdown = 0) then
   begin
   Timer1.Enabled := false;
   StartAction();
   end;
end;

procedure TForm1.Gamepad_Joystick_X(Sender: TObject; value: Integer;
  Gamepad: TGamepadState);
var pid,dir:string;
begin
pid := IntToStr((Sender as TGamepad).Tag);
{
if (value = 0) then dir := 'Release'
               else
if (value > 0) then dir := 'right'
               else dir := 'left';


if (value <> 0) then
   Memo1.Lines.Add('Player '+pid+': '+#13#10+
                        #9+ 'Joystick move: '+ini.ReadString('Player '+pid, 'Joystick.'+dir, 'UNASSIGNED'))
else
   Memo1.Lines.Add('Player '+pid+': '+#13#10+
                        #9+ 'Joystick move: '+dir);
}
end;

procedure TForm1.Gamepad_Joystick_Y(Sender: TObject; value: Integer;
  Gamepad: TGamepadState);
var pid,dir:string;
begin
pid := IntToStr((Sender as TGamepad).Tag);
{
if (value = 0) then dir := 'Release'
               else
if (value > 0) then dir := 'down'
               else dir := 'up';

if (value <> 0) then
   Memo1.Lines.Add('Player '+pid+': '+#13#10+
                        #9+ 'Joystick move: '+ini.ReadString('Player '+pid, 'Joystick.'+dir, 'UNASSIGNED'))
else
   Memo1.Lines.Add('Player '+pid+': '+#13#10+
                        #9+ 'Joystick move: '+dir);
}
end;

procedure TForm1.Gamepad_Joystick_Z(Sender: TObject; value: Integer;
  Gamepad: TGamepadState);
var pid,dir:string;
begin
pid := IntToStr((Sender as TGamepad).Tag);
{
if (value = 0) then dir := 'Release'
               else
if (value > 0) then dir := 'z-down'
               else dir := 'z-up';

if (value <> 0) then
   Memo1.Lines.Add('Player '+pid+': '+#13#10+
                        #9+ 'Joystick move: '+ini.ReadString('Player '+pid, 'Joystick.'+dir, 'UNASSIGNED'))
else
   Memo1.Lines.Add('Player '+pid+': '+#13#10+
                        #9+ 'Joystick move: '+dir);
}
end;

procedure TForm1.Gamepad_Button_Down(Sender: TObject; button: Integer;
  Gamepad: TGamepadState);
var pid,dir:string;
begin
pid := IntToStr((Sender as TGamepad).Tag);
{
Memo1.Lines.Add('Player '+pid+': '+#13#10+
                        #9+ 'Joystick button '+IntToStr(button)+' down: '+ini.ReadString('Player '+pid, 'Joystick.button.'+IntToStr(button), 'BTN '+IntToStr(button)+' UNASSIGNED'));
}
end;

procedure TForm1.Gamepad_Button_Press(Sender: TObject; button: Integer;
  Gamepad: TGamepadState);
var pid,dir:string;
begin
pid := IntToStr((Sender as TGamepad).Tag);

dir := ini.ReadString('Player '+pid, 'Joystick.button.'+IntToStr(button), '0');

if (dir = '0') AND (countdown > 1) then
   begin
   countdown := countdown - 1;
   lblCountDown.Caption.Text := IntToStr(countdown);
   end
else
if (dir = '1') then
   begin
   Timer1.Enabled := false;
   lblCountDown.Caption.Text := 'Abort!';
   do_action := 'cancel';
   StartAction;
   end;
end;

procedure TForm1.Gamepad_Button_Up(Sender: TObject; button: Integer;
  Gamepad: TGamepadState);
var pid,dir:string;
begin
pid := IntToStr((Sender as TGamepad).Tag);
{
Memo1.Lines.Add('Player '+pid+': '+#13#10+
                        #9+ 'Joystick button '+IntToStr(button)+' up: '+ini.ReadString('Player '+pid, 'Joystick.button.'+IntToStr(button), 'BTN '+IntToStr(button)+' UNASSIGNED'));

}end;

procedure TForm1.UsbIN(ASender : TObject; const ADevType,ADriverName,
                       AFriendlyName : string);
begin
{  Memo1.Lines.Add('USB Inserted - Device Type = ' + ADevType + #13#10 +
              #9+'Driver Name = ' + ADriverName + #13+#10 +
              #9+'Friendly Name = ' + AFriendlyName);
}
   Gamepad1.Update;
   Gamepad1.MaxDevices;
//   Memo1.Lines.Add(#13#10+'Max gamepads: '+IntToStr(Gamepad1.MaxDevices));

   InitGamepads(Gamepad1.MaxDevices);
end;


procedure TForm1.UsbOUT(ASender : TObject; const ADevType,ADriverName,
                         AFriendlyName : string);
begin
{  Memo1.Lines.Add('USB Removed - Device Type = ' + ADevType + #13#10 +
              #9+'Driver Name = ' + ADriverName + #13+#10 +
              #9+'Friendly Name = ' + AFriendlyName);
}
   Gamepad1.Update;
   Gamepad1.MaxDevices;
//   Memo1.Lines.Add(#13#10+'Max gamepads: '+IntToStr(Gamepad1.MaxDevices));

   InitGamepads(Gamepad1.MaxDevices);
end;

end.
