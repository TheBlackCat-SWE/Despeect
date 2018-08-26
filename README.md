# DeSpeect

A GUI for Speect

## Description

DeSpeect is a graphical user interface for Speect.
Speect is a multilingual TTS(text-to-speech) system, initially developed by 
The Human Language Technologies group at the Meraka Institute, CSIR, South Africa (more information [here](http://speect.sourceforge.net/index.html)).
DeSpeect requires the Speect version maintained and developed by [MIVOQ](https://www.mivoq.it/)(Github repository [here](https://github.com/mivoq/speect)).

## Requirements 

The following dependencies must be fulfilled in order to successfully:

- Qt 5.10.1  
- g++ 5.4.0   
- git 2.7.4
- curl 7.4.0   
- swig 3.0.8  
- libXML2 2.9.3  
- Python 2.7.11
- autoreconf  5.9
- libGL 10.2.0
- CMake 2.8 

DeSpeect has been developed and tested on Ubuntu(16.04 LTS).

## Building and Installing


In order to successfully build DeSpeect, you must:

- Clone the Despeect repository
- Install Speect
- Configure the application

### Clone the Despeect repository

1. Open a terminal
2. Go to the \<install-directory\> (of your choice)
3. issue the command
``` 
git clone https://github.com/TheBlackCat-SWE/Despeect.git 
```

### Install Speect

1. Open a terminal
2. Go to the \<install-directory\>/Despeect/SpeectLib/
3. issue the command
```
install.sh
```
The compilation time may vary.

### Configure the application

At the time of the writing, is necessary to configure Despeect with QtCreator.
The following instruction are required in order to configure DeSpeect:

1. Open QtCreator
2. Select File -> Open File or Project
3. Go to the directory \<install-directory\>/Despeect and open the file CMakeLists.txt
4. Click on button 'Open'
5. Click on button 'Configure Project'


## Quickstart

A (brief) example of usage

<img src="https://github.com/TheBlackCat-SWE/Despeect/blob/master/readme_images/despeect.png" alt="Despeect main window">

In order to successfully generate the HRG graph you must do:

1. Load a configuration file
2. Insert the text 
3. Select the Utterance Type and Utterance Processor
4. Generate the HRG

### Load a configuration file

1. Select File -> Open File or Project (1)
2. Choose the configuration file voice (you could find some inside the directory \<install-directory\>/Despeect/voices)
3. Click on Open

### Insert text

Insert the text inside the text box (2)

### Select the Utterance Type and Processor

Once loaded the configuration file, the 'Flow Control' widget(3) will contain
a checkbox list of all Utterance Type and Processor available.
Select the required utterance types and processor.

### Generate the HRG

Once you have selected the required utterance types and processor, 
in order to generate the HRG you can:

- Run All(run all Utterance processor inside the selected Utterance Type)
- Run Step (run the Utterance processor individually)

clicking the button 'Run All' and 'Run Step' respectively.

The HRG will be printed in (5)


