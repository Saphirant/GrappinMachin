(gci -include *.cs,*.cpp,*.h,*.idl,*.asmx -recurse | select-string .).Count 


Pause

function Pause ($Message="Press any key to continue...")
{
Write-Host -NoNewLine $Message
$null = $Host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown")
Write-Host ""
}
