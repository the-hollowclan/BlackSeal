; --- BlackSeal Inno Setup Script ---
[Setup]
AppName=ProgramInstalled
AppVersion=1.0
DefaultDirName={pf}\ProgramInstalled
DefaultGroupName=ProgramInstalled
OutputDir=.
OutputBaseFilename=InstallerProgram
Compression=lzma
SolidCompression=yes

[Files]
; Install everything from ./MyApp into the install dir
Source: "MyApp\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs

[Icons]
; Start Menu shortcut
Name: "{group}\ProgramInstalled"; Filename: "{app}\ProgramInstalled.exe"

; Desktop shortcut
Name: "{commondesktop}\ProgramInstsalled"; Filename: "{app}\ProgramInstalled.exe"

[Run]
; Launch the program right after installation
Filename: "{app}\ProgramInstalled.exe"; Description: "Launch App"; Flags: nowait postinstall skipifsilent

[Registry]
; Auto-run entry (Windows startup)
Root: HKCU; Subkey: "Software\Microsoft\Windows\CurrentVersion\Run"; \
    ValueType: string; ValueName: "ProgramInstalled"; ValueData: """{app}\ProgramInstalled.exe"""
    
[UninstallDelete]
; Delete the AppData folder for the app on uninstall
Type: filesandordirs; Name: "{userappdata}\ProgramInstalled"
