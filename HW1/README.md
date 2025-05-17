# Voice Spoken Digits

The aim of this homework is to implement a classifier able to predict which digit is pronounced in a short audio excerpt. 
To achieve this, we have collected our data from the folders of the original dataset in the vector train root:

! git clone https://github.com/Jakobovski/free-spoken-digit-dataset.git\\
train_root = (’free-spoken-digit-dataset/recordings’)\\

The dataset consists of 3000 recordings of spoken digits in wav files at 8kHz.
The recordings are trimmed so that they have near minimal silence at the beginnings and ends.

Furthermore files are named in the following format:

{digitLabel}_{speakerName}_{index}.wav     Example: 7_jackson_32.wav

