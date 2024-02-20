document.getElementById('run-btn').addEventListener('click', function() {
    const code = document.getElementById('code-input').value;
    fetch('/run-code', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
        },
        body: JSON.stringify({ code }),
    })
    .then(response => response.json())
    .then(data => {
        document.getElementById('output').textContent = data.output;
    })
    .catch(error => {
        console.error('Error:', error);
    });
});
