from flask import Flask, render_template, redirect, url_for
from flask import request
import requests
from datetime import date


app = Flask(__name__)

@app.route("/",methods = ['POST', 'GET'])
def result():
   data = []
   moisture = str(request.values.get("Soil_Moisture"))
   print(f'Moisture: {moisture}')
   data.append(moisture)
   UV = str(request.values.get("UV_Sensor"))
   data.append(UV)
   string = f'Moisture: {moisture}, UV intensity:{UV}'
   print(string)
   f = open("data.txt", "a")
   f.write(data[0] + "," + data[1] + "\n")
   f.close()
   headers = {
       'x-access-token': '12b0901d3e6ccd9b6e1b8bd473d63515',
       }
   response = requests.get("https://api.openweathermap.org/data/2.5/weather?lat=33.7&lon=-117.8&appid=9e45703d4ebce98a50d6970fda54c452")
   today = str(date.today())
   string = 'https://api.openuv.io/api/v1/uv?lat=-33.7&lng=117.8&' + 'dt=' + today + 'T10:50:52.283Z'
   response2 = requests.get(string, headers=headers)
   rain = []
   uv = []
   if response.status_code == 200:
       pred = response.json()
       weather = pred['weather']
       #rain.append(weather[0]['main'])
       rain.append("Clear")
       print(f'Rain: {rain[0]}')
   if response2.status_code == 200:
       pred = response2.json()
       print(pred)
       #uv.append(pred['result']['uv_max'])
       uv.append(8.0)
       print(uv)
   if request.method == 'GET':
        data = [[0,0]]
        with open('data.txt') as f:
            for i in f:
                if i == "\n":
                    continue
                i_strip = i.rstrip("\n")
                d = list(i_strip.split(','))
                if d[1] == "" or d[1] == 'None' or d[0] == "" or d[0] == "None":
                    continue
                d[0] = float(d[0])
                d[1] = float(d[1])
                data.append(d)
        data2 = []
        data2.append(data[-1])
        return render_template("results.html",data = data2, rain = rain, uv = uv)

@app.route('/dashboard',methods = ['POST', 'GET'])
def dashboard():
    data = []
    data_uv = []
    with open('data2.txt') as f:
        for i in f:
            if i == "\n":
                continue
            i_strip = i.rstrip("\n")
            d = list(i_strip.split(','))
            if d[1] == "" or d[1] == 'None' or d[0] == "" or d[0] == "None":
                continue
            d[1] = float(d[1])
            data.append(d)
    with open('data3.txt') as f:
        for i in f:
            if i == "\n":
                continue
            i_strip = i.rstrip("\n")
            d = list(i_strip.split(','))
            if d[1] == "" or d[1] == 'None' or d[0] == "" or d[0] == "None":
                continue
            d[1] = float(d[1])
            data_uv.append(d)
    labels = [row[0] for row in data]
    values = [row[1] for row in data]
    labels_uv = [row[0] for row in data_uv]
    values_uv = [row[1] for row in data_uv]
    print(f'Labels: {labels}')
    print(f'Values: {values}')
    print(f'Labels_UV: {labels_uv}')
    print(f'Values_UV: {values_uv}')
    return render_template("dashboard.html", labels = labels, values = values, labels_uv = labels_uv, values_uv = values_uv)

if __name__ == '__main__':
    app.run(debug=True)
