from flask import Flask, render_template, request, jsonify
from sqlalchemy import create_engine
from sqlalchemy.orm import sessionmaker
from database_setup import data, Base

engine = create_engine('sqlite:///R:\\Windows\\Bureau\\Dashboard\\database.db?check_same_thread=False')
Base.metadata.bind = engine
DBSession = sessionmaker(bind=engine)
session = DBSession()

app = Flask(__name__)


@app.route('/', methods=['POST', 'GET'])
def root():
    req = session.query(data).filter().order_by(data.id.desc()).limit(10).all()
    req=req[::-1]
    labels = []
    humi = []
    hygros = []
    temps = []
    for i in req:
        labels.append(i.date[-5:])
        humi.append(i.humidite)
        hygros.append(i.hygrometrie)
        temps.append(i.temperature)
    req = obj = session.query(data).order_by(data.id.desc()).first()
    status = [req.pompe,req.volet,req.brumisateur,req.ventilateur]
    return render_template('index.html',labels = labels, humidite = humi, hygros = hygros,temps = temps,status = status)

    
if __name__ == '__main__':
    app.run(host='0.0.0.0')