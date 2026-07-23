# TFT LCD (ST7735) - Giao tiếp qua SPI1

## 1. Giới thiệu

Điều khiển màn hình TFT LCD (độ phân giải 128x160, driver IC dạng ST7735) qua chuẩn SPI, dùng SPI1 ở chế độ Master.

---

## 2. Sơ đồ kết nối

| Chân STM32 | Chức năng                          |
|------------|-------------------------------------|
| SPI1 (SCK/MOSI) | Truyền xung clock và dữ liệu (cấu hình qua `MX_SPI1_Init`) |
| PB0        | CS (Chip Select) - kéo xuống 0 khi giao tiếp |
| PB1        | A0 / DC (Data/Command) - 0: gửi lệnh, 1: gửi dữ liệu |
| PB10       | RES (Reset phần cứng màn hình) |

### Cấu hình SPI1
- Mode: Master, Full-Duplex (2 lines)
- Data size: 8-bit
- Clock Polarity: Low, Phase: 1 Edge (Mode 0)
- NSS: Software (tự điều khiển CS bằng tay qua PB0, không dùng NSS phần cứng)
- First bit: MSB
- Prescaler: chia 2 (tốc độ cao)

---

## 3. Giao thức CMD / DATA

Mỗi lần gửi đều theo trình tự: **kéo CS xuống → set A0 → gửi qua SPI → kéo CS lên**.

```c
void writeCMD(uint8_t cmd){
    CS = 0;
    A0 = 0;      // 0 = Command
    SPI_Transmit(cmd);
    CS = 1;
}

void writeData(uint8_t data){
    CS = 0;
    A0 = 1;      // 1 = Data
    SPI_Transmit(data);
    CS = 1;
}
```

---

## 4. Các lệnh (command) chính đã dùng

| Mã lệnh | Ý nghĩa                          |
|---------|-----------------------------------|
| `0x01`  | Software Reset                   |
| `0x11`  | Sleep Out (thoát chế độ ngủ)     |
| `0xB1`–`0xB4` | Cấu hình frame rate (normal/idle/partial mode) |
| `0xC0`–`0xC5` | Cấu hình nguồn điện (power control) |
| `0xE0`, `0xE1` | Gamma correction (positive/negative) |
| `0x36`  | Memory Data Access Control (hướng quét/ mirror) |
| `0x3A`  | Interface Pixel Format (`0x05` = RGB565, 16-bit/pixel) |
| `0x20`  | Display Inversion Off |
| `0x2A`  | Column Address Set (set vùng X) |
| `0x2B`  | Row Address Set (set vùng Y) |
| `0x2C`  | Memory Write (bắt đầu ghi dữ liệu pixel) |
| `0x29`  | Display ON |

---

## 5. Luồng khởi tạo (`initTFT`)

1. Reset phần cứng qua chân RES (kéo xuống rồi lên, delay 20ms)
2. Software Reset (`0x01`) + delay 200ms
3. Sleep Out (`0x11`) + delay 200ms
4. Cấu hình frame rate, power, gamma (theo thông số chuẩn ST7735)
5. Set Memory Access Control (`0x36`) và Pixel Format RGB565 (`0x3A = 0x05`)
6. Set vùng hiển thị full màn hình bằng `setPOS(0, 0, 128, 160)`
7. Bật màn hình (`0x29`)

---

## 6. Vẽ toàn màn hình 1 màu (`fullDisplay`)

- Gửi lệnh `0x2C` (Memory Write)
- Lặp `128 * 160` lần, mỗi lần gửi 2 byte màu theo định dạng **RGB565** (byte thấp trước, byte cao sau)

```c
writeData(color & 0xFF);   // byte thấp
writeData(color >> 8);     // byte cao
```
