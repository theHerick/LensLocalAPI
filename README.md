# LensLocalAPI

**Computer Vision Bridge for ESP32 & IoT Prototyping**

LensLocalAPI is an experimental computer vision bridge for resource-constrained devices such as ESP32-CAM. It lets embedded devices send images to a computer-based processing node and receive structured recognition results through Firebase, without running heavy models on the microcontroller.

The current experimental vision provider uses **Playwright + Chromium** to interact with the **Google Lens Web interface**.

---

## Why this project exists

Microcontrollers are great for sensing and control, but modern vision models can be expensive in RAM/VRAM, setup effort, and runtime requirements.

LensLocalAPI focuses on **low-friction MVP validation** for students, makers, researchers, and embedded developers who want to test visual features quickly.

This project helps during early prototyping when you want to avoid:

- heavy local AI stack setup on the microcontroller;
- CUDA/GPU requirements for some workflows;
- large model downloads and extra dependencies;
- integrating a paid cloud vision API before validating the idea.

> Local AI is still a valid approach when hardware and project constraints support it. LensLocalAPI currently prioritizes simplicity for early experiments.

---

## How it works

Current implementation flow:

```text
ESP32-CAM / Mobile Test Client
        │
        │ Image (Base64 JPEG)
        ▼
Firebase Realtime Database
  - /requests/{id}
  - /queue/{id}
        │
        │ SSE events from /queue
        ▼
LensLocalAPI (.NET WPF)
  - FirebaseStreamService (SSE listener)
  - VisionProcessor (queue + orchestration)
        │
        ├── Local preprocessing / dominant color (HSV-based)
        │
        └── Vision Provider (current)
              Playwright + Chromium + Google Lens Web
                        │
                        ▼
                 Structured VisionResult
                        │
                        ▼
Firebase updates
  - /requests/{id} (completed/error + result)
  - /latest_result
        │
        ├── WPF dashboard (local app)
        └── Web dashboard (polls /latest_result)
```

---

## Vision Provider concept

LensLocalAPI is organized as a bridge:

```text
Device → LensLocalAPI → Vision Provider
```

In the current codebase, recognition is already separated behind `IVisionRecognitionService` (`/home/runner/work/LensLocalAPI/LensLocalAPI/Vision/IVisionRecognitionService.cs`).

- **Current provider (implemented):** Google Lens Web (browser automation)
- **Possible future providers (not implemented yet):**
  - Local model provider
  - Official cloud vision providers
  - OpenAI Vision / Gemini Vision
  - Custom computer vision model

These future providers are roadmap possibilities only.

---

## Experimental Project

LensLocalAPI is an **experimental** project intended mainly for:

- prototyping;
- education;
- research;
- MVP validation.

It is **not recommended as production infrastructure**.

Current behavior depends on Google Lens Web UI automation. If Google changes that interface or behavior, this integration may stop working.

There is no guarantee of:

- uptime;
- stability;
- compatibility over time;
- deterministic latency.

---

## Google disclaimer

LensLocalAPI is an independent and unofficial experimental project. It is not affiliated with, endorsed by, sponsored by, or supported by Google. Google Lens is a trademark/service of Google.

The current experimental provider interacts with the publicly accessible Google Lens web interface through browser automation.

This project is **not designed** to bypass CAPTCHAs, authentication mechanisms, rate limits, access controls, or other technical restrictions.

---

## Features (current)

- ESP32-CAM request flow via Firebase Realtime Database
- Event-driven processing using Firebase SSE (`/queue`)
- Browser automation with Playwright + Chromium
- Experimental Google Lens Web provider integration
- Local dominant color analysis (HSV-based fallback)
- Structured result payload (`object`, `color`, `description`, `rawResult`)
- WPF desktop dashboard for status/log monitoring
- Web dashboard (`/web`) for live result viewing and mobile test uploads
- Decoupled device-to-processing architecture for IoT MVPs

---

## When should I use this?

### Good fit

- ESP32 / ESP32-CAM prototypes
- university and academic projects
- hackathons
- proof-of-concepts
- educational experiments
- local IoT MVP validation

### Not recommended

- production systems
- safety-critical applications
- high-volume image processing
- applications requiring guaranteed uptime
- applications requiring guaranteed latency
- commercial infrastructure dependent on Google Lens Web behavior

---

## Firebase Security

For quick local tests, some users start with open rules. If you do this, treat it as:

## DEVELOPMENT / LOCAL TESTING ONLY

Open rules like below make your database publicly accessible and are unsafe for production:

```json
{
  "rules": {
    ".read": true,
    ".write": true
  }
}
```

Production deployments should use Firebase Authentication and restrictive rules.

Baseline safer example (adapt for your app):

```json
{
  "rules": {
    "requests": {
      "$requestId": {
        ".read": "auth != null",
        ".write": "auth != null"
      }
    },
    "queue": {
      "$requestId": {
        ".read": "auth != null",
        ".write": "auth != null"
      }
    },
    "latest_result": {
      ".read": "auth != null",
      ".write": "auth != null"
    }
  }
}
```

Security reminders:

- open rules are for controlled prototyping only;
- never store sensitive data in open test databases;
- never publish private credentials/tokens;
- never commit secrets.

---

## Setup

### 1) Configure Firebase

1. Create a Firebase Realtime Database project.
2. Copy your database URL (example: `https://your-project-id-default-rtdb.firebaseio.com`).
3. Choose your rules strategy:
   - quick local testing (open rules, unsafe);
   - authenticated restricted rules (recommended).

### 2) Configure ESP32-CAM firmware

Edit `/home/runner/work/LensLocalAPI/LensLocalAPI/esp32cam_firmware/esp32cam_firmware.ino`:

```cpp
const char* WIFI_SSID     = "YOUR_WIFI_SSID";
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";
const char* FIREBASE_URL  = "https://your-project-id-default-rtdb.firebaseio.com";
```

### 3) Configure desktop app

Use either:

- GUI settings in the WPF app (`iniciar.bat` → Configurar Firebase), or
- `/home/runner/work/LensLocalAPI/LensLocalAPI/appsettings.json`.

### 4) Optional web dashboard

Configure Firebase web client settings in `/home/runner/work/LensLocalAPI/LensLocalAPI/web/index.html` and deploy `/web` (for example with Vercel).

---

## ESP32 / IoT MVP examples

Possible experimental use cases:

- object recognition experiments;
- approximate color identification;
- assistive-device prototypes;
- home automation triggers;
- visual classification proof-of-concepts;
- smart sensor projects;
- academic demonstrations.

Do not assume guaranteed accuracy for critical decisions.

---

## Limitations

- Depends on external web service behavior
- Browser automation can break after UI changes
- Latency is non-deterministic
- Results can vary by image/context
- No SLA
- Not suitable for critical applications
- Open Firebase rules are insecure outside controlled tests
- Visual recognition can be incorrect

---

## Roadmap (proposed)

- [ ] Strengthen provider abstraction boundaries in app services
- [ ] Add a local vision provider option
- [ ] Add official cloud API provider examples
- [ ] Improve ESP32 examples and docs
- [ ] Add Firebase Authentication example
- [ ] Add Docker support
- [ ] Improve structured response schema and confidence metadata

---

## Project structure

```text
LensLocalAPI/
├── LensLocalAPI.csproj
├── ViewModels/
├── Views/
├── Services/
├── Vision/
├── Models/
├── esp32cam_firmware/
├── web/
└── iniciar.bat
```

---

## Authorship

LensLocalAPI

Made by Herick B.
