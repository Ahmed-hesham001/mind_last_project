import os
import numpy as np
import cv2 as cv
import shutil
import pickle as pk
from tkinter import *
from tkinter import ttk
from PIL import Image, ImageTk
import subprocess as sp
import threading as th
import time
import serial
from tkinter import messagebox
# Loaded our three main arrays
people = np.load('files/people.npy', allow_pickle=True)
labels = np.load('files/labels.npy', allow_pickle=True)
features = np.load('files/features.npy', allow_pickle=True)

# Loaded our password variable
with open('files/password.pkl', 'rb') as file:
    password = pk.load(file)

# FACE MODEL
face_model = cv.face.LBPHFaceRecognizer.create()
face_model.read('files/face_model.yml')

# CASCADE
haar_cascade = cv.CascadeClassifier('files/haar_face.xml')

def train():
    global people
    global features
    global labels
    global face_model
    sp.run(['python', 'face_train.py'])
    print("Model has been retrained Successfully")
    people = np.load('files/people.npy', allow_pickle=True)
    labels = np.load('files/labels.npy', allow_pickle=True)
    features = np.load('files/features.npy', allow_pickle=True)
    face_model.read('files/face_model.yml')

def threadTrain():
    th.Thread(target=train).start()
recognized = False
# Main function that reads the "FRAME"
def recognizeFrame(frame):
    global recognized
    grey = cv.cvtColor(frame, cv.COLOR_BGR2GRAY)
    faces_rect = haar_cascade.detectMultiScale(grey, 1.1, 8)
    recognized = False
    
    for (x, y, w, h) in faces_rect:
        faces_roi = grey[y:y + h, x:x + w]
        label, confidence = face_model.predict(faces_roi)

        if confidence > 50 and label < len(people):
            recognized = True
            cv.putText(frame, str(people[label]), (x, y + h + 50), cv.FONT_HERSHEY_COMPLEX, 1.2, (0, 255, 0), 2)
            cv.rectangle(frame, (x, y), (x + w, y + h), (0, 255, 0), 2)
        else:
            cv.putText(frame, "Unknown Person", (x, y + h + 50), cv.FONT_HERSHEY_COMPLEX, 1.2, (0, 0, 255), 2)
            cv.rectangle(frame, (x, y), (x + w, y + h), (0, 0, 255), 2)

    # Update status label
    if recognized or indef:
        status_label.config(text="OPEN", fg="green")
    else:
        status_label.config(text="CLOSED", fg="red")

    return frame

def show_frame():
    __, frame = capture.read()
    frame = recognizeFrame(frame)
    frame = cv.cvtColor(frame, cv.COLOR_BGR2RGB)
    img = Image.fromarray(frame)
    imgtk = ImageTk.PhotoImage(image=img)
    label.imgtk = imgtk
    label.config(image=imgtk)
    label.after(10, show_frame)

def save_person():
    global people
    person = person_entry.get()
    if not person:
        print("No person name entered!")
        return
    
    person_dir = os.path.join("Faces", person)
    if os.path.exists(person_dir):
        # Folder already exists, so skip appending to people list
        print(f"Folder for {person} already exists. Skipping addition to list.")
    else:
        people = np.append(people, person)
        people_listbox.insert(END, person)
    
    os.makedirs(person_dir, exist_ok=True)
    
    # Count existing files to determine the next number
    existing_files = [f for f in os.listdir(person_dir) if f.endswith(".jpg")]
    next_number = len(existing_files) + 1
    
    # Capture the frame and save it
    for i in range(15):
        __, frame = capture.read()
        file_path = os.path.join(person_dir, f"{next_number+i}.jpg")
        cv.imwrite(file_path, frame)
        print(f"Saved {file_path}")
        time.sleep(0.75)
    threadTrain()

    
    # Update the people list and combobox
    update_combobox()
    person_entry.delete(0, END)
def saveThread():
    th.Thread(target=save_person).start()
def delete_person():
    person = delete_combo.get()
    if not person:
        print("No person selected!")
        return
    person_dir = os.path.join("Faces", person)
    if os.path.exists(person_dir):
        shutil.rmtree(person_dir)
        print(f"Deleted {person_dir}")

    global people, labels, features
    person_idx = np.where(people == person)[0][0]
    people = np.delete(people, person_idx)
    labels = np.delete(labels, person_idx)
    features = np.delete(features, person_idx, axis=0)
    threadTrain()
    people_listbox.delete(people_listbox.get(0, END).index(person))
    update_combobox()

    face_model.train(features, labels)
    face_model.save('files/face_model.yml')

def update_combobox():
    delete_combo['values'] = list(people)

def update_password():
    global password
    new_password = password_entry.get()
    if len(new_password) >= 4:
        new_password = new_password[:4]  # Take the first 4 chars
        with open('files/password.pkl', 'wb') as file:
            pk.dump(new_password, file)
        password = new_password
        print("Password updated.")
    else:
        print("Password must be at least 4 characters long.")
    password_entry.delete(0, END)

indef = False
def set_open_indefinitely():
    global indef
    indef = True
    time.sleep(5)
    indef = False
def threadOpen():
    th.Thread(target=set_open_indefinitely).start()
def pywhile():
    arduinoData = serial.Serial("COM8",4800)
    time.sleep(0.5)
    while True:
        global recognized,indef
        flag = recognized or indef
        if (arduinoData.in_waiting>0):
            print("recieved from arduino")
            dataPacket = arduinoData.readline()
            dataPacket = str(dataPacket,'utf-8')
            dataPacket = dataPacket.strip('\r\n\0')
            print(dataPacket)
            time.sleep(1)
            if dataPacket == "face rec flag:":
                arduinoData.write(b'1' if flag else b'0')
                # if flag == True:
                    # messagebox.showinfo('Success','Face Recognized')
                print(flag)
            elif dataPacket == "Entered Password: ":
                if (arduinoData.in_waiting>0):
                    print(00)
                    inpPass = arduinoData.readline()
                    print(11)
                    inpPass = str(inpPass,'utf-8')
                    print(22)
                    inpPass = inpPass.strip('\r\n\0')
                    print(33)
                    print(f"{inpPass} is the input password")
                    password =""
                    with open('files/password.pkl', 'rb') as file:
                        password = pk.load(file)
                    print(f"{password} is the database password")
                    inpPass = inpPass.strip()
                    password = password.strip()
                    print(str(inpPass)==password)
                    if inpPass == password:
                        arduinoData.write(b'1')
                        # messagebox.showinfo("Success","Password Accepted")
                    else:
                        arduinoData.write(b'0')
                        
    
                    
            
        
# Set up Tkinter window
root = Tk()
root.title("Face Recognition")
root.geometry("1200x700")
root.configure(bg="#153052")

# Title Label
title_label = Label(root, text="Face Recognition Model", font=("Helvetica", 30), fg="white", bg="#153052")
title_label.grid(row=0, column=0, columnspan=3, pady=(10, 20))

# Left Frame (Video Display)
frame_container = Frame(root, padx=10, pady=10, bg="#153052", borderwidth=2, relief="solid", highlightbackground="cyan", highlightcolor="cyan", highlightthickness=2)
frame_container.grid(row=1, column=0)

label = Label(frame_container, bg="#153052")
label.pack()

# Middle Frame (Status Label and List of Users)
middle_frame = Frame(root, padx=20, pady=10, bg="#153052")
middle_frame.grid(row=1, column=1)

status_label = Label(middle_frame, text="CLOSED", font=("Helvetica", 24), fg="red", bg="#153052")
status_label.pack()

people_listbox = Listbox(middle_frame, height=20, width=30, bg="#1e3a8a", fg="white")
for person in people:
    people_listbox.insert(END, person)
people_listbox.pack()

# Right Frame (Entry and Buttons)
right_frame = Frame(root, padx=20, pady=10, bg="#153052")
right_frame.grid(row=1, column=2)

person_label = Label(right_frame, text="Enter Person Name:", bg="#153052", fg="white")
person_label.pack(pady=10)

person_entry = Entry(right_frame, bg="#1e3a8a", fg="white", insertbackground='white')
person_entry.pack(pady=10)

add_button = Button(right_frame, text="Add Person to Database", command=saveThread, bg="#10b981", fg="white", relief=FLAT, padx=20, pady=10)
add_button.pack(pady=10, padx=10)

delete_label = Label(right_frame, text="Select Person to Delete:", bg="#153052", fg="white")
delete_label.pack(pady=10)

delete_combo = ttk.Combobox(right_frame, values=list(people), background="#1e3a8a", foreground="white")
update_combobox()
delete_combo.pack(pady=10)

delete_button = Button(right_frame, text="Delete Person", command=delete_person, bg="#ef4444", fg="white", relief=FLAT, padx=20, pady=10)
delete_button.pack(pady=10, padx=10)

# Password Update
password_label = Label(right_frame, text="New Password:", bg="#153052", fg="white")
password_label.pack(pady=10)

password_entry = Entry(right_frame, bg="#1e3a8a", fg="white", insertbackground='white')
password_entry.pack(pady=10)

update_password_button = Button(right_frame, text="Update Password", command=update_password, bg="#3b82f6", fg="white", relief=FLAT, padx=20, pady=10)
update_password_button.pack(pady=10, padx=10)

# Set "OPEN"
set_open_button = Button(right_frame, text="Set OPEN", command=threadOpen, bg="#10b981", fg="white", relief=FLAT, padx=20, pady=10)
set_open_button.pack(pady=10, padx=10)

# Reset the status label to normal operation mode (by resetting indef to False)


capture = cv.VideoCapture(0)
th.Thread(target=pywhile).start()
show_frame()

root.mainloop()
capture.release()
cv.destroyAllWindows()