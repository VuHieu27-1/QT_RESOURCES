import requests
import urllib.parse

# Đơn giá mỗi kWh
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
            print("Lựa chọn không hợp lệ, vui lòng chọn lại.")

# 2. Nhập số tiền
def enter_amount():
    while True:
        try:
            amount = int(input("Nhập số tiền muốn thanh toán (VNĐ): "))
            if amount >= 1000:
                return amount
            else:
                print("Số tiền phải từ 1.000 VNĐ trở lên.")
        except ValueError:
            print("Vui lòng nhập số hợp lệ.")

# 3. Gửi request tạo URL thanh toán
def create_vnpay_qr(mode, amount):
    # Gửi POST request tới server Django của bạn
    SERVER_URL = "http://localhost:5000/create_payment/"
    data = {
        'mode': mode,
        'amount': amount
    }
    try:
        response = requests.post(SERVER_URL, json=data)
        result = response.json()
        print("\n==== Mã QR thanh toán ====")
        print("URL Thanh toán:", result['payment_url'])
        print("Vui lòng dùng ứng dụng VNPay hoặc ngân hàng quét mã QR này để thanh toán.\n")
    except Exception as e:
        print("Lỗi khi tạo QR thanh toán:", e)

def main():
    mode = choose_mode()
    amount = enter_amount()
    price = PRICE_PER_KWH[mode]
    kwh = amount / price
    print(f"\n👉 Bạn đã chọn chế độ '{mode.upper()}' với số tiền {amount} VNĐ.")
    print(f"⚡ Sẽ sạc được khoảng {kwh:.2f} kWh.")

    create_vnpay_qr(mode, amount)

if __name__ == "__main__":
    main()
