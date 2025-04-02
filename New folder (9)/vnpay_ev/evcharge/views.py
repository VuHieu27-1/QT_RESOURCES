# from django.http import JsonResponse
# from django.views.decorators.csrf import csrf_exempt
# from django.conf import settings
# from django.shortcuts import render
# from datetime import datetime
# from .vnpay import VNPay
# import json
# import logging
# import os
# log_file = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'charging_log.txt')
# logging.basicConfig(
#     filename=log_file,
#     level=logging.INFO,
#     format='%(asctime)s [%(levelname)s] %(message)s',
#     datefmt='%Y-%m-%d %H:%M:%S'
# )
# @csrf_exempt
# def create_payment(request):
#     if request.method == 'POST':
#         data = json.loads(request.body)
#         mode = data.get('mode', 'fast')  # fast / slow
#         amount = int(data.get('amount', 0))  # tiền người dùng nhập

#         order_id = datetime.now().strftime('%Y%m%d%H%M%S')

#         vnp = VNPay()
#         vnp.request_data = {
#             'vnp_Version': '2.1.0',
#             'vnp_Command': 'pay',
#             'vnp_TmnCode': settings.VNPAY_TMN_CODE,
#             'vnp_Amount': amount * 100,
#             'vnp_CurrCode': 'VND',
#             'vnp_TxnRef': order_id,
#             'vnp_OrderInfo': f"Sạc xe chế độ {mode} - {amount} VND",
#             'vnp_OrderType': 'billpayment',
#             'vnp_Locale': 'vn',
#             'vnp_CreateDate': datetime.now().strftime('%Y%m%d%H%M%S'),
#             'vnp_IpAddr': request.META.get('REMOTE_ADDR'),
#             'vnp_ReturnUrl': settings.VNPAY_RETURN_URL
#         }

#         payment_url = vnp.get_payment_url(settings.VNPAY_PAYMENT_URL, settings.VNPAY_HASH_SECRET_KEY)
#         return JsonResponse({'payment_url': payment_url})

#     return JsonResponse({'error': 'Method not allowed'}, status=405)

# def payment_return(request):
#     vnp = VNPay()
#     vnp.response_data = request.GET.dict()

#     if vnp.validate_response(settings.VNPAY_HASH_SECRET_KEY):
#         if vnp.response_data.get("vnp_ResponseCode") == "00":
#             logging.info(f"Giao dịch thành công | Mã: {vnp.response_data.get('vnp_TransactionNo')} | Số tiền: {int(vnp.response_data.get('vnp_Amount')) / 100} VND")
#             result = "✅ Thanh toán thành công!"
#         else:
#             logging.error("❌ Giao dịch thất bại.")
#             result = "❌ Thanh toán thất bại!"

#         return render(request, "payment_return.html", {
#             "result": result,
#             "data": vnp.response_data
#         })
#     else:
#         return render(request, "payment_return.html", {
#             "result": "⚠️ Sai checksum! Giao dịch không hợp lệ!",
#             "data": {}
#         })
from django.http import JsonResponse
from django.views.decorators.csrf import csrf_exempt
from django.conf import settings
from django.shortcuts import render
from datetime import datetime
import json
import logging
import os
from .vnpay import VNPay

# Đường dẫn file log
log_file = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'charging_log.txt')

# Cấu hình logging
logging.basicConfig(
    filename=log_file,
    level=logging.INFO,
    format='%(asctime)s [%(levelname)s] %(message)s',
    datefmt='%Y-%m-%d %H:%M:%S'
)

# Endpoint để tạo URL thanh toán VNPay
@csrf_exempt
def create_payment(request):
    if request.method == 'POST':
        try:
            # Nhận dữ liệu từ request (chế độ và số tiền)
            data = json.loads(request.body)
            mode = data.get('mode', 'fast')  # fast / slow
            amount = int(data.get('amount', 0))  # tiền người dùng nhập
        except json.JSONDecodeError:
            return JsonResponse({'error': 'Invalid JSON format'}, status=400)
        
        # Kiểm tra nếu số tiền <= 0
        if amount <= 0:
            return JsonResponse({'error': 'Amount must be greater than 0'}, status=400)

        # Tạo order_id duy nhất
        order_id = datetime.now().strftime('%Y%m%d%H%M%S')

        # Khởi tạo đối tượng VNPay
        vnp = VNPay()
        vnp.request_data = {
            'vnp_Version': '2.1.0',
            'vnp_Command': 'pay',
            'vnp_TmnCode': settings.VNPAY_TMN_CODE,
            'vnp_Amount': amount * 100,  # Tiền gửi vào VNPay là tính bằng đơn vị "đồng" (VND)
            'vnp_CurrCode': 'VND',  # Mã tiền tệ là VND
            'vnp_TxnRef': order_id,  # Mã giao dịch
            'vnp_OrderInfo': f"Sạc xe chế độ {mode} - {amount} VND",  # Thông tin giao dịch
            'vnp_OrderType': 'billpayment',  # Loại giao dịch
            'vnp_Locale': 'vn',  # Ngôn ngữ giao dịch
            'vnp_CreateDate': datetime.now().strftime('%Y%m%d%H%M%S'),  # Thời gian tạo giao dịch
            'vnp_IpAddr': request.META.get('REMOTE_ADDR'),  # Địa chỉ IP của người dùng
            'vnp_ReturnUrl': settings.VNPAY_RETURN_URL,  # URL trả về sau khi thanh toán thành công
        }

        # Tạo URL thanh toán VNPay
        payment_url = vnp.get_payment_url(settings.VNPAY_PAYMENT_URL, settings.VNPAY_HASH_SECRET_KEY)

        # Log thông tin giao dịch
        logging.info(f"Created payment URL for {order_id}: {payment_url}")

        # Trả về payment_url cho ESP32 hoặc hệ thống khác
        return JsonResponse({'payment_url': payment_url})

    # Trả về lỗi nếu không phải POST
    return JsonResponse({'error': 'Method not allowed'}, status=405)


# Endpoint để xử lý kết quả trả về từ VNPay sau khi người dùng thanh toán
def payment_return(request):
    # Khởi tạo đối tượng VNPay
    vnp = VNPay()
    vnp.response_data = request.GET.dict()

    # Kiểm tra tính hợp lệ của dữ liệu từ VNPay
    if vnp.validate_response(settings.VNPAY_HASH_SECRET_KEY):
        # Kiểm tra mã phản hồi từ VNPay
        if vnp.response_data.get("vnp_ResponseCode") == "00":
            # Giao dịch thành công
            logging.info(f"Giao dịch thành công | Mã giao dịch: {vnp.response_data.get('vnp_TransactionNo')} | Số tiền: {int(vnp.response_data.get('vnp_Amount')) / 100} VND")
            result = "✅ Thanh toán thành công!"
        else:
            # Giao dịch thất bại
            logging.error(f"❌ Giao dịch thất bại | Mã giao dịch: {vnp.response_data.get('vnp_TransactionNo')}")
            result = "❌ Thanh toán thất bại!"
        
        # Trả về kết quả giao dịch cho người dùng
        return render(request, "payment_return.html", {
            "result": result,
            "data": vnp.response_data
        })
    else:
        # Dữ liệu không hợp lệ (checksum sai)
        logging.error("❌ Sai checksum! Giao dịch không hợp lệ.")
        return render(request, "payment_return.html", {
            "result": "⚠️ Sai checksum! Giao dịch không hợp lệ!",
            "data": {}
        })




