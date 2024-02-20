import subprocess
import sys

from flask import Flask, request, jsonify, send_from_directory

app = Flask(__name__, static_folder='frontend')


@app.route('/')
def index():
    return send_from_directory(app.static_folder, 'index.html')


@app.route('/<path:path>')
def static_file(path):
    return send_from_directory(app.static_folder, path)


@app.route('/run-code', methods=['POST'])
def run_code():
    data = request.json
    code = data.get('code', '')

    try:
        # Kjør Python-kode direkte
        output = subprocess.run([sys.executable, "-c", code], capture_output=True, text=True, timeout=30)
        return jsonify({'output': output.stdout, 'error': output.stderr})
    except subprocess.TimeoutExpired:
        return jsonify({'error': 'Tidsavbrudd under kjøring av kode'})
    except Exception as e:
        return jsonify({'error': str(e)})


if __name__ == '__main__':
    app.run(debug=True, host='0.0.0.0')
