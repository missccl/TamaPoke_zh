# TamaPoke 安裝指南

本指南介紹瀏覽器基本韌體安裝，以及可準備遊戲資產的 Windows 整合安裝程式。

## 選擇安裝方式

| 方式 | 支援裝置 | 寶可夢精靈圖／自備資產 |
| --- | --- | --- |
| 瀏覽器安裝 | Waveshare ESP32-S3-Touch-AMOLED-1.75（16 MB） | 不包含 |
| Windows 整合安裝程式 | Waveshare ESP32-S3-Touch-AMOLED-1.75（16 MB）及 1.75C（32 MB） | 可下載／加入 |

**瀏覽器安裝不支援 1.75C。** 如不確定板子型號，請查看裝置標籤；不要把 1.75 韌體安裝到其他 ESP32-S3 板上。

## 瀏覽器線上安裝（只限 1.75）

### 需要準備

- Waveshare ESP32-S3-Touch-AMOLED-1.75（16 MB）
- 可傳輸資料的 USB-C 線
- 最新版桌面版 Google Chrome 或 Microsoft Edge
- 透過 HTTPS 提供的安裝網頁（GitHub Pages 或自架靜態網站），以及穩定的互聯網連線

Web Serial 需要安全連線和瀏覽器支援。Safari、Firefox 和手機瀏覽器不適用。若使用 Linux，瀏覽器可能需要額外的序列埠存取權限。

### 方式 A：GitHub Pages 線上安裝

1. 維護者執行 `Build-Public.ps1`，把產生的 `TamaPoke-Web-Installer-v*.zip` 一併上傳到 GitHub Release。
2. 在儲存庫 Settings → Pages 將來源設為 **GitHub Actions**（只需設定一次）。
3. 開啟 Actions → **Deploy Web Installer** → **Run workflow**：分支選 **main**，並填入 Release 標籤（例如 `webinstaller`）。  
   此儲存庫的 `github-pages` 環境只允許 `main` 部署；從 tag 觸發會出現 “Tag … is not allowed to deploy to github-pages”。
4. 使用者用桌面 Chrome／Edge 開啟 Pages 網址，喚醒裝置並以 USB-C 連接，按「連接裝置並安裝」。

### 方式 B：自行託管 ZIP

1. 從[本專案的 GitHub Releases](https://github.com/missccl/TamaPoke_zh/releases)下載 `TamaPoke-Web-Installer-v*.zip`，並解壓縮。
2. 把解壓後的所有檔案（包括 `index.html`、`installer.css`、`installer.js`、`manifest.json` 和 `firmware` 資料夾）放在同一個 HTTPS 靜態網站目錄。
3. 讓瀏覽器開啟該目錄的 `index.html`。只在本機測試時可使用 `localhost`；直接以 `file://` 開啟不受支援。
4. 喚醒裝置，使用 USB-C 資料線連接電腦。
5. 按網頁上的「連接裝置並安裝」，在系統對話框選擇裝置的 ESP32-S3 序列埠，並依畫面提示完成安裝。
6. 安裝期間不要拔除 USB 線或關閉瀏覽器頁面。

如果沒有顯示序列埠，請改用資料線、USB 埠或支援的桌面瀏覽器，並檢查系統序列埠權限。頁面若顯示找不到 `manifest.json` 或韌體檔案，代表部署的不是完整套件。

### 公開基本韌體的限制

瀏覽器套件只包含基本韌體，不包含寶可夢精靈圖、LittleFS 資產映像、額外音效或使用者自備資產。首次安裝後，遊戲可能沒有精靈圖；安裝程式不會從網上下載或建立資產映像。這是為了避免在公開套件中散佈第三方或使用者資產。

安裝程式會寫入開機程式、分割區表和基本應用程式，但不會要求清除整個快閃記憶體。儲存資料和現有檔案系統一般不會被安裝程式主動清除；不過，任何韌體更新前都應先備份重要資料。需要精靈圖、自備圖片或音效時，請使用 Windows 整合安裝程式。

## Windows 整合安裝程式（1.75／1.75C）

### 需要準備

- Windows 10 或 Windows 11 電腦
- Waveshare ESP32-S3-Touch-AMOLED-1.75 或 1.75C
- 可傳輸資料的 USB-C 線
- 互聯網連線
- GitHub Releases 的公開安裝程式 ZIP

### 安裝步驟

1. 從[本專案的 GitHub Releases](https://github.com/missccl/TamaPoke_zh/releases)下載最新的公開安裝程式 ZIP，使用「全部解壓縮」將檔案解壓至資料夾；不要直接從 ZIP 執行程式。
2. 確認 `TamaPoke-Flasher-v*.exe` 和 `sample_Additional_assets.zip` 位於同一資料夾。
3. 喚醒裝置，並以 USB-C 資料線連接電腦。
4. 執行安裝程式，按「새로고침」（重新整理）確認資產狀態及 COM 埠。
5. 按「자동 확인 후 설치」（自動檢查並安裝），並依畫面提示選擇是否保留相容的現有儲存資料。

首次使用時，安裝程式會下載必要工具和精靈圖、產生 LittleFS，並建立對應板子的完整韌體。此過程需要互聯網，可能需要一些時間；請勿在工作仍進行時關閉程式或拔除 USB 線。安裝程式會以快閃記憶體容量判別 1.75 或 1.75C。

Windows 可能會顯示未簽署程式的安全提示。請先確認程式來自本專案的官方 Releases，再決定是否執行。

### 自備圖片或音效

預設安裝可直接使用 ZIP 附帶的 `sample_Additional_assets.zip`，不需修改。若要使用自己的資產：

1. 複製 `sample_Additional_assets.zip`，只修改複本。
2. 將複本命名為 `Additional_assets.zip`，再依[額外資產指南](ADDITIONAL_ASSETS_GUIDE.md)更換圖片或音效。
3. 保留 `sample_Additional_assets.zip` 作為備份，並把兩個 ZIP 與安裝程式放在同一資料夾。

若兩個 ZIP 都存在，安裝程式會優先使用 `Additional_assets.zip`。缺少或損壞的檔案會先詢問你是否以樣本檔補上；重複音效檔必須自行清理。只有在你有權使用的素材才可加入。自備素材只會在你的電腦上處理，不會上載至外部伺服器。更換素材後，按「새로고침」（重新整理）。

安裝程式首次執行時建立的檔案會存放於程式旁的 `TamaPoke-Flasher-Data` 資料夾：

```text
TamaPoke-Flasher-Data\
├─ Cache\       下載的精靈圖、轉換資產及完整韌體
├─ Payload\     安裝工具和不含資產的基本韌體
├─ Logs\        安裝記錄
└─ Backups\     你選擇保留的儲存資料備份
```

## 常見問題

### 瀏覽器沒有顯示序列埠

確認你使用最新版桌面版 Chrome／Edge、HTTPS 網頁和資料傳輸 USB 線。也請確認裝置已喚醒，並允許瀏覽器存取序列埠。

### 瀏覽器安裝後沒有精靈圖

這是公開基本韌體的預期限制。請使用 Windows 整合安裝程式建立包含精靈圖和可選自備資產的映像。

### 安裝程式找不到額外資產

請確認 `sample_Additional_assets.zip` 與執行檔位於同一資料夾；如果使用自備資產，請檢查 `Additional_assets.zip` 的檔名和內部資料夾結構。更換素材後按「새로고침」（重新整理）。

### Windows 安裝程式找不到裝置

喚醒裝置、改用 USB-C 資料線或另一個 USB 埠，並確認沒有其他程式正在使用 COM 埠。

### 保存資料

Windows 安裝程式只會辨識已知 TamaPoke 格式的韌體，並依你選擇備份／還原相容資料。其他韌體的資料未必相容。瀏覽器安裝不提供儲存資料備份功能；更新前請使用合適的方法備份。

## 發佈和資產權利

公開程式碼和瀏覽器安裝套件不附帶 Pokémon 精靈圖或使用者素材。Windows 安裝程式在安裝期間從 [SpriteCollab](https://github.com/PMDCollab/SpriteCollab) 下載所需精靈圖，並保留其來源及授權資料。下載動作不會授予使用權。請遵守該專案的 [CC BY-NC 4.0](https://github.com/PMDCollab/SpriteCollab/blob/master/LICENSE.md) 條款和署名要求。

請勿重新散佈含有下載精靈圖或使用者資產的完整 `*-full.bin` 韌體。只散佈公開安裝程式及不含資產的基本韌體。
