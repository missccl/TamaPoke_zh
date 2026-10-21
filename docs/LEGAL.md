# 法律及發佈說明

本頁說明公開儲存庫、瀏覽器套件和 Windows 安裝程式包含哪些資料，以及哪些素材會在使用者電腦上準備。本頁不是法律意見。

## 公開內容

- 原始 TamaPoke 程式碼及本專案的香港繁體中文版修改
- 不含遊戲資產的 ESP32 基本韌體，包括中文字型子集
- Windows 安裝程式原始碼、文件和建置腳本
- ccs21 自製的示範圖片和 0 位元組音效標記

## 不會隨公開韌體提供的內容

- 寶可夢精靈圖及其轉換檔
- 使用者自備的圖片、實際音效和其他私人素材
- 含有以上素材的完整 `*-full.bin` 韌體

瀏覽器安裝 ZIP 只含不帶精靈圖和 LittleFS 資產映像的基本韌體。Windows 安裝程式會在使用者電腦上下載必要精靈圖、處理可選資產、建立 LittleFS，並產生裝置專用的完整韌體。自備素材不會上載至外部伺服器；下載和產生的檔案會儲存在執行檔旁的 `TamaPoke-Flasher-Data`。

## 授權範圍

- `LICENSE` 的 MIT 條款適用於原始專案程式碼及本儲存庫的程式碼修改。
- Noto Sans CJK HK 字型子集依 SIL Open Font License 1.1 授權；詳見 `licenses/NotoSansCJK-OFL.txt`。
- SpriteCollab 精靈圖依其 CC BY-NC 4.0 條款及各製作者署名要求使用。
- Arduino、.NET、NAudio 和其他第三方元件各自受其授權約束，詳見 `THIRD_PARTY_NOTICES.md`。
- 使用者必須自行確認其圖片和音效的嵌入、修改及使用權。
- 寶可夢、Tamagotchi 相關名稱、角色、設計和商標屬於各自權利人。

開源授權只適用於授權提供者有權授予的範圍。下載或轉換素材的功能不會授予商標、角色或其他權利。

## 完整韌體的重新散佈

本專案不授權或建議第三方重新散佈使用者建立的 `*-full.bin`、精靈圖快取或 `Additional_assets.zip`。完整韌體可能結合權利不同的程式碼和素材；重新散佈者須自行遵守每項素材的使用條件和署名要求。

## 非官方作品

本專案是非官方、非商業粉絲作品，與 The Pokémon Company、Nintendo、Creatures、GAME FREAK、Bandai 或 Waveshare 無關，亦未獲其認可。相關名稱、角色、設計和商標均屬各自權利人。
