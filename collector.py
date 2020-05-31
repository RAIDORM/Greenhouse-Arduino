import serial
import datetime

from sqlalchemy import create_engine
from sqlalchemy.orm import sessionmaker
# import de sqlalchemy et de ma table users
from database_setup import data, Base

# connexion a la bdd
engine = create_engine('sqlite:///R:\\Windows\\Documents\\Greenhouse-Arduino\\database.db')
Base.metadata.bind = engine
# creation d'une session de connexion a la bdd
DBSession = sessionmaker(bind=engine)
session = DBSession()


# configuration du port série
print('Entrez le port COM de votre arduino')
com_number = input()
ser = serial.Serial("COM" + com_number, 9600)


# fonction d'écoute sur le port série

def serial_listener():
    if ser.isOpen():
        print(f'Port {ser.name} open')
        while True:
            line = ser.readline()
            result = line.decode().rstrip()
            # rstrip pour enlever les EOL et 8 pour la lenght de l'UID
            if result[0:3] == "BOT":
                bdd(result)
            else:
                print(result)

# fonction de select du nombre de trajets de l'utilisateur

def bdd(result):
    # récupération de la date
    date = datetime.datetime.now().strftime('%d/%m/%Y/%H:%M')
    result = result[3:]
    result = result.split('/')
    insert = data(
        hygrometrie = result[0],
        temperature = result[1],
        humidite = result[2],
        pompe = result[3],
        volet = result[4],
        brumisateur = result[5],
        ventilateur = result[6],
        date = date
    )
    session.add(insert)
    session.commit()

serial_listener()