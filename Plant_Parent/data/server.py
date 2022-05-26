from flask import Flask, render_template, redirect, url_for
from flask import request

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
   if request.method == 'GET':
       print("Inside POST")
       f = open('data.txt', 'r')
       data = f.read().split(',')
       return render_template("results.html",data = data)



if __name__ == '__main__':
    app.run(debug=True)
