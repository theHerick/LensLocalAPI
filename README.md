<div align="center">

<pre>
    ██╗      ███████╗███╗   ██╗███████╗██╗      ██████╗  ██████╗ █████╗ ██╗      █████╗ ██████╗ ██╗
    ██║      ██╔════╝████╗  ██║██╔════╝██║     ██╔═══██╗██╔════╝██╔══██╗██║     ██╔══██╗██╔══██╗██║
    ██║      █████╗  ██╔██╗ ██║███████╗██║     ██║   ██║██║     ███████║██║     ███████║██████╔╝██║
    ██║      ██╔══╝  ██║╚██╗██║╚════██║██║     ██║   ██║██║     ██╔══██╗██║     ██╔══██╗██╔═══╝ ██║
    ███████╗ ███████╗██║ ╚████║███████╗███████╗╚██████╔╝╚██████╗██║  ██║███████╗██║  ██║██║     ██║
    ╚══════╝ ╚══════╝╚═╝  ╚═══╝╚══════╝╚══════╝ ╚═════╝  ╚═════╝╚═╝  ╚═╝╚══════╝╚═╝  ╚═╝╚═╝     ╚═╝
</pre>

<p><b>IoT Computer Vision Platform · Zero API Cost · Powered by Google Lens & Firebase</b></p>

<img src="https://img.shields.io/badge/Status-Online-brightgreen?logo=render" alt="Status" />
<img src="https://img.shields.io/badge/Engine-Google_Lens-blue?logo=google" alt="Google Lens" />
<img src="https://img.shields.io/badge/Framework-.NET_10_WPF-purple?logo=dotnet" alt=".NET 10" />
<img src="https://img.shields.io/badge/Hardware-ESP32--CAM-orange?logo=espressif" alt="ESP32-CAM" />
<img src="https://img.shields.io/badge/license-MIT-green" alt="License" />

</div>

---

## What is LensLocalAPI?

**LensLocalAPI** is a professional, open-source IoT computer vision platform that turns an ultra-affordable ($4) ESP32-CAM module into an AI-powered object and color recognition engine.

Stop paying expensive subscription fees for commercial Vision APIs. Capture photos on hardware, process them instantly through automated Google Lens multimodal search, and stream results to your mobile phone in real time.

```text
ESP32-CAM (.jpg) → LensLocalAPI → Firebase & Mobile App
```

Designed for speed, reliability, and zero-cost MVP product development.

## How it Works

LensLocalAPI provides a seamless computer vision pipeline using a real-time Server-Sent Events (SSE) streaming model:

```text
ESP32-CAM (OV2640)
       │
       ▼
Firebase Realtime DB  ──→  SSE Stream listener (/queue)
       │
       ▼
LensLocalAPI C# Engine ──→  Playwright Google Lens & Local HSV Color Classifier
       │
     ┌─┴─┐
     │   │
     ▼   ▼
   Result  Logs  ──→  Instant sync to Vercel Mobile App & Desktop UI
```

## Features

*   **Zero API Costs** — Unlimited visual recognition powered by Google Lens multimodal AI.
*   **Local HSV Color Classification** — Local C# algorithm for instant Portuguese color name classification.
*   **Realtime SSE Streaming** — Sub-second event triggers using Firebase Realtime Database.
*   **Hardware Stability** — Hardened ESP32-CAM C++ firmware with Auto White Balance, AEC, and PWDN hardware resets.
*   **Cross-Platform Mobile PWA** — Responsive Vercel-ready dashboard for tracking captures on any smartphone.
*   **Customizable UI** — Built-in Firebase configuration manager with live latency and health indicators.

## Tech Stack

| Component | Technology |
|---|---|
| **Desktop Engine** | .NET 10 WPF, C# 12 |
| **Browser Automation** | Microsoft Playwright, Chromium |
| **Hardware Firmware** | ESP32-CAM (OV2640 C++), Arduino Core 3.x |
| **Cloud & Database** | Firebase Realtime Database (SSE), Vercel |
| **Frontend Web App** | HTML5, CSS3, ES6 Modules |

## Deployment

**LensLocalAPI** comes ready out of the box.

```bash
# To run locally on Windows:
iniciar.bat
```

**Required ESP32 Settings:**
- **Board:** AI Thinker ESP32-CAM
- **Flash Frequency:** 80MHz
- **Partition Scheme:** Huge APP (3MB No OTA/1MB SPIFFS)

## Project Structure

```text
LensLocalAPI/
├── LensLocalAPI.csproj   # .NET 10 WPF project manifest
├── ViewModels/           # MainViewModel & RelayCommand logic
├── Views/                # MainWindow WPF dark UI & controls
├── Services/             # GoogleLensService, FirebaseService, ColorAnalyzer
├── Models/               # Data structures & config schemas
├── esp32cam_firmware/    # ESP32-CAM C++ firmware sketch
├── web/                  # Vercel-ready mobile dashboard
└── iniciar.bat           # 1-click startup script
```

<div align="center">
Made by Herick B.
</div>
