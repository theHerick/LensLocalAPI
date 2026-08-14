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

```mermaid
graph TD
    classDef hardware fill:#1e1b4b,stroke:#6366f1,stroke-width:2px,color:#fff,font-weight:bold;
    classDef cloud fill:#064e3b,stroke:#10b981,stroke-width:2px,color:#fff,font-weight:bold;
    classDef engine fill:#312e81,stroke:#818cf8,stroke-width:2px,color:#fff,font-weight:bold;
    classDef output fill:#18181b,stroke:#3f3f46,stroke-width:2px,color:#fff,font-weight:bold;

    subgraph Hardware_Layer ["📷 Hardware Layer"]
        ESP["ESP32-CAM (OV2640)<br/>• Hard Reset PWDN<br/>• Auto White Balance<br/>• Base64 JPEG Captures"]:::hardware
    end

    subgraph Cloud_Bridge ["☁️ Cloud & Messaging Bridge"]
        FB_REQ[("Firebase /requests<br/>(HTTPS SSL Store)")]:::cloud
        FB_QUEUE[("Firebase /queue<br/>(SSE Realtime Event)")]:::cloud
    end

    subgraph Core_Engine ["🤖 LensLocalAPI Desktop Engine (.NET 10)"]
        SSE_LISTEN["SSE Stream Listener<br/>(Non-blocking Channel)"]:::engine
        PLAYWRIGHT["Microsoft Playwright<br/>(Chromium Automation)"]:::engine
        LENS["Google Lens AI<br/>(Multimodal JSON Extraction)"]:::engine
        COLOR["Local HSV Color Classifier<br/>(RGB Hue Matrix)"]:::engine
    end

    subgraph Presentation ["📱 Presentation Layer"]
        DESKTOP["WPF Desktop Dashboard<br/>(Realtime Logs & Preview)"]:::output
        MOBILE["Vercel Mobile PWA<br/>(REST Polling & Live Feed)"]:::output
    end

    ESP -- "HTTPS PUT (Image Base64)" --> FB_REQ
    ESP -- "HTTPS PUT (Notify Fila)" --> FB_QUEUE
    FB_QUEUE -- "SSE Stream Push" --> SSE_LISTEN
    SSE_LISTEN -- "Process Request" --> PLAYWRIGHT
    PLAYWRIGHT -- "Upload & Multimodal Search" --> LENS
    LENS -- "Extract Object JSON" --> COLOR
    COLOR -- "Update /latest_result" --> FB_REQ
    FB_REQ -- "Realtime Sync" --> DESKTOP
    FB_REQ -- "REST Polling 2.5s" --> MOBILE
```

---

## 🔧 Setup & Configuration Guide (Step-by-Step)

### 1. Firebase Project Setup
1. Go to the [Firebase Console](https://console.firebase.google.com/) and create a new project.
2. In the left menu, navigate to **Build → Realtime Database** and click **Create Database**.
3. Under the **Rules** tab, set public read/write access for testing:
   ```json
   {
     "rules": {
       ".read": true,
       ".write": true
     }
   }
   ```
4. Copy your Realtime Database URL (e.g. `https://your-project-id-default-rtdb.firebaseio.com`).

---

### 2. ESP32-CAM Firmware Setup (`/esp32cam_firmware`)
1. Open `esp32cam_firmware/esp32cam_firmware.ino` in Arduino IDE.
2. Update lines 22-24 with your Wi-Fi network and Firebase Database URL:
   ```cpp
   const char* WIFI_SSID     = "YOUR_WIFI_SSID";
   const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";
   const char* FIREBASE_URL  = "https://your-project-id-default-rtdb.firebaseio.com";
   ```
3. In Arduino IDE, select board **AI Thinker ESP32-CAM** and flash the sketch.

---

### 3. Desktop Application Setup (C# .NET WPF)
You can configure your Firebase connection using **either** of the following methods:

- **Method A (GUI):** Run `iniciar.bat`, click **Configurar Firebase** in the top header, paste your Database URL, and click **Salvar e Conectar**.
- **Method B (`appsettings.json`):** Open `appsettings.json` and paste your Database URL:
  ```json
  {
    "Firebase": {
      "DatabaseUrl": "https://your-project-id-default-rtdb.firebaseio.com",
      "QueuePath": "queue",
      "RequestsPath": "requests"
    }
  }
  ```

---

### 4. Deploying the Mobile Web App to Vercel (`/web`)

To view live ESP32 recognition results on your smartphone:

#### Option A: Vercel CLI (1-Click Command)
```bash
cd web
npx vercel --prod
```

#### Option B: Vercel Dashboard (GitHub / Git Import)
1. Open `web/index.html` and paste your Firebase Database URL in line 224:
   ```javascript
   const firebaseConfig = {
       databaseURL: "https://your-project-id-default-rtdb.firebaseio.com"
   };
   ```
2. Go to [Vercel Dashboard](https://vercel.com/new), import your repository, and set the Root Directory to `web`.
3. Click **Deploy**. Your mobile dashboard will be live at `https://your-app.vercel.app`!

---

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
