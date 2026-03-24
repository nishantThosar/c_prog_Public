# ============================================================
#  PiMonitor.ps1 - Raspberry Pi SSH Monitor & Auto-Reconnect
#  Author  : Nishant T
#  Version : 1.0
# ============================================================

# ── Fixed Settings (no need to change these) ──
$PI_SSH_PORT         = 22
$LOG_FILE            = "$PSScriptRoot\pi_disconnect_log.txt"
$PING_INTERVAL       = 10
$RECONNECT_DELAY     = 5
$MAX_RECONNECT_TRIES = 20
$TCP_TIMEOUT_MS      = 3000

# ============================================================
#  CTRL+C CLEAN EXIT HANDLER
# ============================================================

[Console]::TreatControlCAsInput = $false

trap {
    Write-Host ""
    Write-Host "  Monitor stopped by user (Ctrl+C)." -ForegroundColor Yellow
    Add-Content -Path $LOG_FILE -Value "[$( Get-Date -Format 'yyyy-MM-dd HH:mm:ss')] [WARN] Monitor stopped by user (Ctrl+C)."
    exit 0
}

# ============================================================
#  FUNCTIONS
# ============================================================

function Write-Log {
    param([string]$Message, [string]$Level = "INFO")
    $timestamp = Get-Date -Format "yyyy-MM-dd HH:mm:ss"
    $line = "[$timestamp] [$Level] $Message"
    Add-Content -Path $LOG_FILE -Value $line
    switch ($Level) {
        "ERROR" { Write-Host $line -ForegroundColor Red }
        "WARN"  { Write-Host $line -ForegroundColor Yellow }
        "OK"    { Write-Host $line -ForegroundColor Green }
        default { Write-Host $line -ForegroundColor Cyan }
    }
}

function Play-Alert {
    param([string]$Type = "disconnect")
    if ($Type -eq "disconnect") {
        [Console]::Beep(1000, 300); Start-Sleep -Milliseconds 100
        [Console]::Beep(700,  300); Start-Sleep -Milliseconds 100
        [Console]::Beep(400,  500)
    } elseif ($Type -eq "reconnect") {
        [Console]::Beep(600,  200); Start-Sleep -Milliseconds 100
        [Console]::Beep(1000, 400)
    }
}

function Test-PiOnline {
    try {
        $tcp = New-Object System.Net.Sockets.TcpClient
        $connect = $tcp.BeginConnect($PI_HOST, $PI_SSH_PORT, $null, $null)
        $wait = $connect.AsyncWaitHandle.WaitOne($TCP_TIMEOUT_MS, $false)
        if ($wait) {
            $tcp.EndConnect($connect)
            $tcp.Close()
            return $true
        } else {
            $tcp.Close()
            return $false
        }
    } catch {
        return $false
    }
}

function Start-SSHSession {
    if ($SSH_KEY -ne "") {
        ssh -i "$SSH_KEY" -o StrictHostKeyChecking=no -o ConnectTimeout=10 "${PI_USER}@${PI_HOST}"
    } else {
        ssh -o StrictHostKeyChecking=no -o ConnectTimeout=10 "${PI_USER}@${PI_HOST}"
    }
}

function Ask-IntentionalDisconnect {
    Write-Host ""
    Write-Host "  +---------------------------------------------+" -ForegroundColor Yellow
    Write-Host "  |  Pi went offline. Was this intentional?     |" -ForegroundColor Yellow
    Write-Host "  |                                             |" -ForegroundColor Yellow
    Write-Host "  |   Y  =  Yes, I shut it down -- EXIT script  |" -ForegroundColor Yellow
    Write-Host "  |   N  =  No, it crashed -- WAIT & reconnect  |" -ForegroundColor Yellow
    Write-Host "  +---------------------------------------------+" -ForegroundColor Yellow
    Write-Host ""
    $key = $Host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown").Character
    return ($key -eq 'Y' -or $key -eq 'y')
}

function Ask-ManualReconnect {
    Write-Host ""
    Write-Host "  +---------------------------------------------+" -ForegroundColor Cyan
    Write-Host "  |  SSH session ended. Pi is still reachable.  |" -ForegroundColor Cyan
    Write-Host "  |                                             |" -ForegroundColor Cyan
    Write-Host "  |   R        = Reconnect SSH                  |" -ForegroundColor Cyan
    Write-Host "  |   Any key  = Exit monitor                   |" -ForegroundColor Cyan
    Write-Host "  +---------------------------------------------+" -ForegroundColor Cyan
    Write-Host ""
    $key = $Host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown").Character
    return ($key -eq 'R' -or $key -eq 'r')
}

function Wait-ForPiOnline {
    $tries = 0
    Write-Log "Waiting for Pi to come back online (TCP check on port $PI_SSH_PORT)..." "WARN"
    while ($tries -lt $MAX_RECONNECT_TRIES) {
        Start-Sleep -Seconds $PING_INTERVAL
        $tries++
        if (Test-PiOnline) {
            Write-Log "Pi is back online! (attempt $tries)" "OK"
            return $true
        }
        Write-Log "Pi still offline... (attempt $tries / $MAX_RECONNECT_TRIES)" "WARN"
    }
    Write-Log "Gave up waiting after $MAX_RECONNECT_TRIES attempts." "ERROR"
    return $false
}

# ============================================================
#  STARTUP BANNER — Info, Author & How To Run
# ============================================================

Clear-Host
Write-Host ""
Write-Host "  =================================================" -ForegroundColor Magenta
Write-Host "        Pi Monitor & Auto-Reconnect v1.0           " -ForegroundColor Magenta
Write-Host "  =================================================" -ForegroundColor Magenta
Write-Host ""
Write-Host "  Author  : Nishant T" -ForegroundColor White
Write-Host "  Version : 1.0" -ForegroundColor White
Write-Host ""
Write-Host "  -------------------------------------------------" -ForegroundColor DarkGray
Write-Host "  ABOUT THIS SCRIPT" -ForegroundColor Cyan
Write-Host "  -------------------------------------------------" -ForegroundColor DarkGray
Write-Host "  This script monitors your Raspberry Pi over SSH" -ForegroundColor White
Write-Host "  from Windows 11. It will:" -ForegroundColor White
Write-Host ""
Write-Host "    * Continuously check if your Pi is reachable" -ForegroundColor White
Write-Host "    * Auto-reconnect SSH if the Pi disconnects" -ForegroundColor White
Write-Host "    * Play a sound alert when Pi goes offline" -ForegroundColor White
Write-Host "    * Play a sound alert when Pi comes back online" -ForegroundColor White
Write-Host "    * Log all disconnect/reconnect events to a" -ForegroundColor White
Write-Host "      .txt file saved next to this script" -ForegroundColor White
Write-Host "    * Uses TCP port check (not ping) so it works" -ForegroundColor White
Write-Host "      on office Wi-Fi that blocks ICMP packets" -ForegroundColor White
Write-Host ""
Write-Host "  -------------------------------------------------" -ForegroundColor DarkGray
Write-Host "  HOW TO RUN" -ForegroundColor Cyan
Write-Host "  -------------------------------------------------" -ForegroundColor DarkGray
Write-Host "  1. Open PowerShell (Run as Administrator)" -ForegroundColor White
Write-Host "  2. Allow scripts to run (first time only):" -ForegroundColor White
Write-Host "       Set-ExecutionPolicy -Scope CurrentUser RemoteSigned" -ForegroundColor Yellow
Write-Host "  3. Unblock the file (first time only):" -ForegroundColor White
Write-Host "       Unblock-File -Path .\PiMonitor.ps1" -ForegroundColor Yellow
Write-Host "  4. Navigate to the script folder:" -ForegroundColor White
Write-Host "       cd C:\Users\Nishant\Downloads" -ForegroundColor Yellow
Write-Host "  5. Run the script:" -ForegroundColor White
Write-Host "       .\PiMonitor.ps1" -ForegroundColor Yellow
Write-Host ""
Write-Host "  -------------------------------------------------" -ForegroundColor DarkGray
Write-Host "  CONTROLS" -ForegroundColor Cyan
Write-Host "  -------------------------------------------------" -ForegroundColor DarkGray
Write-Host "  Ctrl+C          = Stop the monitor anytime" -ForegroundColor White
Write-Host "  exit (in SSH)   = End SSH session (will ask to" -ForegroundColor White
Write-Host "                    reconnect or stop)" -ForegroundColor White
Write-Host "  Y / N prompt    = Tell the script if you shut" -ForegroundColor White
Write-Host "                    down the Pi on purpose" -ForegroundColor White
Write-Host ""
Write-Host "  =================================================" -ForegroundColor Magenta
Write-Host ""

# ── Pause so user can read before continuing ──
Write-Host "  Press any key to continue..." -ForegroundColor DarkGray
$null = $Host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown")
Write-Host ""

# ============================================================
#  STARTUP — Ask for IP and Username
# ============================================================

# ── Ask for IP ──
do {
    Write-Host "  Enter Pi IP address " -ForegroundColor White -NoNewline
    Write-Host "(e.g. 10.182.7.147): " -ForegroundColor DarkGray -NoNewline
    $PI_HOST = Read-Host
    $PI_HOST = $PI_HOST.Trim()
    if ($PI_HOST -eq "") {
        Write-Host "  [!] IP address cannot be empty. Please try again." -ForegroundColor Red
    }
} while ($PI_HOST -eq "")

# ── Ask for Username ──
do {
    Write-Host "  Enter Pi username   " -ForegroundColor White -NoNewline
    Write-Host "(default is 'pi'):    " -ForegroundColor DarkGray -NoNewline
    $PI_USER = Read-Host
    $PI_USER = $PI_USER.Trim()
    if ($PI_USER -eq "") {
        Write-Host "  [!] Username cannot be empty. Please try again." -ForegroundColor Red
    }
} while ($PI_USER -eq "")

# ── Ask for SSH Key (optional) ──
Write-Host "  SSH key path        " -ForegroundColor White -NoNewline
Write-Host "(leave blank for password auth): " -ForegroundColor DarkGray -NoNewline
$SSH_KEY = (Read-Host).Trim()

Write-Host ""
Write-Host "  --------------------------------------------" -ForegroundColor DarkGray
Write-Host "  Host     : $PI_HOST"        -ForegroundColor White
Write-Host "  User     : $PI_USER"        -ForegroundColor White
Write-Host "  Auth     : $(if ($SSH_KEY -ne '') { 'SSH Key: ' + $SSH_KEY } else { 'Password' })" -ForegroundColor White
Write-Host "  Log File : $LOG_FILE"       -ForegroundColor White
Write-Host "  --------------------------------------------" -ForegroundColor DarkGray
Write-Host ""
Write-Host "  Press Ctrl+C anytime to stop the monitor." -ForegroundColor DarkGray
Write-Host ""

# ── Confirm before starting ──
Write-Host "  Start monitor with these settings? " -ForegroundColor Cyan -NoNewline
Write-Host "[Y/N]: " -ForegroundColor DarkGray -NoNewline
$confirm = $Host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown").Character
Write-Host ""

if ($confirm -ne 'Y' -and $confirm -ne 'y') {
    Write-Host ""
    Write-Host "  Cancelled. Exiting." -ForegroundColor Yellow
    exit 0
}

Write-Host ""
Write-Log "Monitor started. Target: ${PI_USER}@${PI_HOST}:${PI_SSH_PORT} (TCP check)"

# ============================================================
#  MAIN LOOP
# ============================================================

# Initial connectivity check
if (-not (Test-PiOnline)) {
    Write-Log "Pi is NOT reachable at startup. Waiting..." "WARN"
    $online = Wait-ForPiOnline
    if (-not $online) {
        Write-Log "Pi never came online. Exiting." "ERROR"
        exit 1
    }
}

while ($true) {

    $sessionStart = Get-Date
    Write-Log "Starting SSH session..." "OK"
    Start-SSHSession
    $duration = ((Get-Date) - $sessionStart).ToString("hh\:mm\:ss")
    Write-Log "SSH session ended. Duration: $duration" "WARN"

    if (Test-PiOnline) {
        $reconnect = Ask-ManualReconnect
        if ($reconnect) {
            Write-Log "Reconnecting by user request..." "OK"
            continue
        } else {
            Write-Log "User chose to exit monitor." "INFO"
            break
        }

    } else {
        Write-Log "Pi went OFFLINE (port $PI_SSH_PORT unreachable)." "ERROR"
        Play-Alert -Type "disconnect"

        $intentional = Ask-IntentionalDisconnect

        if ($intentional) {
            Write-Log "Intentional shutdown confirmed. Monitor exiting." "INFO"
            Write-Host ""
            Write-Host "  Monitor stopped. Goodbye!" -ForegroundColor Green
            break

        } else {
            Write-Log "Unexpected disconnect. Waiting for Pi to come back..." "WARN"
            $cameBack = Wait-ForPiOnline

            if ($cameBack) {
                Write-Log "Waiting $RECONNECT_DELAY seconds before reconnecting SSH..." "INFO"
                Start-Sleep -Seconds $RECONNECT_DELAY
                Play-Alert -Type "reconnect"
                Write-Log "Reconnecting SSH..." "OK"
            } else {
                Write-Log "Pi did not come back. Monitor exiting." "ERROR"
                break
            }
        }
    }
}

Write-Log "Monitor session ended."
Write-Host ""
Write-Host "  Log saved to: $LOG_FILE" -ForegroundColor DarkGray