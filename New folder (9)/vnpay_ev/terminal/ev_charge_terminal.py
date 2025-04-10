import requests
import urllib.parse
import qrcode
import logging
import os

# Tự động tạo file log cùng cấp với script
log_file = os.path.join(os.path.dirname(__file__), '..', 'charging_log.txt')

logging.basicConfig(
    filename=log_file,
    level=logging.INFO,
    format='%(asctime)s [%(levelname)s] %(message)s',
    datefmt='%Y-%m-%d %H:%M:%S'
)

# Đơn giá mỗi kWh theo chế độ
PRICE_PER_KWH = {
    'fast': 5000,
    'slow': 3000
}

# 1. Chọn chế độ
def choose_mode():
    while True:
        print("Chọn chế độ sạc:")
        print("1. Sạc nhanh (5.000 VNĐ/kWh)")
        print("2. Sạc chậm (3.000 VNĐ/kWh)")
        choice = input("Nhập số [1/2]: ")
        if choice == '1':
            return 'fast'
        elif choice == '2':
            return 'slow'
        else:
            print("❌ Lựa chọn không hợp lệ, vui lòng chọn lại.")

# 2. Nhập số tiền
def enter_amount():
    while True:
        try:
            amount = int(input("Nhập số tiền muốn thanh toán (VNĐ): "))
            if amount >= 1000:
                return amount
            else:
                print("❌ Số tiền phải từ 1.000 VNĐ trở lên.")
        except ValueError:
            print("❌ Vui lòng nhập số hợp lệ.")

# 3. Gửi request tạo URL thanh toán
def create_vnpay_qr(mode, amount):
    SERVER_URL = "http://127.0.0.1:5000/payment_return/"
    data = {
        'mode': mode,
        'amount': amount
    }
    logging.info(f"Bắt đầu giao dịch | Chế độ: {mode} | Số tiền: {amount} VNĐ")
    try:
        response = requests.post(SERVER_URL, json=data)
        if response.status_code == 200:
            result = response.json()
            payment_url = result.get('payment_url')
            if payment_url:
                print("\n==== ✅ Mã QR thanh toán ====")
                print("🔗 URL:", payment_url)

                # Sinh mã QR
                qr = qrcode.make(payment_url)
                qr.show()

                # Tuỳ chọn lưu QR ra file
                qr.save("vnpay_qr.png")
                print("📁 Đã lưu QR tại: vnpay_qr.png")
            else:
                print("❌ Không nhận được URL thanh toán từ server.")
                logging.error(f"Lỗi từ server: Không có 'payment_url' trong phản hồi {response.text}")
        else:
            print(f"❌ Lỗi tạo thanh toán: {response.status_code} {response.text}")
            logging.error(f"Lỗi tạo thanh toán: {response.status_code} {response.text}")
    except Exception as e:
        print(f"❌ Lỗi khi tạo QR thanh toán: {e}")
        logging.error(f"Lỗi khi tạo QR thanh toán: {e}")

# 4. Chạy quy trình chính
def main():
    mode = choose_mode()
    amount = enter_amount()
    price = PRICE_PER_KWH[mode]
    kwh = amount / price
    print(f"\n👉 Bạn đã chọn chế độ **{mode.upper()}** với số tiền **{amount} VNĐ**.")
    print(f"⚡ Sẽ sạc được khoảng **{kwh:.2f} kWh**.\n")

    create_vnpay_qr(mode, amount)

if __name__ == "__main__":
    main()
