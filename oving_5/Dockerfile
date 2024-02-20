# Use an official Python runtime as an image
FROM python:3.9-slim

# Sett the working directory in the container
WORKDIR /usr/src/app

# Kopier innholdet i gjeldende katalog inn i containeren på /usr/src/app
# Copy the contentes in the current working directory 
COPY . .

# Installer alle nødvendige pakker spesifisert i requirements.txt
# Install all the necessary packages
RUN pip install --no-cache-dir -r requirements.txt

# Make port 5000 available outside this container
EXPOSE 5000

# Define enviroment variables
ENV FLASK_APP=app.py
ENV FLASK_RUN_HOST=0.0.0.0

# Run app.py when the container starts
CMD ["flask", "run"]
