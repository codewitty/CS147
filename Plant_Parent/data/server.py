from flask import Flask, render_template, redirect, url_for
from flask import request
import requests
from datetime import date


app = Flask(__name__)

@app.route("/", methods=['GET', 'POST'])
def hello():
    data = []
    moisture = str(request.values.get("Soil_Moisture"))
    data.append(moisture)
    UV = str(request.values.get("UV_Sensor"))
    data.append(UV)
    string = f'Moisture: {moisture}, UV intensity:{UV}'
    print(string)
    f = open("data.txt", "a")
    f.write(data[0] + "," + data[1])
    f.close()
    return render_template("results.html",data = data)

@app.route('/result',methods = ['POST', 'GET'])
def result():
   headers = {
       'x-access-token': '12b0901d3e6ccd9b6e1b8bd473d63515',
       }
   response = requests.get("https://api.openweathermap.org/data/2.5/weather?lat=33.7&lon=-117.8&appid=9e45703d4ebce98a50d6970fda54c452")
   today = str(date.today())
   string = 'https://api.openuv.io/api/v1/uv?lat=-33.7&lng=117.8&' + 'dt=' + today + 'T10:50:52.283Z'
   response2 = requests.get(string, headers=headers)
   rain = "None"
   uv = "None"
   if response.status_code == 200:
       pred = response.json()
       weather = pred['weather']
       rain = weather[0]['main']
   if response2.status_code == 200:
       pred = response2.json()
       uv = pred['result']['uv_max']
       print(uv)
   if request.method == 'GET':
        data = []
        with open('data.txt') as f:
            for i in f:
                if i == "\n":
                    continue
                i_strip = i.rstrip("\n")
                d = list(i_strip.split(','))
                if d[1] == "":
                    continue
                else:
                    d[0] = float(d[0])
                    d[1] = float(d[1])
                    data.append(d)
        print(f'Data:{data}')
        data2 = []
        data2.append(data[-1])
        labels = [row[0] for row in data]
        values = [row[1] for row in data]
        return render_template("results.html",data = data2, rain = rain)

@app.route('/dashboard',methods = ['POST', 'GET'])
def dashboard():
    data = []
    with open('data.txt') as f:
        for i in f:
            if i == "\n":
                continue
            i_strip = i.rstrip("\n")
            d = list(i_strip.split(','))
            if d[1] == "":
                continue
            else:
                d[1] = float(d[1])
                data.append(d)
    print(f'Data:{data}')
    labels = [row[0] for row in data]
    values = [row[1] for row in data]
    print(labels)
    return render_template("dash.html",title = 'Bitcoin price', labels = labels, values = values)

if __name__ == '__main__':
    app.run(debug=True)
