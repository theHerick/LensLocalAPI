# LensLocalAPI - Plataforma IoT de Visão Computacional para MVPs

Uma solução completa, open-source e de custo zero para transformar módulos **ESP32-CAM** em um sistema de visão computacional alimentado por **Google Lens** e **Firebase Realtime Database**.

---

## 🚀 Arquitetura do Sistema

```
[ ESP32-CAM ] ──(HTTPS/Base64)──> [ Firebase Realtime DB ] <──(SSE Stream)──> [ LensLocalAPI C# Engine ]
                                            │                                          │
                                            ▼                                          ▼
                                   [ Web Mobile App ]                       [ Google Lens Automation ]
```

### Componentes do Projeto:
1. **ESP32-CAM Firmware (`/esp32cam_firmware`)**: Firmware em C++ com suporte a Auto White Balance, calibração de exposição, descarte de frames iniciais e reconexão HTTPS SSL.
2. **C# Engine (`LensLocalAPI.csproj`)**: Aplicação desktop WPF em .NET 10 que escuta a fila do Firebase em tempo real via SSE e automatiza o Google Lens via Microsoft Playwright.
3. **Web / Mobile App (`/web`)**: PWA responsivo pronto para deploy no Vercel com monitoramento em tempo real do último resultado.

---

## 💻 Requisitos

- **.NET 8.0 SDK** ou superior
- **Arduino IDE** (com suporte a placas ESP32)
- **Conta no Firebase** (Realtime Database configurado em modo livre de leitura/escrita)

---

## ⚙️ Configuração Inicial

### 1. Firebase Realtime Database
Crie um projeto no Firebase e copie a URL do seu Realtime Database (exemplo: `https://seu-projeto-default-rtdb.firebaseio.com`).

### 2. Firmware ESP32-CAM
Abra a pasta `esp32cam_firmware` no Arduino IDE e configure suas credenciais:
```cpp
const char* WIFI_SSID     = "SUA_REDE_WIFI";
const char* WIFI_PASSWORD = "SUA_SENHA_WIFI";
const char* FIREBASE_URL  = "https://seu-projeto-default-rtdb.firebaseio.com";
```
Grave o firmware na sua placa ESP32-CAM.

### 3. Aplicação Desktop (.NET)
Edite o arquivo `appsettings.json` ou configure diretamente pela interface do aplicativo clicando em **Configurar Firebase**:
```json
{
  "Firebase": {
    "DatabaseUrl": "https://seu-projeto-default-rtdb.firebaseio.com",
    "QueuePath": "queue",
    "RequestsPath": "requests"
  }
}
```

---

## 🚀 Como Executar

Execute o script de 1 clique no Windows:
```cmd
iniciar.bat
```

---

## 📄 Licença

Este projeto está licenciado sob a licença MIT. Sinta-se livre para utilizar em seus produtos e MVPs.
