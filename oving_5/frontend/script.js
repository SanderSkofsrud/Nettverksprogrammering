document.getElementById('run-btn').addEventListener('click', function() {
    const code = document.getElementById('code-input').value;
    const language = document.getElementById('language-selector').value;
    fetch('/run-code', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
        },
        body: JSON.stringify({ code, language }),
    })
    .then(response => response.json())
    .then(data => {
        document.getElementById('output').textContent = data.output || data.error;
    })
    .catch(error => {
        console.error('Error:', error);
    });
});

// Add an event listener to the language selector
document.getElementById('language-selector').addEventListener('change', function() {
    updateTextAreaWithHelloWorld(this.value);
});

function updateTextAreaWithHelloWorld(language) {
    const helloWorldExamples = {
        python: 'print("Hello, Python!")',
        javascript: 'console.log("Hello, JS!");',
        typescript: 'console.log("Hello, TS!");',
        java: 'public class Main {\n    public static void main(String[] args) {\n        System.out.println("Hello, Java!");\n    }\n}',
        c: '#include <stdio.h>\nint main() {\n   printf("Hello, C!");\n   return 0;\n}',
        cpp: '#include <iostream>\nint main() {\n    std::cout << "Hello, C++!" << std::endl;\n    return 0;\n}',
        rust: 'fn main() {\n    println!("Hello, Rust!");\n}',
        // Add other languages here
    };

    document.getElementById('code-input').value = helloWorldExamples[language] || '';
}

// Initialize the textarea with the default language's Hello World
updateTextAreaWithHelloWorld(document.getElementById('language-selector').value);
