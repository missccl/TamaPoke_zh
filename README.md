# TamaPoke 香港繁體中文版

適用於 Waveshare ESP32-S3-Touch-AMOLED-1.75 和 1.75C 的非官方、非商業粉絲作品。本儲存庫提供韌體及 Windows 整合安裝程式的公開原始碼。

> [!IMPORTANT]
> 儲存庫和公開安裝程式不包含寶可夢精靈圖、額外音效，亦不提供已整合這些資產的完整韌體。Windows 安裝程式會在你的電腦上從原始來源下載所需精靈圖；使用者自備資產亦只會在本機處理。

> [!WARNING]
> 請勿把 Windows 安裝程式製作的完整韌體（`*-full.bin`）重新散佈。該映像包含本儲存庫程式碼以外、權利各異的下載精靈圖和使用者資產。

## 安裝方式

### 瀏覽器線上安裝：Waveshare AMOLED 1.75

公開基本韌體的瀏覽器安裝程式只支援 **Waveshare ESP32-S3-Touch-AMOLED-1.75（16 MB）**，不支援 1.75C。

**方式 A：GitHub Pages 線上安裝（發布後）**

1. 把含有韌體、`sprites-*.pak` 和 `forms.pak` 的 zip 上傳到 GitHub Release。檔名可以是 `TamaPoke-Web-Installer-v*.zip`，也可以是其他名稱，例如 `release20261022.zip`。
2. 在儲存庫 Settings → Pages 將來源設為 **GitHub Actions**。
3. 開啟 Actions → **Deploy Web Installer** → **Run workflow**，分支選 **main**，tag 填該 Release 標籤（例如 `tamapoke-ko-zh`），再執行。  
   （`github-pages` 環境只允許從 `main` 部署；不要用 tag 觸發，否則會被 environment protection 擋下。）
4. 用最新版桌面 Chrome 或 Microsoft Edge 開啟 Pages 網址。先安裝中文韌體，插入 microSD，再按「安裝全部地區和形態」。

Release 的 zip 下載沒有瀏覽器需要的 CORS 標頭，所以頁面不能直接抓 Release 檔案。這個 workflow 會把 zip 裡的韌體、九個地區包和形態包放到同一個網站上再發布。zip 內若多一層資料夾，或路徑用反斜線，也會先抽出來。

**方式 B：自行託管 ZIP**

1. 從[本專案的 GitHub Releases](https://github.com/missccl/TamaPoke_zh/releases)下載 `TamaPoke-Web-Installer-v*.zip`，解壓後把整個資料夾放到支援 HTTPS 的靜態網站。
2. 使用最新版桌面版 Chrome 或 Microsoft Edge 開啟該網站。
3. 用 USB-C 資料線連接裝置，按網頁上的安裝按鈕，並選擇 ESP32-S3 序列埠。

瀏覽器必須支援 Web Serial，頁面亦必須透過 HTTPS 提供（`localhost` 除外）。Safari、Firefox 和手機瀏覽器不支援此安裝方式。安裝不會清除使用者資料，但更換韌體前仍建議先備份。

已經在 GitHub Release 頁下載 zip 或解壓出 `.pak` 時，在安裝頁按「選擇已下載的 .pak 或 zip」。可一次選多個 `sprites-地區.pak` 和 `forms.pak`，或只選那個 zip。其後精靈圖從本機讀取，不會再從網頁下載。若選的是解壓後的資料夾而不是壓縮檔，改按「選擇已解壓的資料夾」，並且只選含有 `manifest.json` 的那個資料夾。韌體寫入後，插入 microSD，按「連接裝置」，再按「安裝全部地區和形態」。不要直接雙擊 html 檔。可在 `web` 目錄執行 `python3 -m http.server 8080`，再用 Chrome 或 Edge 開啟 `http://localhost:8080`。

釋出的 Web Installer zip 含有韌體和地區精靈圖。未選擇本地套件、網站上又只有基本韌體時，板上可能尚未有精靈圖。

### Windows 整合安裝程式：1.75 和 1.75C

電腦和壓縮檔初次使用者，請先閱讀[安裝指南](docs/INSTALLATION_GUIDE.md)。

1. 從[本專案的 GitHub Releases](https://github.com/missccl/TamaPoke_zh/releases)下載公開安裝程式 ZIP 並解壓。
2. 將裝置喚醒，使用 USB-C 資料線連接 Windows 10/11 電腦。
3. 執行 `TamaPoke-Flasher-v*.exe`，按「새로고침」（重新整理）確認資產狀態，再按「자동 확인 후 설치」（自動檢查並安裝）。

Windows 安裝程式會以快閃記憶體容量識別 1.75（16 MB）或 1.75C（32 MB），並可按你的選擇備份／還原已辨識的 TamaPoke 儲存資料。首次執行需連接互聯網以下載安裝工具和精靈圖；無須預先安裝 Arduino 或 Python。

## 香港名稱來源

- 151 種寶可夢名稱以 [PoGoMap-GUI 的香港繁體名稱資料](https://github.com/Go2Engle/PoGoMap-GUI/blob/master/PokeAlarm/defaults/alarms.zh_hk.json)作參考。
- 招式名稱參考 [PokeAPI 的繁體中文招式資料](https://github.com/PokeAPI/pokeapi/blob/master/data/v2/csv/move_names.csv)。該資料標示為繁體中文，並非每個名稱都能證明是香港專用寫法；已知香港用語會優先採用。

這些資料只作名稱翻譯參考，不代表官方授權或認可。

## 資產與授權

Windows 安裝程式會從 [PMDCollab/SpriteCollab](https://github.com/PMDCollab/SpriteCollab) 下載所需精靈圖，並保留製作者資料、追蹤清單和授權副本。下載不會賦予額外使用權；請遵守其 [CC BY-NC 4.0](https://github.com/PMDCollab/SpriteCollab/blob/master/LICENSE.md) 條款和署名要求。

可選的圖片及音效格式請參閱[額外資產指南](docs/ADDITIONAL_ASSETS_GUIDE.md)。自備資產只會在你的電腦上處理，不會上載至外部伺服器。

## 照顧

燈光按鈕打開睡覺選單：可以睡覺或起床，也可以在晚上 7 時前請保姆。狀態都高於 15 才可以。保姆照顧到當天 19:00，期間不會掉狀態。22:00 回到主畫面會自動睡覺，當天再用燈光叫醒就不會再自動睡。晚上睡覺時心情和清潔保持不變，飽食度最低停在 50。

訓練選單標題的「一起玩」有五個小遊戲和散步。得分超過 0 會回復心情；達到滿分的 20%、50%、80% 分別得到 1、2、3 點對應訓練。原有的波波球、沙包和反應遊戲仍然在訓練列裡。散步消耗 8 點活力，用板上的計步感應器，1000 步或 20 分鐘結束。

設定頁有省電開關，以及四段亮度。第三段是原本的亮度，第四段更亮。省電會放慢畫面更新，並把閒置亮度壓低。

## 開發者建置

需要 Arduino CLI、ESP32 Arduino Core 3.3.11、.NET 10 SDK、GFX Library for Arduino 1.6.7、SensorLib 0.4.1 及 XPowersLib 0.3.3。

首次在 Windows 準備工具（PowerShell）：

```powershell
winget install ArduinoSA.CLI
winget install Microsoft.DotNet.SDK.10
# 關閉並重新開啟 PowerShell，讓 PATH 生效
arduino-cli config init
arduino-cli core update-index --additional-urls https://espressif.github.io/arduino-esp32/package_esp32_index.json
arduino-cli core install esp32:esp32@3.3.11 --additional-urls https://espressif.github.io/arduino-esp32/package_esp32_index.json
arduino-cli lib install "GFX Library for Arduino@1.6.7" "SensorLib@0.4.1" "XPowersLib@0.3.3"
```

然後在儲存庫根目錄執行：

```powershell
powershell.exe -NoProfile -ExecutionPolicy Bypass -File .\Build-Public.ps1
```

腳本會建置兩款桌面安裝程式韌體，並另外輸出只供 1.75 使用的瀏覽器安裝資料夾及 ZIP（含 `index.html`、樣式／腳本、`manifest.json` 與 `firmware/`）。瀏覽器套件只包含基本韌體，不會加入精靈圖、LittleFS 資產或使用者檔案。把該 ZIP 上傳到 Release 後，可透過 `.github/workflows/deploy-web-installer.yml` 部署到 GitHub Pages。

## 專案及權利聲明

- 本專案源自 [socquique/TamaPoke](https://github.com/socquique/TamaPoke)。
- 遊戲、名稱、角色、設計和商標的權利均屬各自權利人。本專案與 The Pokémon Company、Nintendo、Creatures、GAME FREAK、Bandai 或 Waveshare 無關，亦未獲其認可。
- `LICENSE` 的 MIT 條款只適用於原始程式碼及本儲存庫的程式碼修改，不涵蓋寶可夢相關資料、下載精靈圖、使用者資產或第三方程式庫。
- 詳情請參閱[法律及發佈說明](docs/LEGAL.md)、[製作者名單](CREDITS.md)和[第三方聲明](THIRD_PARTY_NOTICES.md)。
