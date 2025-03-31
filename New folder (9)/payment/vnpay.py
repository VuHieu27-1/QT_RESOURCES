import hashlib
import hmac
import urllib.parse
from datetime import datetime
from dotenv import load_dotenv
import uuid
import os
from django.shortcuts import render, redirect
from django.http import JsonResponse
from django.conf import settings

load_dotenv()

VNPAY_TMN_CODE = os.getenv("VNPAY_TMN_CODE")
VNPAY_HASH_SECRET = os.getenv("VNPAY_HASH_SECRET")
VNPAY_PAYMENT_URL = os.getenv("VNPAY_PAYMENT_URL")
VNPAY_RETURN_URL = os.getenv("VNPAY_RETURN_URL")

# Utility Functions
def generate_txn_ref():
    return uuid.uuid4().hex.upper()

def get_client_ip(request):
    x_forwarded_for = request.META.get('HTTP_X_FORWARDED_FOR')
    if x_forwarded_for:
        ip = x_forwarded_for.split(',')[0]
    else:
        ip = request.META.get('REMOTE_ADDR')
    return ip

def create_vnpay_url(amount, order_info, ip_addr, order_type='other', bank_code='', language='vn'):
    params = {
        "vnp_Version": "2.1.0",
        "vnp_Command": "pay",
        "vnp_TmnCode": VNPAY_TMN_CODE,
        "vnp_Amount": str(amount * 100),
        "vnp_CurrCode": "VND",
        "vnp_TxnRef": generate_txn_ref(),
        "vnp_OrderInfo": order_info,
        "vnp_OrderType": order_type,
        "vnp_Locale": language,
        "vnp_ReturnUrl": VNPAY_RETURN_URL,
        "vnp_IpAddr": ip_addr,
        "vnp_CreateDate": datetime.now().strftime("%Y%m%d%H%M%S"),
    }

    if bank_code:
        params['vnp_BankCode'] = bank_code

    sorted_params = sorted(params.items())
    query_string = urllib.parse.urlencode(sorted_params)
    hash_data = '&'.join([f"{k}={v}" for k, v in sorted_params])

    secure_hash = hmac.new(
        bytes(VNPAY_HASH_SECRET, 'utf-8'),
        hash_data.encode('utf-8'),
        hashlib.sha512
    ).hexdigest()

    payment_url = f"{VNPAY_PAYMENT_URL}?{query_string}&vnp_SecureHash={secure_hash}"
    return payment_url

# Views
def payment(request):
    if request.method == 'POST':
        form = PaymentForm(request.POST)
        if form.is_valid():
            order_type = form.cleaned_data['order_type']
            amount = form.cleaned_data['amount']
            order_desc = form.cleaned_data['order_desc']
            bank_code = form.cleaned_data.get('bank_code', '')
            language = form.cleaned_data.get('language', 'vn')

            ipaddr = get_client_ip(request)

            payment_url = create_vnpay_url(amount, order_desc, ipaddr, order_type, bank_code, language)
            return redirect(payment_url)
        else:
            return render(request, "payment.html", {"title": "Thanh toán", "form": form})

    return render(request, "payment.html", {"title": "Thanh toán"})

def payment_ipn(request):
    inputData = request.GET.dict()

    if inputData:
        vnp_ResponseCode = inputData.get('vnp_ResponseCode')
        valid_signature = validate_vnpay_signature(inputData, VNPAY_HASH_SECRET)

        if valid_signature:
            if vnp_ResponseCode == '00':
                # Update transaction status here
                return JsonResponse({'RspCode': '00', 'Message': 'Confirm Success'})
            else:
                return JsonResponse({'RspCode': '01', 'Message': 'Payment Error'})
        else:
            return JsonResponse({'RspCode': '97', 'Message': 'Invalid Signature'})
    else:
        return JsonResponse({'RspCode': '99', 'Message': 'Invalid request'})

def payment_return(request):
    inputData = request.GET.dict()

    context = {
        "title": "Kết quả thanh toán",
        "order_id": inputData.get('vnp_TxnRef', ''),
        "amount": int(inputData.get('vnp_Amount', 0)) / 100,
        "order_desc": inputData.get('vnp_OrderInfo', ''),
        "vnp_TransactionNo": inputData.get('vnp_TransactionNo', ''),
        "vnp_ResponseCode": inputData.get('vnp_ResponseCode', '')
    }

    valid_signature = validate_vnpay_signature(inputData, VNPAY_HASH_SECRET)

    if valid_signature:
        context["result"] = "Thành công" if context["vnp_ResponseCode"] == "00" else "Lỗi"
    else:
        context["result"] = "Lỗi"
        context["msg"] = "Sai checksum"

    return render(request, "payment_return.html", context)

# Helper to validate response signature
def validate_vnpay_signature(data, secret_key):
    vnp_secure_hash = data.pop('vnp_SecureHash', None)
    vnp_secure_hash_type = data.pop('vnp_SecureHashType', None)

    sorted_params = sorted(data.items())
    hash_data = '&'.join([f"{k}={v}" for k, v in sorted_params])

    secure_hash = hmac.new(
        bytes(secret_key, 'utf-8'),
        hash_data.encode('utf-8'),
        hashlib.sha512
    ).hexdigest()

    return secure_hash == vnp_secure_hash
