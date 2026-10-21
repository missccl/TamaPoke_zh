(() => {
  const status = document.getElementById("manifest-status");
  const button = document.getElementById("install-button");
  if (!status || !button) return;

  const manifestUrl = button.getAttribute("manifest") || "manifest.json";

  function show(message, state) {
    status.hidden = false;
    status.dataset.state = state;
    status.textContent = message;
  }

  async function verifyManifest() {
    try {
      const response = await fetch(manifestUrl, { cache: "no-cache" });
      if (!response.ok) {
        show(
          "找不到 manifest.json。請使用 Releases 的 TamaPoke-Web-Installer 套件，或完成建置後再部署此資料夾。",
          "error"
        );
        return;
      }

      const manifest = await response.json();
      const parts = manifest?.builds?.[0]?.parts;
      if (!Array.isArray(parts) || parts.length === 0) {
        show("manifest.json 格式不正確，無法安裝。", "error");
        return;
      }

      const missing = [];
      for (const part of parts) {
        if (!part?.path) {
          missing.push("(空白路徑)");
          continue;
        }
        try {
          const partResponse = await fetch(part.path, { method: "HEAD", cache: "no-cache" });
          if (!partResponse.ok) missing.push(part.path);
        } catch {
          missing.push(part.path);
        }
      }

      if (missing.length) {
        show(
          `韌體檔案尚未就緒：${missing.join("、")}。請部署完整的 Web Installer 套件。`,
          "error"
        );
        return;
      }

      const version = manifest.version ? `v${manifest.version}` : "";
      const name = manifest.name || "TamaPoke";
      show(`已載入 ${name}${version ? ` ${version}` : ""}，可以開始安裝。`, "ok");
    } catch (error) {
      console.error(error);
      show("無法讀取安裝清單。請確認此頁以 HTTP(S) 提供，而非 file://。", "error");
    }
  }

  if (!window.isSecureContext) {
    show("目前不是安全內容環境。請以 HTTPS 或 localhost 開啟。", "error");
    return;
  }

  verifyManifest();
})();
