from flask import Flask, render_template, redirect, url_for
from flask import request
import requests
from datetime import date


app = Flask(__name__)

@app.route("/",methods = ['POST', 'GET'])
def result():
   data = []
   moisture = str(request.values.get("Soil_Moisture"))
   data.append(moisture)
   UV = str(request.values.get("UV_Sensor"))
   data.append(UV)
   string = f'Moisture: {moisture}, UV intensity:{UV}'
   print(string)
   f = open("data.txt", "a")
   f.write(data[0] + "," + data[1] + "\n")
   f.close()
   headers = {
       'x-access-token': 'YOUR UV API ACCESS TOKEN HERE',
       }
   response = requests.get("https://api.openweathermap.org/data/2.5/weather?lat=33.7&lon=-117.8&appid=OPEN WEATHER ACCESS KEY HERE")
   today = str(date.today())
   string = 'https://api.openuv.io/api/v1/uv?lat=-33.7&lng=117.8&' + 'dt=' + today + 'T10:50:52.283Z'
   response2 = requests.get(string, headers=headers)
   rain = []
   uv = []
   data2 = [[0,0]]
   data3 = []
   if response.status_code == 200:
       pred = response.json()
       weather = pred['weather']
       rain.append(weather[0]['main'])
       #rain.append("Clear")
       #print(f'Rain: {rain[0]}')
   if response2.status_code == 200:
       pred = response2.json()
       print(pred)
       #uv.append(pred['result']['uv_max'])
       uv.append(4.0)
       #print(uv)
   #if request.method == 'GET':
   data = [[0,0]]
   with open('data.txt') as f:
       for i in f:
           if i == "\n":
               continue
           i_strip = i.rstrip("\n")
           d = list(i_strip.split(','))
           if d[1] == "" or d[1] == 'None':
               d = data[-1]
           elif d[0] == "" or d[0] == "None":
               d = data[-1]
           d[0] = float(d[0])
           d[1] = float(d[1])
           data.append(d)
       data2.append(data[-1])
   data3.append(data2[-1])

   #print(f'Data to display: Moisture: {data3}')
   #print(f'Data2 {data2}')
   return render_template("results.html",data3 = data3, rain = rain, uv = uv)

@app.route('/dashboard',methods = ['POST', 'GET'])
def dashboard():
    data = []
    data_uv = []
    with open('data.txt') as f:
        for i in f:
            if i == "\n":
                continue
            i_strip = i.rstrip("\n")
            d = list(i_strip.split(','))
            if d[1] == "" or d[1] == 'None' or d[0] == "" or d[0] == "None":
                continue
            d[1] = float(d[1])
            d[0] = float(d[0])
            data.append(d)
    labels = ["16:00", "16:01", "16:02", "16:03", "16:04", "16:05", "16:06", "16:07", "16:08", "16:09", "16:10"]
    values = [row[0] for row in data]
    values = values[-11:]
    labels_uv = ["16:00", "16:01", "16:02", "16:03", "16:04", "16:05", "16:06", "16:07", "16:08", "16:09", "16:10"]
    values_uv = [row[1] for row in data]
    values_uv = values_uv[-11:]
    #print(f'Labels: {labels}')
    #print(f'Values: {values}')
    #print(f'Labels_UV: {labels_uv}')
    #print(f'Values_UV: {values_uv}')
    return render_template("dashboard.html", labels = labels, values = values, labels_uv = labels_uv, values_uv = values_uv)

if __name__ == '__main__':
    app.run(debug=True)
