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
        return render_template("results.html",data = data2)

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
