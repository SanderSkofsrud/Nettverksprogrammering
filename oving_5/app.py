import sys
import tempfile
import os
import subprocess
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
    code = data['code']
    language = data['language']
    result = {"output": "", "error": ""}

    try:
        with tempfile.TemporaryDirectory() as tmpdirname:
            if language == "python":
                # Python code can be executed directly
                completed = subprocess.run(
                    [sys.executable, "-c", code],
                    capture_output=True, text=True, timeout=30
                )
            elif language == "javascript":
                filename = os.path.join(tmpdirname, "script.js")
                with open(filename, "w") as file:
                    file.write(code)
                completed = subprocess.run(
                    ["node", filename],
                    capture_output=True, text=True, timeout=30
                )

            elif language == "typescript":
                filename = os.path.join(tmpdirname, "script.ts")
                with open(filename, "w") as file:
                    file.write(code)
                subprocess.run(
                    ["tsc", filename],
                    capture_output=True, text=True, timeout=30
                )
                js_filename = os.path.join(tmpdirname, "script.js")
                completed = subprocess.run(
                    ["node", js_filename],
                    capture_output=True, text=True, timeout=30
                )
            elif language == "java":
                filename = os.path.join(tmpdirname, "Main.java")
                with open(filename, "w") as file:
                    file.write(code)
                # Compile Java code
                subprocess.run(["javac", filename], capture_output=True, text=True, timeout=30)
                # Run Java code
                completed = subprocess.run(
                    ["java", "-cp", tmpdirname, "Main"],
                    capture_output=True, text=True, timeout=30
                )
            elif language == "cpp":
                filename = os.path.join(tmpdirname, "main.cpp")
                executable = os.path.join(tmpdirname, "main")
                with open(filename, "w") as file:
                    file.write(code)
                # Compile C++ code
                subprocess.run(["g++", filename, "-o", executable], capture_output=True, text=True, timeout=30)
                # Run C++ code
                completed = subprocess.run(
                    [executable],
                    capture_output=True, text=True, timeout=30
                )
            elif language == "c":
                filename = os.path.join(tmpdirname, "main.c")
                executable = os.path.join(tmpdirname, "main")
                with open(filename, "w") as file:
                    file.write(code)
                # Compile C code
                subprocess.run(["gcc", filename, "-o", executable], capture_output=True, text=True, timeout=30)
                # Run C code
                completed = subprocess.run(
                    [executable],
                    capture_output=True, text=True, timeout=30
                )
            elif language == "rust":
                src_dir = os.path.join(tmpdirname, "src")
                os.mkdir(src_dir)
                filename = os.path.join(src_dir, "main.rs")
                with open(filename, "w") as file:
                    file.write(code)
                # Compile Rust code
                subprocess.run(["rustc", filename, "-o", tmpdirname + "/main"], capture_output=True, text=True, timeout=30)
                # Run Rust code
                completed = subprocess.run(
                    [tmpdirname + "/main"],
                    capture_output=True, text=True, timeout=30
                )
            else:
                return jsonify({"error": "Unsupported language specified"})

            result["output"] = completed.stdout
            result["error"] = completed.stderr
    except subprocess.TimeoutExpired:
        result["error"] = "Time limit exceeded while running the code"
    except Exception as e:
        result["error"] = str(e)

    return jsonify(result)

if __name__ == '__main__':
    app.run(debug=True, host='0.0.0.0')
