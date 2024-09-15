import cv2 as cv
import os
import numpy as np
import pickle as pk # used to save and load the password from a pkl file

# we will use this array to save the names of the ppl (image folders names)
people = []

# we will use this array to store the features of the people where each face will correspond to a label (name)
features = []

# we will use this array to save the labels (names) corresponding to the features
labels = []

for i in os.listdir(r'faces'):
    people.append(i)

# Sanity Check
print(f"people present in database : {people}")

# Using the Haar_cascade XML file to initialize a 
# classifier (to generate features and labels array to train our face model/recognizer to)
haar_cascade = cv.CascadeClassifier("files/haar_face.xml") 

def train():
    # Every Folder in Faces
    for person in people:
        
        # The Path to ever Folder
        path = os.path.join(r"faces",person)
        label = people.index(person)

        # Every Image in Folder
        for image in os.listdir(path):

            # The Path to every Image
            img_path = os.path.join(path,image)

            # Saved the path as a CV photo
            img_array = cv.imread(img_path)

            # Converted each image to a grey scale image for easier face detection and less strain
            grey = cv.cvtColor(img_array,cv.COLOR_BGR2GRAY)

            # using the haar_cascade to study the photo , parameter (img = gray , scaling factor = 1.1 , Min Neighbor = 5)
            # Note , Min Neighbors : This parameter specifies how many neighbors each candidate rectangle should have to retain it as a face.
            # a higher Min Neighbors means less detection but higher qualioty (fewer mistakes) , so we change it according to our needs
            # the lower , the more sensitive it is to NOISE (NOISE BAD)
            faces_rect = haar_cascade.detectMultiScale(grey,1.1,5)
            
            # Loop over the dimensions in faces_rect
            for (x,y,w,h) in faces_rect:
                # We are defining the area of interset as the area in the grey image from points (x,y) to (x+w,y+h)
                faces_roi = grey[y:y+h,x:x+w]
                # Adding features and labels
                features.append(faces_roi)
                labels.append(label)
train()

print("Features and Labels cascading successful")

# converting features, people and labels to numpy array (numpy fo2 ya norm)
people = np.array(people)
features = np.array(features,dtype='object')
labels = np.array(labels)

# creating a recognizer and training it to match features to labels
face_model = cv.face.LBPHFaceRecognizer.create()
face_model.train(features,labels)

print("Face Model Training Successful")

# Save the model to a YML file for importing in the recognition program
face_model.save('files/face_model.yml')

# saving the array to npy files
np.save("files/people.npy",people)
np.save('files/features.npy',features)
np.save('files/labels.npy',labels)

print("Model Files saved to Files Folder successfully")

print("Program Successful")