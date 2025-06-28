# FirmwareUpdater for Orange Pi

A Qt-based desktop application for:

- Downloading firmware folders from FTP using `wget`
- Updating firmware on 4 Orange Pi boards over SSH (via `plink` and `pscp`)
- Backing up the existing binary (`wfb_server`) with timestamped names
- Rewriting `wlan = ...` line in `wfb_server.cfg` only .75 ip
- Sending arbitrary files via MAVLink FTP (UDP)

## 📦 Dependencies
- Qt 5/6 (Core, GUI, Widgets, Network, Concurrent)
- PuTTY tools: `plink.exe`, `pscp.exe`
- `wget.exe` (e.g. from MSYS2 or GnuWin32)
- MAVLink headers (`mavlink.h`, `common/mavlink_msg_file_transfer_protocol.h`, etc.)

## 🖥️ Target Devices

This app works with the following IPs:

- `192.168.144.75`
- `192.168.144.100`
- `192.168.144.125`
- `192.168.144.185`

## 🔧 Building (Windows example)
qmake
make
