import { execSync } from "child_process";
import fs from "fs";

function getIPAddress() {
  // Lệnh để lấy địa chỉ IP trên Ubuntu/Linux
  const ip = execSync("hostname -I | awk '{print $1}'").toString().trim();
  return ip;
}

function setEnvVariable() {
  const ip = getIPAddress();
  console.log(`Địa chỉ IP của bạn là: ${ip}`);

  // Tạo hoặc cập nhật file .env
  const content = `VITE_BACKEND_URL=http://${ip}:5050\n`;
  fs.writeFileSync(".env", content);
}

setEnvVariable();
