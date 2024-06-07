import threading
import joblib
import requests
import time
from flask import Flask, render_template, request

app = Flask(__name__)

# Initialize global variables to store ThingSpeak data
moisture_data = ''
temperature_data = ''
humidity_data = ''

# Locks to ensure thread safety while updating global variables
lock = threading.Lock()

# Function to update ThingSpeak data in the background
def update_thingspeak_data():
    global moisture_data, temperature_data, humidity_data
    while True:
        try:
            url = 'https://api.thingspeak.com/channels/2536473/feeds/last.json?api_key=KJAQM5X243JPPGGC'
            response = requests.get(url)
            if response.status_code == 200:
                data = response.json()['feeds'][0]
                with lock:
                    moisture_data = data.get('field1', '')
                    temperature_data = data.get('field2', '')
                    humidity_data = data.get('field3', '')
            else:
                print("Error fetching data from ThingSpeak")
        except Exception as e:
            print(f"Error: {e}")

        # Wait for some time before fetching data again (adjust as needed)
        time.sleep(60)  # Fetch data every minute

# Start the background thread to continuously update ThingSpeak data
update_thread = threading.Thread(target=update_thingspeak_data)
update_thread.daemon = True
update_thread.start()


@app.route('/')
def home():
    return render_template('Home_1.html')


@app.route('/Predict')
def prediction():
    return render_template('Index.html')


@app.route('/form', methods=["POST"])
def brain():
    Nitrogen = float(request.form['Nitrogen'])
    Phosphorus = float(request.form['Phosphorus'])
    Potassium = float(request.form['Potassium'])
    Temperature = float(request.form['Temperature'])
    Humidity = float(request.form['Humidity'])
    Ph = float(request.form['ph'])
    Rainfall = float(request.form['Rainfall'])

    values = [Nitrogen, Phosphorus, Potassium, Temperature, Humidity, Ph, Rainfall]

    if Ph > 0 and Ph <= 14 and Temperature < 100 and Humidity > 0:
        joblib.load('crop_app', 'r')
        model = joblib.load(open('crop_app', 'rb'))
        arr = [values]
        acc = model.predict(arr)

        # Pass ThingSpeak data to the template
        with lock:
            return render_template('prediction.html', prediction=str(acc), moisture=moisture_data, temperature=temperature_data, humidity=humidity_data)
    else:
        return "Sorry...  Error in entered values in the form. Please check the values and fill it again"


if __name__ == '__main__':
    app.run(debug=True)
