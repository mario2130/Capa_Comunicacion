from flask import Flask, Response,request

app = Flask(__name__)

@app.route("/")
def hello_world():
    return "<p>Hello, World!</p>"

@app.post('/wemostime')
def add_time():
    content = request.json
    print(content['time'])
    return Response("", status=200, mimetype='application/json')

if __name__ == '__main__':
    app.run(host='192.168.100.30', port=5000, debug=True, threaded=False)