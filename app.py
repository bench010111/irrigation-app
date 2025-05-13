from flask import Flask, render_template, request, jsonify
import os

app = Flask(__name__)

# 상태를 저장할 변수
valve_state = {f"{i}-{j}": "off" for i in range(1, 6) for j in [1, 2]}  # 관수-밸브 상태
schedule = {f"{i}-{j}": {"on": None, "off": None} for i in range(1, 6) for j in [1, 2]}  # 예약 시간

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/toggle-valve', methods=['POST'])
def toggle_valve():
    irrigation = request.form['irrigation']
    valve = request.form['valve']
    state = request.form['state']
    
    # 수동 제어 상태 변경
    valve_state_key = f"{irrigation}-{valve}"
    valve_state[valve_state_key] = state
    
    print(f"관수 {irrigation}, 밸브 {valve} 상태: {state}")
    return jsonify({"status": "success"}), 200

@app.route('/set-schedule', methods=['POST'])
def set_schedule():
    irrigation = request.form['irrigation']
    valve = request.form['valve']
    time_type = request.form['timeType']
    value = request.form['value']
    
    # 예약 시간 저장
    schedule_key = f"{irrigation}-{valve}"
    schedule[schedule_key][time_type] = value
    
    print(f"관수 {irrigation}, 밸브 {valve} 예약 {time_type}: {value}")
    return jsonify({"status": "success"}), 200

if __name__ == "__main__":
    port = int(os.environ.get("PORT", 10000))
    app.run(host='0.0.0.0', port=port, debug=True)
