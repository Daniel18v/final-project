unit u_server_management;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, TlHelp32, Forms;


procedure ExecBackgroundProcess(ProgramName : String; TimeOut: Integer; Directory:string; var procPID:cardinal);
procedure ExecuteAndWait(const ProgramName: string; Directory:string);
function processExists(exeFileName: string; var procPID:cardinal): Boolean;
function KillProcess(PID: cardinal): Integer;

implementation

/// <summary> Runs a new process in the background. Waits for a short period, then checks that the process started succesfully.
/// If the process has already finished, checks the exit status. Otherwise, leaves it to run. </summary>
/// <param> ProgramName The executable name, including any parameters.</param>
/// <param> TimeOut Milliseconds to wait before checking the process has executed correctly</param>
/// <param> Directory The full path of the working directory</param>
/// <exception> Exception If the process was not started correctly or if the process was started but returned
/// an error before the timeout.</exception>
procedure ExecBackgroundProcess(ProgramName : String; TimeOut: Integer; Directory:string; var procPID:cardinal);
var
    StartInfo : TStartupInfo;
    ProcInfo : TProcessInformation;
    CreateOK : Boolean;
    status: Cardinal;
    theExitCode: Cardinal;
begin
    FillChar(StartInfo,SizeOf(TStartupInfo),#0);
    FillChar(ProcInfo,SizeOf(TProcessInformation),#0);
    StartInfo.cb := SizeOf(TStartupInfo);
    UniqueString(ProgramName); // Required if a const string is passed in. Otherwise the program crashes.
    CreateOK := CreateProcess(nil, PChar(ProgramName), nil, nil,False,
              CREATE_NEW_PROCESS_GROUP+NORMAL_PRIORITY_CLASS+CREATE_NO_WINDOW,
              nil, PChar(Directory), StartInfo, ProcInfo);

    if CreateOK then
    begin
        procPID := ProcInfo.dwProcessId;
        status:=WaitForSingleObject(ProcInfo.hProcess, TimeOut);
        if status<> WAIT_TIMEOUT then
        begin
            // Program has exited. Get exit code.
            GetExitCodeProcess(ProcInfo.hProcess, theExitCode);
           // if theExitCode<>0 then raise Exception.Create('Program '''+ProgramName+''' failed with exit code '+IntToStr(theExitCode));
        end
    end ;
   // else
     // Raise Exception.Create('Unable to run '+ProgramName+' in directory '+Directory);


    CloseHandle(ProcInfo.hProcess);
    CloseHandle(ProcInfo.hThread);
end;



/// <summary> Runs a new process and waits until it finishes.
/// <param> ProgramName The executable name.</param>
/// <param> Directory The full path of the working directory</param>
procedure ExecuteAndWait(const ProgramName: string; Directory:string);
var
  tmpStartupInfo: TStartupInfo;
  tmpProcessInformation: TProcessInformation;
  tmpProgram: String;
begin
  tmpProgram := trim(ProgramName);
  FillChar(tmpStartupInfo, SizeOf(tmpStartupInfo), 0);
  with tmpStartupInfo do
  begin
    cb := SizeOf(TStartupInfo);
    wShowWindow := SW_HIDE;
  end;

  if CreateProcess(nil, pchar(tmpProgram), nil, nil, true, CREATE_NO_WINDOW,
    nil, PChar(Directory), tmpStartupInfo, tmpProcessInformation) then
  begin
    // loop every 10 ms
    while WaitForSingleObject(tmpProcessInformation.hProcess, 10) > 0 do
    begin
      Application.ProcessMessages;
    end;
    CloseHandle(tmpProcessInformation.hProcess);
    CloseHandle(tmpProcessInformation.hThread);
  end
  else
  begin
    RaiseLastOSError;
  end;
end;



/// <summary> Checks to see if any processes exist that are started by exeFileName
/// <param> exeFileName The executable name.</param>
/// <param> procPID Variable that will receive the process id in case it's found</param>
function processExists(exeFileName: string; var procPID:cardinal): Boolean;
var
  ContinueLoop: BOOL;
  FSnapshotHandle: THandle;
  FProcessEntry32: TProcessEntry32;
begin
  FSnapshotHandle := CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
  FProcessEntry32.dwSize := SizeOf(FProcessEntry32);
  ContinueLoop := Process32First(FSnapshotHandle, FProcessEntry32);
  Result := False;
  while Integer(ContinueLoop) <> 0 do
  begin
    if ((UpperCase(ExtractFileName(FProcessEntry32.szExeFile)) =
      UpperCase(ExeFileName)) or (UpperCase(FProcessEntry32.szExeFile) =
      UpperCase(ExeFileName))) then
    begin
      procPID := FProcessEntry32.th32ProcessID;
      Result := True;
    end;
    ContinueLoop := Process32Next(FSnapshotHandle, FProcessEntry32);
  end;
  CloseHandle(FSnapshotHandle);
end;




/// <summary> Kills process by process id
/// <param> procPID Process ID for the program intended to be killed</param>
function KillProcess(PID: cardinal): Integer;
var
  PH, EC: Cardinal;
begin
  Result:= 0;
  PH:= OpenProcess(PROCESS_ALL_ACCESS, True, PID);
  GetExitCodeProcess(PH, EC);
  if (PH = 0) then
     Result:= GetLastError
  else begin
    if not TerminateProcess(PH, EC) then
    Result:= GetLastError;
    CloseHandle(PH);
  end;
end;

end.
