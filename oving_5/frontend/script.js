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
