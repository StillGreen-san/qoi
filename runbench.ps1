Param(
	[Parameter(Mandatory)]
	[string]$BenchmarkExecutable,
	[Parameter(Mandatory)]
	[int]$RunCount,
	[Parameter(Mandatory)]
	[ValidateSet('SGS', 'Quick', 'Full', 'Encode', 'Decode')]
	[string[]]$BenchmarkTags
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

for ($i = $BenchmarkTags.Count - 1; $i -ge 0 ; $i--) {
	if (!$BenchmarkTags[$i].StartsWith('[') -or !$BenchmarkTags[$i].EndsWith(']')) {
		$Tag = $BenchmarkTags[$i]
		$BenchmarkTags[$i] = "[$Tag]"
	} # TODO make comma separated?
}

$BenchmarkObjects = @{}
[string]$CurrentLocation = Get-Location

for ($RunNumber = 1; $RunNumber -le $RunCount; $RunNumber++) {
	[int]$Progress = ($RunNumber / $RunCount) * 100
	Write-Progress -Activity "Running Benchmarks" -Status "$RunNumber / $RunCount" -PercentComplete $Progress

	Set-Location -Path $ExecutablePath
	$BenchmarkResult = &"./$ExecutableFile" $BenchmarkTags --order rand --rng-seed time | Out-String -Stream
	Set-Location -Path $CurrentLocation

	$ResultLines = $BenchmarkResult.Count
	if ($ResultLines -lt 15) {
		Write-Error -Message "No Tests were Run"
		return
	}

	$Line = 0
	for (; $Line -lt $ResultLines -and !$BenchmarkResult[$Line].StartsWith('-'); $Line++) {
	}

	while ($BenchmarkResult[$Line].StartsWith('-')) {
		$GroupName = $BenchmarkResult[$Line + 1]
		if (!$BenchmarkObjects.ContainsKey($GroupName)) {
			$BenchmarkObjects.Add($GroupName, @{})
		}
		$Line += 10

		while ($BenchmarkResult[$Line].Length -gt 0) {
			$FunctionName = ([regex]"^(.*?) +\d").Matches($BenchmarkResult[$Line + 0])[0].Groups[1].Value
			if (!$BenchmarkObjects[$GroupName].ContainsKey($FunctionName)) {
				$BenchmarkObjects[$GroupName].Add($FunctionName, [System.Collections.ArrayList]@())
			}
			$Matches = ([regex]"([\d\.]+) ([mnu]s)").Matches($BenchmarkResult[$Line + 1])
			[double]$MicroSeconds = $Matches[0].Groups[1].Value
			switch ($Matches[0].Groups[2].Value) {
				"ms" { $MicroSeconds *= 1000 }
				"ns" { $MicroSeconds *= 0.001 }
			}
			$BenchmarkObjects[$GroupName][$FunctionName].Add($MicroSeconds) | Out-Null
			$Line += 4
		}
		$Line += 1
	}
	
}

foreach ($GroupKey in $BenchmarkObjects.Keys) {
	"$GroupKey"
	foreach ($FunctionKey in $BenchmarkObjects[$GroupKey].Keys) {
		$Mean = ($BenchmarkObjects[$GroupKey][$FunctionKey] | Measure-Object -Average).Average
		switch ($Mean) {
			{ $_ -ge 1000 } { $Mean = "$([Math]::Round($Mean / 1000, 4)) ms" }
			{ $_ -le 0.001 } { $Mean = "$([Math]::Round($Mean * 1000, 4)) ns" }
			Default { $Mean = "$([Math]::Round($Mean, 4)) us" }
		}
		"    $FunctionKey = $Mean"
	} # TODO format table
}
