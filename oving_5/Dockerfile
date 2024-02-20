# Start with a base image that has Python and Node.js installed
FROM nikolaik/python-nodejs:python3.9-nodejs14

# Set the working directory in the container
WORKDIR /usr/src/app

# Install system dependencies for C, C++, Java, Rust, and update certificates
RUN apt-get update && apt-get install -y --no-install-recommends \
    gcc \
    g++ \
    build-essential \
    && apt-get install -y --no-install-recommends ca-certificates-java \
    && update-ca-certificates -f \
    && apt-get install -y --no-install-recommends default-jdk \
    && curl https://sh.rustup.rs -sSf | sh -s -- -y \
    && npm install -g typescript \
    && rm -rf /var/lib/apt/lists/*

# Add Rust to PATH
ENV PATH="/root/.cargo/bin:${PATH}"

# Copy only the necessary dependency files
COPY requirements.txt ./

# Install any needed packages specified in requirements.txt
RUN pip install --no-cache-dir -r requirements.txt

# Copy the rest of your app's source code
COPY . .

# Make port 5000 available to the world outside this container
EXPOSE 5000

# Define environment variable for Flask
ENV FLASK_APP=app.py
ENV FLASK_RUN_HOST=0.0.0.0

# Run app.py when the container launches
CMD ["flask", "run"]
