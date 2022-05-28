# pcre2
# trim /g (-{70,}\X*\n\n\n)=+\X*
# benches /gU (-{70,}\n\N*\n-{70,}\X*\n\n\n)+
# benchname /gU -{70,}\n(\N*)\n-{70,}
# benchstats /gU \n(\N+)  *(\d+)  *(\d+)  *([\d\.]+ [mns]+)\n +([\d\.]+ [mns]+)  *([\d\.]+ [mns]+)  *([\d\.]+ [mns]+)\n +([\d\.]+ [mns]+)  *([\d\.]+ [mns]+)  *([\d\.]+ [mns]+)\n
 #>

Param(
    [Parameter(Mandatory)]
	[string]$BenchmarkExecutable,
	[string[]]$TagsToRun
)

if (!(Test-Path -Path $BenchmarkExecutable)) {
	Write-Error -Message "Not a valid Executable Path"
	return
}

if (!(Split-Path -Path $BenchmarkExecutable -IsAbsolute)) {
	[string]$BenchmarkExecutable = Resolve-Path -Path $BenchmarkExecutable
}

[string]$ExecutablePath = Split-Path -Path $BenchmarkExecutable
[string]$ExecutableFile = Split-Path -Path $BenchmarkExecutable -Leaf


