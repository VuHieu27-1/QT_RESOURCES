# main.py

from zalopay.client import create_order
from zalopay.query import query_order
from charging.controller import calculate_duration, calculate_kwh, log_transaction
import time
import qrcode

def input_terminal_data():
    """
    Nhập dữ liệu từ người dùng: số tiền và chế độ sạc
    """
    while True:
        try:
            amount = int(input("💰 Nhập số tiền muốn sạc (VNĐ): ").strip())
            if amount <= 0:
                raise ValueError
            break
        except ValueError:
            print("❌ Vui lòng nhập số tiền hợp lệ (> 0)")

    while True:
        mode = input("⚡ Nhập chế độ sạc (slow / fast): ").strip().lower()
        if mode in ["slow", "fast"]:
            break
        else:
            print("❌ Chế độ không hợp lệ. Chỉ chấp nhận: slow hoặc fast")

    return amount, mode

def show_qr_terminal(data: str):
    """
    In mã QR vào terminal (dưới dạng ASCII) và lưu ảnh mã QR vào file png
    """
    print("📲 Mã QR thanh toán:")
    qr = qrcode.QRCode(border=2)
    qr.add_data(data)
    qr.make(fit=True)
    qr.print_ascii(invert=True)
    qr.make_image(fill_color="black", back_color="white").save("zalopay_qr.png")
    print("🖼️ Ảnh mã QR đã lưu tại: zalopay_qr.png")

def wait_for_payment(app_trans_id: str, timeout: int = 300, interval: int = 5) -> dict:
    """
    Chờ xác nhận thanh toán tối đa `timeout` giây (5 phút), kiểm tra mỗi `interval` giây
    Trả về kết quả đầu tiên khi giao dịch thành công.
    """
    print("⏳ Đang kiểm tra trạng thái thanh toán...")
    elapsed = 0
    while elapsed < timeout:
        result = query_order(app_trans_id)
        print(f"📡 Phản hồi từ ZaloPay: {result}")
        if result.get("order_status") == 1 or (result.get("return_code") == 1 and result.get("zp_trans_id")):
            print("✅ Thanh toán thành công!")
            return result
        time.sleep(interval)
        elapsed += interval
    print("❌ Hết thời gian chờ thanh toán.")
    return {}

def main():
    print("====================")
    print("🔋 TRẠM SẠC THÔNG MINH")
    print("====================")

    amount, mode = input_terminal_data()

    # Tạo đơn hàng
    print("📦 Đang tạo đơn hàng...")
    response = create_order(amount=amount)

    if response.get("return_code") != 1:
        print("❌ Không thể tạo đơn hàng. Chi tiết:", response)
        return

    app_trans_id = response.get("app_trans_id")
    order_url = response.get("order_url")

    if not app_trans_id or not order_url:
        print("❌ Thiếu thông tin đơn hàng trả về từ ZaloPay.")
        return

    print(f"🧾 Mã giao dịch: {app_trans_id}")
    print(f"🔗 Link thanh toán: {order_url}")
    show_qr_terminal(order_url)

    # Tự động kiểm tra trạng thái thanh toán ngay, không cần nhấn Enter
    result = wait_for_payment(app_trans_id, timeout=300)

    if not result:
        return

    # Tính toán thời gian và điện năng tiêu thụ
    duration = calculate_duration(amount, mode)
    kwh = calculate_kwh(duration, mode)

    # Ghi log giao dịch
    log_transaction(app_trans_id, amount, mode, duration, kwh)

    # Hiển thị kết quả giao dịch và thoát
    print("====================")
    print("✅ GIAO DỊCH THÀNH CÔNG")
    print(f"🔢 Số tiền: {amount} VNĐ")
    print(f"⚡ Chế độ sạc: {mode.upper()}")
    print(f"⏱️ Thời gian sạc: {duration} phút")
    print(f"🔌 Ước tính điện tiêu thụ: {kwh:.2f} kWh")
    print(f"🧾 Mã giao dịch: {app_trans_id}")
    print("====================")

if __name__ == "__main__":
    main()
