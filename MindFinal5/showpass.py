import pickle as pk
with open('files/password.pkl','rb') as file:
    password = pk.load(file)
    print(password)
    print(type(password))