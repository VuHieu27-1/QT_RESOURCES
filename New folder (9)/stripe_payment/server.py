# server.py

from flask import Flask, request, jsonify
from zalopay.client import create_order
from zalopay.query import query_order
from charging.controller import calculate_duration, calculate_kwh, log_transaction

app = Flask(__name__)

# Tạm lưu đơn hàng trong bộ nhớ
order_store = {}

@app.route("/api/create-payment", methods=["POST"])
def create_payment():
    data = request.get_json()
    amount = data.get("amount")
    mode = data.get("mode")

    if not isinstance(amount, int) or amount <= 0:
        return jsonify({"error": "Số tiền không hợp lệ."}), 400

    if mode not in ["slow", "fast"]:
        return jsonify({"error": "Chế độ sạc không hợp lệ."}), 400

    response = create_order(amount=amount)
    if response.get("return_code") != 1:
        return jsonify({"error": "Không thể tạo đơn hàng.", "details": response}), 500

    app_trans_id = response.get("app_trans_id")
    order_store[app_trans_id] = {"amount": amount, "mode": mode}

    return jsonify({
        "app_trans_id": app_trans_id,
        "order_url": response.get("order_url"),
        "amount": amount,
        "mode": mode
    })

@app.route("/api/check-payment", methods=["POST"])
def check_payment():
    data = request.get_json()
    app_trans_id = data.get("app_trans_id")

    if not app_trans_id:
        return jsonify({"error": "Thiếu mã giao dịch (app_trans_id)."}), 400

    # Truy vấn trạng thái từ ZaloPay
    result = query_order(app_trans_id)
    if result.get("order_status") == 1 or (result.get("return_code") == 1 and result.get("zp_trans_id")):
        info = order_store.get(app_trans_id)
        if not info:
            return jsonify({"error": "Không tìm thấy đơn hàng trong hệ thống."}), 404

        amount = info["amount"]
        mode = info["mode"]
        duration = calculate_duration(amount, mode)
        kwh = calculate_kwh(duration, mode)

        # Log lại giao dịch
        log_transaction(app_trans_id, amount, mode, duration, kwh)

        return jsonify({
            "status": "paid",
            "amount": amount,
            "mode": mode,
            "duration": duration,
            "kwh": round(kwh, 2),
            "app_trans_id": app_trans_id
        })
    else:
        return jsonify({"status": "pending"})

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000, debug=True)
