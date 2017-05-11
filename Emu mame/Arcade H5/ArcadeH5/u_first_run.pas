unit u_first_run;

interface

uses
  Windows, Messages, SysUtils, Dialogs, Variants, Classes, Forms, IniFiles;


function CheckConfigFiles():boolean; // return true if files ok

function SetupConfigFiles():boolean;


implementation

uses uMain;

var libpath:string;
    slash_basedir:string;
    cfga,cfgm,cfgp:string;

function CheckConfigFiles():boolean;
var  // config file for apache (a) / mysql (m) / php (p)
    myini:TIniFile;
begin
//libpath := ExtractFilePath(Application.ExeName)+BACKENDDIR;//'library\';
libpath := frmMain.backend_dir; //ExtractFilePath(Application.ExeName)+BACKENDDIR;//'library\';
slash_basedir := StringReplace(ExtractFilePath(Application.ExeName),'\','/',[rfReplaceAll]);

//cfga := libpath+'conf\httpd.conf';
//cfgm := libpath+'my.ini';
//cfgp := libpath+'bin\php.ini';
cfga := frmMain.http_bin+'conf\httpd.conf';
cfgm := frmMain.mysql_bin+'my.ini';
cfgp := frmMain.http_bin+'php\php.ini';

if FileExists(cfga) AND FileExists(cfgm) AND FileExists(cfgp) then
   begin
   // check mysql config file to check if path is correct
   myini := TIniFile.Create(cfgm);
//   if LowerCase(myini.ReadString('mysqld','basedir','notset')) <> LowerCase(slash_basedir+'library') then
   if LowerCase(myini.ReadString('mysqld','basedir','notset')) <> LowerCase(StringReplace(ExcludeTrailingPathDelimiter(frmMain.mysql_bin),'\','/',[rfReplaceAll])) then
      begin
      // basedir path for mysql is not correct
      Result := false;
      end
   else
      begin
      Result := true;
      end;
   myini.Free;
   end
else
    begin
    // one or more config files not found
    Result := false;
    end;
end;


function SetupConfigFiles():boolean;
var source,str:string;
    txt:TStringStream;
    S:TStringList;
begin
source := libpath +'setup\ro.mythweb.arcadeh5.httpd.base';
if FileExists(source) then
   begin
   txt := TStringStream.Create;
   txt.LoadFromFile(source);
   S := TStringList.Create;
   str := StringReplace(ExcludeTrailingPathDelimiter(frmMain.http_bin),'\','/',[rfReplaceAll]);
//   S.Text := StringReplace(txt.DataString, '{SLASH_BASEDIR}', slash_basedir, [rfReplaceAll,rfIgnoreCase]);
   S.Text := StringReplace(txt.DataString, '{SLASH_BASEDIR}', str, [rfReplaceAll,rfIgnoreCase]);
   S.SaveToFile(cfga);
   txt.Free;
   S.Free;
   end
else
   Result := false;
source := libpath +'setup\ro.mythweb.arcadeh5.sqld.base';
if FileExists(source) then
   begin
   txt := TStringStream.Create;
   txt.LoadFromFile(source);
   S := TStringList.Create;
   str := StringReplace(ExcludeTrailingPathDelimiter(frmMain.mysql_bin),'\','/',[rfReplaceAll]);
  // S.Text := StringReplace(txt.DataString, '{SLASH_BASEDIR}', slash_basedir, [rfReplaceAll,rfIgnoreCase]);
   S.Text := StringReplace(txt.DataString, '{SLASH_BASEDIR}', str, [rfReplaceAll,rfIgnoreCase]);
   S.SaveToFile(cfgm);
   txt.Free;
   S.Free;
   end
else
   Result := false;
source := libpath +'setup\ro.mythweb.arcadeh5.php.base';
if FileExists(source) then
   begin
   txt := TStringStream.Create;
   txt.LoadFromFile(source);
   S := TStringList.Create;
   str := StringReplace(ExcludeTrailingPathDelimiter(frmMain.http_bin),'\','/',[rfReplaceAll]);
   S.Text := StringReplace(txt.DataString, '{SLASH_BASEDIR}', str, [rfReplaceAll,rfIgnoreCase]);
   S.SaveToFile(cfgp);
   txt.Free;
   S.Free;

   // send success
   Result := true;
   end
else
   Result := false;

end;

end.
