unit gamepad;
(*
Version 1.1 [2014-06-03] - Added MaxDevices property to get connected devices number
                         - Added destructor
                         - Events + Timer
                         - Changed from simple class to class(TComponent)
                         - added Register procedure
                         - changed events from public to published
                         - Added checks to see if device is connected
                         - Moved DeviceNr to published section
                         - Added private FSetNeutralState to reset LastState used in events
*)

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, ExtCtrls,
  MMSystem;

type
   TPOVControl = record
   up,down,left,right:boolean;
  end;
type
   TGamepadState = record
     X, Y, Z:integer;
     R, U, V:integer;
     POV:TPOVControl;
     buttons:array[0..31] of boolean;
   end;
type
  TGamepadMoveEvent = procedure (Sender : TObject; value:integer; Gamepad:TGamepadState) of object;
  TGamepadButtonEvent = procedure (Sender : TObject; button:integer; Gamepad:TGamepadState) of object;    // for button press
  TGamepadPOVEvent = procedure (Sender : TObject; POV:TPOVControl; Gamepad:TGamepadState) of object;

  { TGamepad - A wrapper class for the Windows-Joystick-API}
  TGamepad = class(TComponent)
    private
      FTimer: TTimer;
      FRange:integer;
      FDeadZone:integer;
      FPluggedIn:boolean;                 // v1.1 - indicated if joystick is connected
      FLastState:TGamepadState;           // v1.1
 
      FOnXChange,
      FOnYChange,
      FOnZChange,
      FOnRChange,
      FOnUChange,
      FOnVChange: TGamepadMoveEvent;    // v1.1

      FOnButtonPress,
      FOnButtonDown,
      FOnButtonUp: TGamepadButtonEvent;      // v1.1

      FOnPOVChange:TGamepadPOVEvent;         // v1.1

      procedure FSetNeutralState;            // set FLastState to neutral state
      procedure FTimerUpdate(Sender: TObject);    // v1.1
      function GetButton(index:integer):boolean;
      function GetX:integer;
      function GetY:integer;
      function GetZ:integer;
      function GetR:integer;
      function GetU:integer;
      function GetV:integer;
      function GetPOV:TPOVControl;
      function GetDeviceNrMax:integer;        // v1.1
      procedure UpdateDeviceNr(nr:cardinal);
    protected
      Device:TJoyInfoEx;
      DeviceInfo:TJoyCaps;
      FDeviceNr:Cardinal;
      CenterX,CenterY,CenterZ:Integer;
      CenterR,CenterU,CenterV:Integer;
    public
      procedure Update;
      procedure Calibrate;
      constructor Create;
      destructor Destroy;
      property X:integer read GetX;
      property Y:integer read GetY;
      property Z:integer read GetZ;
      property R:integer read GetR;  // rudder or fourth joystick axis
      property U:integer read GetU;  // fifth axis
      property V:integer read GetV;  // sixth axis
      property Range:integer read FRange write FRange;
      property DeadZone:integer read FDeadZone write FDeadZone;
      property POV:TPOVControl read GetPov;
      property Buttons[index:integer]:boolean read GetButton;
    published
      property MaxDevices:integer read GetDeviceNrMax;
      property DeviceNr:Cardinal read FDeviceNr write UpdateDeviceNr;
      property PluggedIn:boolean read FPluggedIn;  // read only
      // events for direction
      property OnX: TGamepadMoveEvent read FOnXChange write FOnXChange;
      property OnY: TGamepadMoveEvent read FOnYChange write FOnYChange;
      property OnZ: TGamepadMoveEvent read FOnZChange write FOnZChange;
      property OnR: TGamepadMoveEvent read FOnRChange write FOnRChange;
      property OnU: TGamepadMoveEvent read FOnUChange write FOnUChange;
      property OnV: TGamepadMoveEvent read FOnVChange write FOnVChange;
      property OnPOV: TGamepadPOVEvent read FOnPOVChange write FOnPOVChange;
      // events for buttons
      property OnButtonPress: TGamepadButtonEvent read FOnButtonPress write FOnButtonPress;
      property OnButtonDown: TGamepadButtonEvent read FOnButtonDown write FOnButtonDown;
      property OnButtonUp: TGamepadButtonEvent read FOnButtonUp write FOnButtonUp;
  end;

procedure Register;

implementation

{ TGamepad }

// begin update 1.1 timer functions
procedure TGamepad.FTimerUpdate(Sender: TObject);
var newState : TGamepadState;
    i:integer;

    function dirAbsValue(value:integer):integer;
             begin
             if value = 0 then Result := 0
                          else
             if value < 0 then Result := -1
                          else Result := 1;


             end;
begin
Update;
if not FPluggedIn then Exit; // no processing if gamepad not connected

FTimer.Enabled := false;     // disable timer while running update

  // direction
  newState.X := GetX; newState.Y := GetY; newState.Z := GetZ;
  newState.R := GetR; newState.U := GetU; newState.V := GetV;
  // pov
  newState.POV := GetPov;
  // button states
  for i := 0 to 31 do newState.buttons[i] := device.wbuttons and (1 shl (i)) > 0;

  // directional events x y z
  if Assigned(OnX) and (dirAbsValue(FLastState.X)  <> dirAbsValue(newState.X) ) then OnX(Self, newState.X, newState);
  if Assigned(OnY) and (dirAbsValue(FLastState.Y)  <> dirAbsValue(newState.Y) ) then OnY(Self, newState.Y, newState);
  if Assigned(OnZ) and (dirAbsValue(FLastState.Z)  <> dirAbsValue(newState.Z) ) then OnZ(Self, newState.Z, newState);
  // directional events r u v
  if Assigned(OnR) and (dirAbsValue(FLastState.R)  <> dirAbsValue(newState.R) ) then OnR(Self, newState.R, newState);
  if Assigned(OnU) and (dirAbsValue(FLastState.U)  <> dirAbsValue(newState.U) ) then OnU(Self, newState.U, newState);
  if Assigned(OnV) and (dirAbsValue(FLastState.V)  <> dirAbsValue(newState.V) ) then OnV(Self, newState.V, newState);
  // directional events POV
  if Assigned(OnPOV) and (
        (newState.POV.up <> FLastState.POV.up) or
        (newState.POV.down <> FLastState.POV.down) or
        (newState.POV.left <> FLastState.POV.left) or
        (newState.POV.right <> FLastState.POV.right) ) then OnPOV(Self, newState.POV, newState);
  // button events
  if Assigned(OnButtonPress) then
     begin
     for i := 0 to 31 do
         begin
         // trigger event for each button pressed
         if not newState.buttons[i] and FLastState.buttons[i] then OnButtonPress(Self, i, newState);
         end;
     end;
  if Assigned(OnButtonDown) then
     begin
     for i := 0 to 31 do
         begin
         // trigger event for each button pressed
         if newState.buttons[i] and not FLastState.buttons[i] then OnButtonDown(Self, i, newState);
         end;
     end;
  if Assigned(OnButtonUp) then
     begin
     for i := 0 to 31 do
         begin
         // trigger event for each button pressed
         if not newState.buttons[i] and FLastState.buttons[i] then OnButtonUp(Self, i, newState);
         end;
     end;

FLastState := newState;
FTimer.Enabled := true;    // restore timer after running update
end;
// end update 1.1 timer functions

function TGamepad.GetX:integer;
begin
  result := round(range/32767*(Device.wXpos-CenterX));
  if abs(result) <= deadzone then result := 0;
end;

function TGamepad.GetY:integer;
begin
  result := round(range/32767*(Device.wYpos-CenterY));
  if abs(result) <= deadzone then result := 0;
end;

function TGamepad.GetZ:integer;
begin
  result := round(range/32767*(Device.wZpos-CenterZ));
  if abs(result) <= deadzone then result := 0;
end;

function TGamepad.GetR:integer;
begin
  result := round(range/32767*(Device.dwRpos-CenterR));
  if abs(result) <= deadzone then result := 0;
end;

function TGamepad.GetU:integer;
begin
  result := round(range/32767*(Device.dwUpos-CenterU));
  if abs(result) <= deadzone then result := 0;
end;

function TGamepad.GetV:integer;
begin
  result := round(range/32767*(Device.dwVpos-CenterV));
  if abs(result) <= deadzone then result := 0;
end;

function TGamepad.GetPOV:TPOVControl;
begin
  //Verarbeitet die Daten des Steuerkreuzes
  result.up := false;
  result.left := false;
  result.down := false;
  result.right := false;
  if FPluggedIn then
     begin
     if Device.dwPOV = 0 then begin result.up := true; end;
     if Device.dwPOV = 4500 then begin result.up := true; result.right := true end;
     if Device.dwPOV = 9000 then begin result.right := true; end;
     if Device.dwPOV = 13500 then begin result.down := true; result.right := true end;
     if Device.dwPOV = 18000 then begin result.down := true; end;
     if Device.dwPOV = 22500 then begin result.down := true; result.left := true; end;
     if Device.dwPOV = 27000 then begin result.left := true; end;
     if Device.dwPOV = 31500 then begin result.left := true; result.up := true; end;
     end;
end;

procedure TGamepad.Update;
begin
  //Liest die Joystick-Daten ein und schreibt sie in die "Device" Variable
  if (DeviceInfo.wCaps and JOYCAPS_HASZ) <> 0 then Device.dwSize := sizeof(tjoyInfoEx);
  Device.dwFlags:=JOY_RETURNALL;
  if JoygetposEx(DeviceNr,@device) = JOYERR_NOERROR then FPluggedIn := true
                                                    else FPluggedIn := false;
end;

// get connected device count (update 1.1)
function TGamepad.GetDeviceNrMax:integer;
var i, found:integer;
    joyinfo:TJoyInfo;
begin
found := 0;
for i := 0 to joyGetNumDevs-1 do
    begin
    if JoyGetPos(i, @joyinfo) = JOYERR_NOERROR then found:= found+1;
    end;

Result := found;
end;

procedure TGamepad.UpdateDeviceNr(nr:cardinal);
begin
  //0...15 Devices/Gampads/Joysticks
  FDeviceNr := nr;
  joyGetDevCaps(FDeviceNr, @DeviceInfo, sizeof(DeviceInfo));
  FSetNeutralState;
end;

procedure TGamepad.FSetNeutralState;
var i:integer;
begin
  // set last state to neutral
  FLastState.X := 0; FLastState.Y := 0; FLastState.Z := 0;
  FLastState.R := 0; FLastState.U := 0; FLastState.V := 0;
  FLastState.POV.up := false; FLastState.POV.down := false;
  FLastState.POV.left := false; FLastState.POV.right := false;
  for i := 0 to 31 do FLastState.buttons[i] := false;
end;

constructor TGamepad.Create;
begin
  inherited Create(nil);
  DeviceNr := 0;
  Range := 1000;
  DeadZone := 400;
  FPluggedIn := false;
  Calibrate;

  FSetNeutralState;
  // initialize timer for event handling
  FTimer := TTimer.Create(nil);
  FTimer.Interval := 50;
  FTimer.Enabled := true;
  FTimer.OnTimer:=FTimerUpdate;
end;

destructor TGamepad.Destroy;
begin
  FTimer.Free;
  inherited Destroy;
end;

procedure TGamepad.Calibrate;
begin
  //Liest die Nullstellung der Achsen ein
  if (DeviceInfo.wCaps and JOYCAPS_HASZ) <> 0 then Device.dwSize := sizeof(tjoyInfoEx);
  Device.dwFlags:=JOY_RETURNCENTERED;
  if JoygetposEx(DeviceNr,@device) = JOYERR_NOERROR then FPluggedIn := true
                                                    else FPluggedIn := false;
  CenterX := device.wXpos;
  CenterY := device.wYpos;
  CenterZ := device.wZpos;
  CenterU := device.dwUpos;
  CenterV := device.dwVpos;
  CenterR := device.dwRpos;
end;

function TGamepad.GetButton(index:integer):boolean;
begin
  //Liest die Position der Buttons ein.
  result := false;
  if index in [0..31] then
  begin
    result := device.wbuttons and (1 shl (index)) > 0;
  end;
end;



// register component on component pallet
procedure Register;
begin
  RegisterComponents('Gaming', [TGamepad]);
end;

end.
