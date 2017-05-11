{************************************************************************
 * This program is  free software; you can redistribute it  and modify it
 * under the terms of the GNU  General Public License as published by the
 * Free Software Foundation; either version 3  of the license or (at your
 * option) any later version.
 *
 * Author: Grigore Ionut <grigoreionut@anime-club.ro>
 ************************************************************************}
unit u_arcade_functions;

interface

uses SysUtils, Dialogs, iniFiles, u_server_management;

type TDynStringArray = array of ansistring;
type TWideDynStringArray = array of string;

function implode(const glue: ansistring; const pieces: array of ansistring): ansistring;
function explode(const separator, s: ansistring; limit: Integer = 0): TDynStringArray;

function wide_implode(const glue: string; const pieces: TWideDynStringArray): string;
function wide_explode(const separator, s: string; limit: Integer = 0): TWideDynStringArray;

procedure ArcadeH5_StartGame(emu_version:string; emulator:string; params:string);
function prepare_emu_cmd(cmd_template:string; params:string; emu_path:string=''; emu_exe:string=''):string;

implementation

uses uMain;

function wide_implode(const glue: string; const pieces: TWideDynStringArray): string;
var I: Integer;
begin
  Result := '';
  for I := 0 to High(Pieces) do
    Result := Result + Glue + Pieces[I];
  Delete(Result, 1, Length(Glue));
end;

function wide_explode(const Separator, S: string; Limit: Integer = 0): TWideDynStringArray;
  var
    SepLen: Integer;
    F, P: PWideChar;
begin
  SetLength(Result, 0);
  if (S = '') or (Limit < 0) then
    Exit;
  if Separator = '' then
    begin
      SetLength(Result, 1);
      Result[0] := S;
      Exit;
    end;
  SepLen := Length(Separator);

  P := PWideChar(S);
  while P^ <> #0 do
    begin
      F := P;
      P := StrPos(P, PWideChar(Separator));
      if (P = nil) or ((Limit > 0) and (Length(Result) = Limit - 1)) then
        P := StrEnd(F);
      SetLength(Result, Length(Result) + 1);
      SetString(Result[High(Result)], F, P - F);
      F := P;
      //ERROR in case if string is finished with separator - fix by Coyotee
      if P = Separator then
        SetLength(Result, Length(Result) + 1);
      //END of modification
      while (P^ <> #0) and (P - F < SepLen) do
        Inc(P);
    end;
end;


//////////////////////////////////////////////////////////////////////////
// Procedure - implode
// Author    - Ronald Buster
// Remarc    - PHP like implode function
//
// Returns a string containing a string representation of all the array
// elements in the same order, with the glue string between each element.
//////////////////////////////////////////////////////////////////////////

function implode(const glue: ansistring; const pieces: array of ansistring): ansistring;
var I: Integer;
begin
  Result := '';
  for I := 0 to High(Pieces) do
    Result := Result + Glue + Pieces[I];
  Delete(Result, 1, Length(Glue));
end;

//////////////////////////////////////////////////////////////////////////
// Procedure - explode
// Author    - Ronald Buster
// Remarc    - PHP like explode function
//
// Returns an array of strings, each of which is a substring of string
// formed by splitting it on boundaries formed by the string separator.
// If limit is set, the returned array will contain a maximum of limit
// elements with the last element containing the rest of string.
//
//////////////////////////////////////////////////////////////////////////

function explode(const Separator, S: ansistring; Limit: Integer = 0): TDynStringArray;
  var
    SepLen: Integer;
    F, P: PChar;
begin
  SetLength(Result, 0);
  if (S = '') or (Limit < 0) then
    Exit;
  if Separator = '' then
    begin
      SetLength(Result, 1);
      Result[0] := S;
      Exit;
    end;
  SepLen := Length(Separator);

  P := PChar(S);
  while P^ <> #0 do
    begin
      F := P;
      P := AnsiStrPos(P, PChar(Separator));
      if (P = nil) or ((Limit > 0) and (Length(Result) = Limit - 1)) then
        P := StrEnd(F);
      SetLength(Result, Length(Result) + 1);
      SetString(Result[High(Result)], F, P - F);
      F := P;
      //ERROR in case if string is finished with separator - fix by Coyotee
      if P = Separator then
        SetLength(Result, Length(Result) + 1);
      //END of modification
      while (P^ <> #0) and (P - F < SepLen) do
        Inc(P);
    end;
end;




procedure ArcadeH5_StartGame(emu_version:string; emulator:string; params:string);
var emu_pid:Cardinal;
    emu:TIniFile;
    emu_path, emu_exe:string;
    cmd,workdir:string;
begin

// check if specified emulator is known & configured
if FileExists(frmMain.basepath+'\systems\'+emulator+'.ini') then
   begin
   emu := TIniFile.Create(frmMain.basepath+'\systems\'+emulator+'.ini');

   emu_path := emu.ReadString('SYSTEM', 'PATH', '');
   emu_exe  := emu.ReadString('SYSTEM', 'EXE', '');

   // before game
   cmd     := emu.ReadString('PREGAME', 'START', '');
   workdir := emu.ReadString('PREGAME', 'WORKDIR', '');

   // pregame start
   if Trim(cmd) <> '' then
      begin
      if frmMain.debug then frmMain.debug_log.Lines.Add('ArcadeH5_StartGame :: pre game start');

      if frmMain.gs_sendjs then frmMain.crm.Browser.MainFrame.ExecuteJavaScript('ArcadeStart.exec_pregame();', frmMain.crm.Browser.MainFrame.Url, 0 );
      sleep(100);
      cmd := prepare_emu_cmd(cmd, params, emu_path, emu_exe);
      workdir := prepare_emu_cmd(workdir, params, emu_path, emu_exe);
      ExecuteAndWait(cmd, workdir);
      end;

   // game start
   if frmMain.debug then frmMain.debug_log.Lines.Add('ArcadeH5_StartGame :: game start');

   cmd     := emu.ReadString('GAME', 'START', '');
   workdir := emu.ReadString('GAME', 'WORKDIR', '');
   if frmMain.gs_sendjs then frmMain.crm.Browser.MainFrame.ExecuteJavaScript('ArcadeStart.exec_game();', frmMain.crm.Browser.MainFrame.Url, 0 );
   cmd := prepare_emu_cmd(cmd, params, emu_path, emu_exe);
   workdir := prepare_emu_cmd(workdir, params, emu_path, emu_exe);
   if (emu.ReadString('SYSTEM', 'WAITMODE', 'check4exe') = 'native') then
      begin
      if frmMain.debug then frmMain.debug_log.Lines.Add('ArcadeH5_StartGame :: game start + wait');
      ExecuteAndWait(cmd, workdir);

      // after game
      cmd     := emu.ReadString('POSTGAME', 'START', '');
      workdir := emu.ReadString('POSTGAME', 'WORKDIR', '');

      // postgame start
      if Trim(cmd) <> '' then
         begin
         if frmMain.debug then frmMain.debug_log.Lines.Add('ArcadeH5_StartGame :: post game start');
         if frmMain.gs_sendjs then frmMain.crm.Browser.MainFrame.ExecuteJavaScript('ArcadeStart.exec_postgame();', frmMain.crm.Browser.MainFrame.Url, 0 );
         cmd := prepare_emu_cmd(cmd, params, emu_path, emu_exe);
         workdir := prepare_emu_cmd(workdir, params, emu_path, emu_exe);
         ExecuteAndWait(cmd, workdir);
         end;

      if frmMain.gs_sendjs then frmMain.crm.Browser.MainFrame.ExecuteJavaScript('ArcadeStart.exec_endgame();', frmMain.crm.Browser.MainFrame.Url, 0 );
      end
   else
      begin
      if frmMain.debug then frmMain.debug_log.Lines.Add('ArcadeH5_StartGame :: game start + wait4exe');
      // start game without waiting
      ExecBackgroundProcess(cmd, 5, workdir, emu_pid);


      // use check4exe method to check for game end
      frmMain.check4exe := emu_exe;

      // after game
      cmd     := emu.ReadString('POSTGAME', 'START', '');
      workdir := emu.ReadString('POSTGAME', 'WORKDIR', '');

      // postgame start
      if Trim(cmd) <> '' then
         begin
         if frmMain.debug then frmMain.debug_log.Lines.Add('ArcadeH5_StartGame :: post game - preparing data');
         //frmMain.crm.Browser.MainFrame.ExecuteJavaScript('ArcadeStart.exec_postgame();', frmMain.crm.Browser.MainFrame.Url, 0 );
         cmd := prepare_emu_cmd(cmd, params, emu_path, emu_exe);
         workdir := prepare_emu_cmd(workdir, params, emu_path, emu_exe);
         frmMain.check4exe_post_cmd := cmd;
         frmMain.check4exe_post_dir := workdir;
         //ExecuteAndWait(cmd, workdir);
         end;

      frmMain.GameEndTimer.Enabled := true;
      end;
   // release ini file
   emu.Free;
   end
else
   begin
   if frmMain.gs_sendjs then frmMain.crm.Browser.MainFrame.ExecuteJavaScript('ArcadeStart.err_emulator_unknown();', frmMain.crm.Browser.MainFrame.Url, 0 );
   end;

end;





function prepare_emu_cmd(cmd_template:string; params:string; emu_path:string=''; emu_exe:string=''):string;
var prm,param:TWideDynStringArray;
    i:integer;
    exec_cmd:string;
begin
exec_cmd := cmd_template;
exec_cmd := StringReplace(exec_cmd, '$PATH$', emu_path, [rfIgnoreCase,rfReplaceAll]);
exec_cmd := StringReplace(exec_cmd, '$EXE$',  emu_exe,  [rfIgnoreCase,rfReplaceAll]);

prm := wide_explode('&', params);
//if frmMain.debug then
//   begin
   for i := 0 to High(prm) do
     begin
     param := wide_explode('=', prm[i],2);
     exec_cmd := StringReplace(exec_cmd, '%'+param[0]+'%', param[1], [rfReplaceAll,rfIgnoreCase]);
     end;
//   end;
Result := exec_cmd;
end;




end.
