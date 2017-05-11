// ==========================================================================
// USB - Insertion and Removal Detection Class
// Mike Heydon 2007
//
// Assignable Events
// eg. procedure TForm1.MyOnInsert(AObject : TObject;
//                                 const ADevType,ADriverName,
//                                       AFriendlyName : string)
//
// OnUsbInsertion : TOnUsbChangeEvent
// OnUsbRemoval : TOnUsbChangeEvent
//
// Example of string returned by API
//
// \\?\USB#Vid_4146&Pid_d2b5#0005050400044#{a5dcbf10-6530-11d2-901f-00c04fb951ed}
//
// Example of output from above string from my Iomega Stick :
//
// USB Inserted
// Device Type   = USB Mass Storage Device
// Driver Name   = Disk drive
// Friendly Name = I0MEGA UMni1GB*IOM2J4 USB Device
//
// Example Code (Skeleton) ....
//
// interface
// uses MahUSB
//
// type
//   TForm1 = class(TForm)
//     procedure FormShow(Sender: TObject);
//     procedure FormClose(Sender: TObject; var Action: TCloseAction);
//   private
//     { Private declarations }
//     FUsb : TUsbClass;
//     procedure UsbIN(ASender : TObject; const ADevType,ADriverName,
//                     AFriendlyName : string);
//     procedure UsbOUT(ASender : TObject; const ADevType,ADriverName,
//                      AFriendlyName : string);
//   public
//   end;
//
// implementation
//
// procedure TForm1.FormShow(Sender: TObject);
// begin
//   FUsb := TUsbClass.Create;
//   FUsb.OnUsbInsertion := UsbIN;
//   FUsb.OnUsbRemoval := UsbOUT;
// end;
//
// procedure TForm1.FormClose(Sender: TObject; var Action: TCloseAction);
// begin
//   FreeAndNil(FUsb);
// end;
//
// procedure TForm1.UsbIN(ASender : TObject; const ADevType,ADriverName,
//                        AFriendlyName : string);
// begin
//   showmessage('USB Inserted - Device Type = ' + ADevType + #13#10 +
//               'Driver Name = ' + ADriverName + #13+#10 +
//               'Friendly Name = ' + AFriendlyName);
// end;
//
//
// procedure TForm1.UsbOUT(ASender : TObject; const ADevType,ADriverName,
//                         AFriendlyName : string);
// begin
//   showmessage('USB Removed - Device Type = ' + ADevType + #13#10 +
//               'Driver Name = ' + ADriverName + #13+#10 +
//               'Friendly Name = ' + AFriendlyName);
// end;
//
// end.
//
// =========================================================================

unit MahUSB;
interface
uses Windows, Messages, SysUtils, Classes, Registry, Masks;

type
  { Event Types }
  TOnUsbChangeEvent = procedure(AObject : TObject;
                                const ADevType,ADriverName,
                                      AFriendlyName : string) of object;

  { USB Class }
  TUsbClass = class(TObject)
  private
    FHandle : HWND;
    FOnUsbRemoval,
    FOnUsbInsertion : TOnUsbChangeEvent;
    procedure GetUsbInfo(const ADeviceString : string;
                         out ADevType,ADriverDesc,
                            AFriendlyName : string);
    procedure WinMethod(var AMessage : TMessage);
    procedure RegisterUsbHandler;
    procedure WMDeviceChange(var AMessage : TMessage);
  public
    constructor Create;
    destructor Destroy; override;
    property OnUsbInsertion : TOnUsbChangeEvent read FOnUsbInsertion
                                           write FOnUsbInsertion;
    property OnUsbRemoval : TOnUsbChangeEvent read FOnUsbRemoval
                                         write FOnUsbRemoval;
  end;



// -----------------------------------------------------------------------------
implementation

type
  // Win API Definitions
  PDevBroadcastDeviceInterface  = ^DEV_BROADCAST_DEVICEINTERFACE;
  DEV_BROADCAST_DEVICEINTERFACE = record
    dbcc_size : DWORD;
    dbcc_devicetype : DWORD;
    dbcc_reserved : DWORD;
    dbcc_classguid : TGUID;
    dbcc_name : char;
  end;

const
  // Miscellaneous
  GUID_DEVINTF_USB_DEVICE : TGUID = '{A5DCBF10-6530-11D2-901F-00C04FB951ED}';
  USB_INTERFACE                = $00000005; // Device interface class
  USB_INSERTION                = $8000;     // System detected a new device
  USB_REMOVAL                  = $8004;     // Device is gone

  // Registry Keys
  USBKEY  = 'SYSTEM\CurrentControlSet\Enum\USB\%s\%s';
  USBSTORKEY = 'SYSTEM\CurrentControlSet\Enum\USBSTOR';
  SUBKEY1  = USBSTORKEY + '\%s';
  SUBKEY2  = SUBKEY1 + '\%s';


constructor TUsbClass.Create;
begin
  inherited Create;
  FHandle := AllocateHWnd(WinMethod);
  RegisterUsbHandler;
end;


destructor TUsbClass.Destroy;
begin
  DeallocateHWnd(FHandle);
  inherited Destroy;
end;


procedure TUsbClass.GetUsbInfo(const ADeviceString : string;
                               out ADevType,ADriverDesc,
                                   AFriendlyName : string);
var sWork,sKey1,sKey2 : string;
    oKeys,oSubKeys : TStringList;
    oReg : TRegistry;
    i,ii : integer;
    bFound : boolean;
begin
  ADevType := '';
  ADriverDesc := '';
  AFriendlyName := '';

  if ADeviceString <> '' then begin
    bFound := false;
    oReg := TRegistry.Create;
    oReg.RootKey := HKEY_LOCAL_MACHINE;

    // Extract the portions of the string we need for registry. eg.
    // \\?\USB#Vid_4146&Pid_d2b5#0005050400044#{a5dcbf10- ..... -54334fb951ed}
    // We need sKey1='Vid_4146&Pid_d2b5' and sKey2='0005050400044'
    sWork := copy(ADeviceString,pos('#',ADeviceString) + 1,1026);
    sKey1 := copy(sWork,1,pos('#',sWork) - 1);
    sWork := copy(sWork,pos('#',sWork) + 1,1026);
    sKey2 := copy(sWork,1,pos('#',sWork) - 1);

    // Get the Device type description from \USB key
    if oReg.OpenKeyReadOnly(Format(USBKEY,[skey1,sKey2])) then begin
      ADevType := oReg.ReadString('DeviceDesc');
      oReg.CloseKey;
      oKeys := TStringList.Create;
      oSubKeys := TStringList.Create;

      // Get list of keys in \USBSTOR and enumerate each key
      // for a key that matches our sKey2='0005050400044'
      // NOTE : The entry we are looking for normally has '&0'
      // appended to it eg. '0005050400044&0'
      if oReg.OpenKeyReadOnly(USBSTORKEY) then begin
        oReg.GetKeyNames(oKeys);
        oReg.CloseKey;

        // Iterate through list to find our sKey2
        for i := 0 to oKeys.Count - 1 do begin
          if oReg.OpenKeyReadOnly(Format(SUBKEY1,[oKeys[i]])) then begin
            oReg.GetKeyNames(oSubKeys);
            oReg.CloseKey;

            for ii := 0 to oSubKeys.Count - 1 do begin
              if MatchesMask(oSubKeys[ii],sKey2 + '*') then begin
                // Got a match?, get the actual desc and friendly name
                if oReg.OpenKeyReadOnly(Format(SUBKEY2,[oKeys[i],
                                        oSubKeys[ii]])) then begin
                  ADriverDesc := oReg.ReadString('DeviceDesc');
                  AFriendlyName := oReg.ReadString('FriendlyName');
                  oReg.CloseKey;
                end;

                bFound := true;
              end;
            end;
          end;

          if bFound then break;
        end;
      end;

      FreeAndNil(oKeys);
      FreeAndNil(oSubKeys);
    end;

    FreeAndNil(oReg);
  end;
end;


procedure TUsbClass.WMDeviceChange(var AMessage : TMessage);
var iDevType : integer;
    sDevString,sDevType,
    sDriverName,sFriendlyName : string;
    pData : PDevBroadcastDeviceInterface;
begin
  if (AMessage.wParam = USB_INSERTION) or
     (AMessage.wParam = USB_REMOVAL) then begin
    pData := PDevBroadcastDeviceInterface(AMessage.LParam);
    iDevType := pData^.dbcc_devicetype;

    // Is it a USB Interface Device ?
    if iDevType = USB_INTERFACE then begin
      sDevString := PChar(@pData^.dbcc_name);
      GetUsbInfo(sDevString,sDevType,sDriverName,sFriendlyName);

      // Trigger Events if assigned
      if (AMessage.wParam = USB_INSERTION) and
        Assigned(FOnUsbInsertion) then
          FOnUsbInsertion(self,sDevType,sDriverName,sFriendlyName);
      if (AMessage.wParam = USB_REMOVAL) and
        Assigned(FOnUsbRemoval) then
          FOnUsbRemoval(self,sDevType,sDriverName,sFriendlyName);
    end;
  end;
end;



procedure TUsbClass.WinMethod(var AMessage : TMessage);
begin
if (AMessage.Msg = WM_DEVICECHANGE) then
	WMDeviceChange(AMessage)
else
	AMessage.Result := DefWindowProc(FHandle,AMessage.Msg,
									AMessage.wParam,AMessage.lParam);
end;


procedure TUsbClass.RegisterUsbHandler;
var rDbi : DEV_BROADCAST_DEVICEINTERFACE;
    iSize : integer;
begin
  iSize := SizeOf(DEV_BROADCAST_DEVICEINTERFACE);
  ZeroMemory(@rDbi,iSize);
  rDbi.dbcc_size := iSize;
  rDbi.dbcc_devicetype := USB_INTERFACE;
  rDbi.dbcc_reserved := 0;
  rDbi.dbcc_classguid  := GUID_DEVINTF_USB_DEVICE;
  rDbi.dbcc_name := #0;
  RegisterDeviceNotification(FHandle,@rDbi,DEVICE_NOTIFY_WINDOW_HANDLE);
end;


end.