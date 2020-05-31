import sys
# for creating the mapper code
from sqlalchemy import Column, Integer, Float, String

# for configuration and class code
from sqlalchemy.ext.declarative import declarative_base

# for creating foreign key relationship between the tables
from sqlalchemy.orm import relationship

# for configuration
from sqlalchemy import create_engine

# create declarative_base instance
Base = declarative_base()

# we'll add classes here

# creates a create_engine instance at the bottom of the file
engine = create_engine('sqlite:///R:\\Windows\\Documents\\Greenhouse-Arduino\\database.db?check_same_thread=False')

Base.metadata.create_all(engine)

# we create the class Book and extend it from the Base Class.


class data(Base):
    __tablename__ = 'data'

    id = Column(Integer, primary_key=True, autoincrement=True)
    temperature = Column(Float, nullable=False)
    humidite = Column(Integer, nullable=False)
    hygrometrie = Column(Float, nullable=False)
    pompe = Column(Integer, nullable=False)
    volet = Column(Integer, nullable=False)
    ventilateur = Column(Integer, nullable=False)
    brumisateur = Column(Integer, nullable=False)
    date = Column(String(250), nullable=False)
