# 🛰️ BTL_LTNC: EARTH DEFENDER

**Earth Defender** là một game bắn súng không gian 2D sử dụng thư viện SDL2,điều khiển một tàu vũ trụ để bảo vệ Trái Đất khỏi các đợt tấn công liên tiếp của thiên thạch và kẻ địch.
---

## 🎮 Cách chơi

- **Di chuyển tàu**: Dùng chuột
- **Bắn laser**: Nhấn `SPACE`
- **Dùng khiên bảo vệ**: Nhấn `X` (bảo vệ trong 5 giây, hồi chiêu 20 giây)
- **Ultimate (phá hủy thiên thạch)**: Nhấn `C` (hồi chiêu 30 giây)
- **Luật điểm & máu**:
  - Bắn trúng 1 thiên thạch: +1 điểm
  - Va chạm khi không có khiên: -1 máu
  - Tàu có 10 máu, **game over** khi hết máu hoặc thiên thạch va vào Trái Đất
- **Tăng độ khó**:
  - Mỗi 5 điểm: tăng tốc độ bắn và tăng số lượng thiên thạch

---

## ⚙️ Các tính năng nổi bật

- **Tự xây dựng hệ thống đối tượng**: Tàu vũ trụ, thiên thạch, đạn, kẻ địch, ultimate,...
- **Hệ thống va chạm hoàn chỉnh**: nhiều loại tương tác giữa các đối tượng game
- **Sử dụng đa thư viện SDL2**:
  - `SDL2` (hiển thị)
  - `SDL2_image` (texture)
  - `SDL2_ttf` (hiển thị chữ)
  - `SDL2_mixer` (âm thanh, nhạc nền)
- **Hệ thống cooldown & skill**: ultimate và khiên có thời gian hồi riêng
- **Hình ảnh động (animation)**: hiệu ứng cháy nổ, nền di chuyển liên tục
- **Game loop mượt mà**: kiểm soát FPS và xử lý sự kiện

---

## 🛠️ Build & chạy game

### Yêu cầu:

- C++ compiler (g++/MinGW)
- SDL2, SDL2_image, SDL2_ttf, SDL2_mixer

```bash
g++ -o EarthDefender main.cpp game.cpp collisions.cpp utils.cpp -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
./EarthDefender
