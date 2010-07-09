; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppName "COPASI"
#define MyAppVersion "0.0.0"
#define MyAppPublisher "copasi.org"
#define MyAppURL "http://www.copasi.org/"
#define MyAppExeName "bin\CopasiUI.exe"
#define MyBuild "0"
#define MyAppId "{{00000000-0000-0000-0000-000000000000}"
#define MyWorkDir "C:\cygwin\home\shoops\environment\qt4\copasi_dev\development\InnoSetup"

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={#MyAppId}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={pf}\{#MyAppPublisher}\{#MyAppName} {#MyAppVersion}
DefaultGroupName={#MyAppName} {#MyAppVersion}
OutputDir={#MyWorkDir}
OutputBaseFilename=Copasi-{#MyBuild}-WIN32
SetupIconFile={#MyWorkDir}\addremov.ico
Compression=lzma/ultra
SolidCompression=true
InternalCompressLevel=ultra
;SignTool=Standard sign /f C:\cygwin\home\shoops\environment\qt4\copasi_dev\development\InnoSetup\copasi.pfx /p Koichi01 $f
SignedUninstaller=false
ChangesEnvironment=yes
VersionInfoVersion={#MyAppVersion}.0
ShowLanguageDialog=no
UninstallDisplayIcon={app}\share\copasi\icons\Copasi.ico
UninstallDisplayName={#MyAppName} {#MyAppVersion}

[Languages]
Name: english; MessagesFile: compiler:Default.isl

[Tasks]
Name: desktopicon; Description: {cm:CreateDesktopIcon}; GroupDescription: {cm:AdditionalIcons}; Flags: unchecked
Name: quicklaunchicon; Description: {cm:CreateQuickLaunchIcon}; GroupDescription: {cm:AdditionalIcons}; Flags: unchecked; OnlyBelowVersion: 0,6.1

[Files]
; NOTE: Don't use "Flags: ignoreversion" on any shared system files
Source: ..\setup\copasi\bin\QtXml4.dll; DestDir: {app}\bin
Source: ..\setup\copasi\bin\CopasiSE.exe; DestDir: {app}\bin
Source: ..\setup\copasi\bin\CopasiUI.exe; DestDir: {app}\bin
Source: ..\setup\copasi\bin\libmmd.dll; DestDir: {app}\bin
Source: ..\setup\copasi\bin\Qt3Support4.dll; DestDir: {app}\bin
Source: ..\setup\copasi\bin\QtCore4.dll; DestDir: {app}\bin
Source: ..\setup\copasi\bin\QtGui4.dll; DestDir: {app}\bin
Source: ..\setup\copasi\bin\QtNetwork4.dll; DestDir: {app}\bin
Source: ..\setup\copasi\bin\QtOpenGL4.dll; DestDir: {app}\bin
Source: ..\setup\copasi\bin\QtSql4.dll; DestDir: {app}\bin
Source: ..\setup\copasi\bin\QtSvg4.dll; DestDir: {app}\bin
Source: ..\setup\copasi\README.txt; DestDir: {app}
Source: ..\setup\copasi\LICENSE.txt; DestDir: {app}
Source: ..\setup\copasi\share\copasi\config\MIRIAMResources.xml; DestDir: {app}\share\copasi\config
Source: ..\setup\copasi\share\copasi\doc\html\TutWiz-Step6.html; DestDir: {app}\share\copasi\doc\html
Source: ..\setup\copasi\share\copasi\doc\html\TutWiz-Step1.html; DestDir: {app}\share\copasi\doc\html
Source: ..\setup\copasi\share\copasi\doc\html\TutWiz-Step2.html; DestDir: {app}\share\copasi\doc\html
Source: ..\setup\copasi\share\copasi\doc\html\TutWiz-Step3.html; DestDir: {app}\share\copasi\doc\html
Source: ..\setup\copasi\share\copasi\doc\html\TutWiz-Step4.html; DestDir: {app}\share\copasi\doc\html
Source: ..\setup\copasi\share\copasi\doc\html\TutWiz-Step5.html; DestDir: {app}\share\copasi\doc\html
Source: ..\setup\copasi\share\copasi\doc\html\figures\TimeCourseDialog.png; DestDir: {app}\share\copasi\doc\html\figures
Source: ..\setup\copasi\share\copasi\doc\html\figures\ModelSettingsDialog.png; DestDir: {app}\share\copasi\doc\html\figures
Source: ..\setup\copasi\share\copasi\doc\html\figures\NewPlotAdded.png; DestDir: {app}\share\copasi\doc\html\figures
Source: ..\setup\copasi\share\copasi\doc\html\figures\ObjectBrowserSelection.png; DestDir: {app}\share\copasi\doc\html\figures
Source: ..\setup\copasi\share\copasi\doc\html\figures\ObjectBrowserTree.png; DestDir: {app}\share\copasi\doc\html\figures
Source: ..\setup\copasi\share\copasi\doc\html\figures\OutputAssistant.png; DestDir: {app}\share\copasi\doc\html\figures
Source: ..\setup\copasi\share\copasi\doc\html\figures\PlotCurveSelectionDialog.png; DestDir: {app}\share\copasi\doc\html\figures
Source: ..\setup\copasi\share\copasi\doc\html\figures\PlotDefinition.png; DestDir: {app}\share\copasi\doc\html\figures
Source: ..\setup\copasi\share\copasi\doc\html\figures\PlotWindow.png; DestDir: {app}\share\copasi\doc\html\figures
Source: ..\setup\copasi\share\copasi\doc\html\figures\ReactionDialog.png; DestDir: {app}\share\copasi\doc\html\figures
Source: ..\setup\copasi\share\copasi\doc\html\figures\ReactionOverview.png; DestDir: {app}\share\copasi\doc\html\figures
Source: ..\setup\copasi\share\copasi\doc\html\figures\ReactionOverviewEmpty.png; DestDir: {app}\share\copasi\doc\html\figures
Source: ..\setup\copasi\share\copasi\doc\html\figures\ReportDefinitionDialog.png; DestDir: {app}\share\copasi\doc\html\figures
Source: ..\setup\copasi\bin\vcredist_x86.exe; DestDir: {app}; Flags: deleteafterinstall
Source: ..\setup\copasi\share\copasi\examples\YeastGlycolysis.gps; DestDir: {app}\share\copasi\examples
Source: ..\setup\copasi\share\copasi\examples\brusselator.cps; DestDir: {app}\share\copasi\examples
Source: ..\setup\copasi\share\copasi\examples\CircadianClock.cps; DestDir: {app}\share\copasi\examples
Source: ..\setup\copasi\share\copasi\examples\DimericMWC-stiff.cps; DestDir: {app}\share\copasi\examples
Source: ..\setup\copasi\share\copasi\examples\Genetic-2000Elo.xml; DestDir: {app}\share\copasi\examples
Source: ..\setup\copasi\share\copasi\examples\MAPK-HF96-layout.cps; DestDir: {app}\share\copasi\examples
Source: ..\setup\copasi\share\copasi\examples\Metabolism-2000Poo.xml; DestDir: {app}\share\copasi\examples
Source: ..\setup\copasi\share\copasi\examples\NF-kappaB.cps; DestDir: {app}\share\copasi\examples
Source: ..\setup\copasi\share\copasi\examples\Olsen2003_peroxidase.cps; DestDir: {app}\share\copasi\examples
Source: ..\setup\copasi\share\copasi\icons\CopasiDoc.ico; DestDir: {app}\share\copasi\icons
Source: ..\setup\copasi\share\copasi\icons\Copasi.ico; DestDir: {app}\share\copasi\icons

[Icons]
Name: {group}\CopasiUI; Filename: {app}\{#MyAppExeName}; WorkingDir: {userdocs}
Name: {group}\{cm:ProgramOnTheWeb,{#MyAppName}}; Filename: {#MyAppURL}
Name: {group}\License; Filename: {app}\LICENSE.txt
Name: {group}\README; Filename: {app}\README.txt
Name: {group}\{cm:UninstallProgram,{#MyAppName}}; Filename: {uninstallexe}
Name: {commondesktop}\{#MyAppName}; Filename: {app}\{#MyAppExeName}; Tasks: desktopicon; WorkingDir: {userdocs}
Name: {userappdata}\Microsoft\Internet Explorer\Quick Launch\{#MyAppName}; Filename: {app}\{#MyAppExeName}; Tasks: quicklaunchicon; WorkingDir: {userdocs}

[Run]
Filename: {app}\{#MyAppExeName}; Description: {cm:LaunchProgram,{#MyAppName}}; Flags: nowait postinstall skipifsilent; WorkingDir: {userdocs}
Filename: {app}\vcredist_x86.exe; StatusMsg: Installing Microsoft Visual C++ 2005 Runtime Libraries; Parameters: /q:a; Check: InstallRuntime

[Dirs]
Name: {app}\bin
Name: {app}\share
Name: {app}\share\copasi
Name: {app}\share\copasi\config
Name: {app}\share\copasi\doc
Name: {app}\share\copasi\doc\html
Name: {app}\share\copasi\doc\html\figures
Name: {app}\share\copasi\examples
Name: {app}\share\copasi\icons

[Registry]
Root: HKCR; SubKey: .cps; ValueType: string; ValueData: COPASI.document; Flags: uninsdeletekey
Root: HKCR; SubKey: COPASI.document; ValueType: string; ValueData: COPASI File; Flags: uninsdeletekey
Root: HKCR; SubKey: COPASI.document\Shell\Open\Command; ValueType: string; ValueData: """{app}\bin\CopasiUI.exe"" ""%1"""; Flags: uninsdeletevalue
Root: HKCR; Subkey: COPASI.document\DefaultIcon; ValueType: string; ValueData: {app}\share\copasi\icons\CopasiDoc.ico,-1; Flags: uninsdeletevalue
Root: HKLM; Subkey: SYSTEM\CurrentControlSet\Control\Session Manager\Environment; ValueType: string; ValueName: COPASIDIR; ValueData: {app}
Root: HKLM; Subkey: SYSTEM\CurrentControlSet\Control\Session Manager\Environment; ValueType: expandsz; ValueName: Path; ValueData: "%COPASIDIR%\bin;{olddata}"; Check: UpdatePath

[Code]
function UpdatePath(): Boolean;
var
  CurrentPath: String;
  Position: Integer;

begin
  Result := True;

  if RegQueryStringValue(HKLM,
    'SYSTEM\CurrentControlSet\Control\Session Manager\Environment',
    'Path', CurrentPath) then
    // Successfully read the value
    begin
      Position := Pos('%COPASIDIR%\bin', CurrentPath);
      if (Position <> 0) then
        // Found an entry to the COPASI binaries in the Path
        begin
          Result := False;
        end;
    end;
end;

function InstallRuntime(): Boolean;
begin
  Result := True;

  if RegKeyExists(HKEY_LOCAL_MACHINE,
    'SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\{A49F249F-0C91-497F-86DF-B2585E8E76B7}') then
  begin
    Result := False;
  end;
end;

function InitializeSetup(): Boolean;
begin
  Result := True;

  RegDeleteValue(HKEY_CURRENT_USER, 'Environment', 'COPASIDIR');
end;
