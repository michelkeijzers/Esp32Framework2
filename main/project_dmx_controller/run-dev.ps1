# Combined dev script for website and Flask simulator
# Usage: pwsh run-dev.ps1

# Check if Flask server is already running on port 8080
$flaskPort = 8080
$flaskRunning = (Test-NetConnection -ComputerName 127.0.0.1 -Port $flaskPort -WarningAction SilentlyContinue).TcpTestSucceeded

if ($flaskRunning) {
    Write-Host "Flask server already running on port $flaskPort. Skipping launch."
} else {
    Write-Host "Starting Flask simulator (server.py) on http://localhost:$flaskPort ..."
    Start-Process -NoNewWindow -FilePath python -ArgumentList 'server.py' -WorkingDirectory $PSScriptRoot/website
    Start-Sleep -Seconds 2
}

# Start Vite dev server (npm run dev)
Write-Host "Starting Vite dev server ..."
Start-Process -NoNewWindow -FilePath npm -ArgumentList 'run', 'dev' -WorkingDirectory $PSScriptRoot/website
