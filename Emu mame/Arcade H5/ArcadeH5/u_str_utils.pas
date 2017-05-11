unit u_str_utils;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, idURI, ShLwApi;


function ExpandFileNameRelBaseDir(const FileName, BaseDir: string): string;

function file_get_contents(const FileName: TFileName): String;
function path_to_uri(const path:string): string;
function Key2Keycode(keycode:string):integer;
function OtherPid(in_pid:integer):integer;

implementation

//uses uMain;

// make sure path is absolute
function ExpandFileNameRelBaseDir(const FileName, BaseDir: string): string;
var
  Buffer: array [0..MAX_PATH-1] of Char;
begin
  if PathIsRelative(PChar(FileName)) then begin
    Result := IncludeTrailingBackslash(BaseDir)+FileName;
  end else begin
    Result := FileName;
  end;
  if PathCanonicalize(@Buffer[0], PChar(Result)) then begin
    Result := Buffer;
  end;
end;


function file_get_contents(const FileName: TFileName): String;
var LStrings: TStringList;
begin
LStrings := TStringList.Create;
try
   LStrings.Loadfromfile(FileName);
   Result := LStrings.text;
finally
   FreeAndNil(LStrings);
end;
end;


function path_to_uri(const path:string): string;
var s:string;
    drive, drive2:string;
    FUrl: TidURI;
begin
drive := ExtractFileDrive(path);
drive2:= stringReplace(drive,':','',[rfReplaceAll]);

s     := stringReplace(path, drive, drive2,[rfIgnoreCase]);
s     := stringReplace(s, '\', '/', [rfReplaceAll]);

FUrl := TidUri.Create('');
s:= FUrl.PathEncode(s);

Result:= 'dcef://'+s;
end;


// this function will convert the char sent to coresponding
// key code. also had a few special cases for navigation
function Key2Keycode(keycode:string):integer;
begin
keycode := UpperCase(keycode);
if keycode = 'VK_LEFT'  then Result := VK_LEFT
else
if keycode = 'VK_RIGHT' then Result := VK_RIGHT
else
if keycode = 'VK_UP'    then Result := VK_UP
else
if keycode = 'VK_DOWN'  then Result := VK_DOWN
else
    begin
    //frmMain.debug_log.Lines.Add('DEBUG: Key2Code: Input: "'+keycode+'"...');
    Result := Ord(keycode[1]);
    //Result := 10;
    end;
end;


function OtherPid(in_pid:integer):integer;
begin
if in_pid = 0 then
   Result := 1
else
   Result := 0;
end;

end.
