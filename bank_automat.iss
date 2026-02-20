; bank_automat Setup Script for Inno Setup
; Requires Inno Setup 6.0 or later

#define MyAppName "Bank Automat"
#define MyAppVersion "0.1"
#define MyAppPublisher "Your Name/Company"
#define MyAppExeName "bank_automat.exe"
#define MyAppURL "https://yourwebsite.com"

[Setup]
; Perusasetukset
AppId={{4ECFDFF7-432B-43C4-9F82-4DFE4B7B5A85}}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={autopf}\{#MyAppName}
DefaultGroupName={#MyAppName}
DisableProgramGroupPage=yes
LicenseFile=
InfoBeforeFile=
InfoAfterFile=
OutputDir=output
OutputBaseFilename=bank_automatSetup
SetupIconFile=
Compression=lzma
SolidCompression=yes
WizardStyle=modern
PrivilegesRequired=admin

; Kieliasetukset
ShowLanguageDialog=auto

[Languages]
Name: "finnish"; MessagesFile: "compiler:Languages\Finnish.isl"
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
; Pääohjelma ja kaikki riippuvuudet
Source: "build\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{group}\{cm:UninstallProgram,{#MyAppName}}"; Filename: "{uninstallexe}"
Name: "{autodesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#MyAppName}}"; Flags: nowait postinstall skipifsilent

[Code]
// Tarkista että .NET Framework tai muut riippuvuudet on asennettu
function InitializeSetup(): Boolean;
begin
  Result := True;
end;