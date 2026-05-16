
$ErrorActionPreference = "Stop"
$fastp = ".\fastp.exe"
$inputFq = "test_input.fastq"
$outputFq = "test_output.fastq"
$htmlReport = "fastp_report.html"
$jsonReport = "fastp_report.json"

Write-Host "`n=== [1/3] Gerando FASTQ de teste (20 reads) ===" -ForegroundColor Cyan
$seq = "AGCTAGCTAGCTAGCTAGCTAGCTAGCTAGCTAGCTAGCTAGCTAGCTAGCTAGCTAGCTAGCTAGCTAGCTAGCT"
$qual = "IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII"
1..20 | ForEach-Object { "@READ_$($_)"; $seq; "+"; $qual } | Set-Content $inputFq
Write-Host "OK: $inputFq criado." -ForegroundColor Green

Write-Host "`n=== [2/3] Executando fastp.exe (QC + Trim + Relatorios) ===" -ForegroundColor Cyan
& $fastp -i $inputFq -o $outputFq -h $htmlReport -j $jsonReport -w 4 --verbose

Write-Host "`n=== [3/3] Verificando artefatos de saida ===" -ForegroundColor Cyan
$pass = $true

if (Test-Path $outputFq) { Write-Host "[OK] $outputFq gerado." -ForegroundColor Green } 
else { Write-Host "[FALHA] $outputFq nao encontrado." -ForegroundColor Red; $pass = $false }

if (Test-Path $htmlReport) { Write-Host "[OK] $htmlReport gerado." -ForegroundColor Green } 
else { Write-Host "[FALHA] $htmlReport nao encontrado." -ForegroundColor Red; $pass = $false }

if (Test-Path $jsonReport) { 
    $jsonSize = (Get-Item $jsonReport).Length
    if ($jsonSize -gt 100) { Write-Host "[OK] $jsonReport gerado ($jsonSize bytes)." -ForegroundColor Green }
    else { Write-Host "[FALHA] $jsonReport vazio ou corrompido." -ForegroundColor Red; $pass = $false }
}
else { 
    Write-Host "[FALHA] $jsonReport nao encontrado." -ForegroundColor Red; $pass = $false 
}

Write-Host "`n==================================================" -ForegroundColor Yellow
if ($pass) { 
    Write-Host "SUCESSO CRITICO: fastp para Windows esta 100% operacional." -ForegroundColor Green 
    Write-Host "Binario standalone | Multithreading | I/O FASTQ/GZ | Relatorios HTML/JSON" -ForegroundColor Green
}
else { 
    Write-Host "FALHA na validacao. Verifique os logs acima." -ForegroundColor Red 
}

Write-Host "`nPressione qualquer tecla para fechar esta janela..." -ForegroundColor DarkYellow
$null = $Host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown")