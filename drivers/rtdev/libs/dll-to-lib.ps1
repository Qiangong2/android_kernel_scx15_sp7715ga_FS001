# dll-to-lib
# Script to generate lib files from .dll and .drv
# version 1.2

# author: no2chem 1/11/2013
# version history
# 1.0 1/11/2013 initial release
# 1.1 1/11/2013 also export functions without RVA
# 1.2 1/12/2013 skip first few lines of dumpbin, fixes random ordinal duplication

[CmdletBinding()]
Param(
  [Parameter(Mandatory=$True,Position=1)]
   [string]$dllpath
)

#used to invoke batchfile properly, 
#source: https://github.com/jbake/Powershell_scripts/blob/master/Invoke-BatchFile.ps1 (Lee Holmes)
function Invoke-BatchFile
{
   param([string]$Path, [string]$Parameters)  

   $tempFile = [IO.Path]::GetTempFileName()  

   ## Store the output of cmd.exe.  We also ask cmd.exe to output   
   ## the environment table after the batch file completes  
   cmd.exe /c " `"$Path`" $Parameters && set > `"$tempFile`" " 

   ## Go through the environment variables in the temp file.  
   ## For each of them, set the variable in our local environment.  
   Get-Content $tempFile | Foreach-Object {   
       if ($_ -match "^(.*?)=(.*)$")  
       { 
           Set-Content "env:\$($matches[1])" $matches[2]  
       } 
   }  

   Remove-Item $tempFile
}

#setup environment
#Set environment variables for Visual Studio Command Prompt
Invoke-BatchFile "$env:VS110COMNTOOLS\..\..\vc\vcvarsall.bat" x86_arm

$dumpbin = "dumpbin.exe"
$lib = "lib.exe"

$dlls = Get-ChildItem $dllpath | ?{($_.Extension -eq ".dll") -or ($_.Extension -eq ".drv")}

$regexliteral = '(?<ordinalno>[0-9]+)\s+[0-9A-F]+\s+[0-9A-F]*\s+(?<function>\w*)'

$regex = New-Object System.Text.RegularExpressions.Regex ($regexliteral, [System.Text.RegularExpressions.RegexOptions]::Multiline)

$i = 0

$dlls | ForEach-Object {
    $countstring = $dlls.Count
    $status = $_.Name + " ($i of $countstring)"
    Write-Progress -Activity "Processing library files..." -Status $status -PercentComplete ($i / $dlls.Count * 100)
    Write-Host "Procssing " $_.Name
    $dllfullpath = $_.FullName
    $unprocesseddef =  &$dumpbin /exports $dllfullpath
    $unprocesseddef = $unprocesseddef | select -Skip 18

    $matches = [System.Text.RegularExpressions.MatchCollection] $regex.Matches($unprocesseddef)

    $def = "EXPORTS`r`n"
    foreach ($match in $matches)
    {
        if ($match.Groups["function"].Success)
        {
            $def += $match.Groups["function"].value + "`t@" + $match.Groups["ordinalno"].value + "`r`n"
        }     
    }

    $deffile = $_.BaseName + ".def"
    $def | Out-File $deffile

    $libfile = $_.BaseName + ".lib"

    #now generate a lib file (for some reason, lib wants MACHINE sometimes, so we hard code it)
    &$lib /def:$deffile /out:$libfile /MACHINE:arm

    $i++
}
