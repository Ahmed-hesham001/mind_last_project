############### THIS FILE IS ONLY USED FOR PRE-ELIMINARY PASSWORD SETTING FOR NOW ###############

import pickle as pk

password = str(input("Enter password : "))
password = password[:4]

with open('files/password.pkl','wb') as file:
    pk.dump(password,file)

print(password)