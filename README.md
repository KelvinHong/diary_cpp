# Diary Notes Application

## Introduction

This diary enable user to record events on multiple titles. 

## Usage

Explain by using examples should be a good idea.

The user first create titles: **Vacation, Work, Family**. Then, record event on **Vacation**. 
Say the event has a summary title **"Camping"**, then he recorded the detail of the event as follow: 

```
    Today is a sunny day,
I brought my wife and children for a relaxing camping at XXX site.
It is a memorial day before I return to the boring work tomorrow.
```

The recorded event can be found in `data/` folder. The file is saved as `{Title}_{Unix_second}.txt`. However, when view contents of the diary, the `Unix_second` will be changed to human-readable form. 

## Features

The user can create new event, view existing events and edit existing events. 
The csv file `events.csv` acts as a master file to record main details of the events. 
The `LastEdtit` column record the Unix-second time for where the user last created/edited the textfile.
The `Time` column record whenever the textfile first recorded, hence will not be changed after edit.

Currently, the application doesn't support deletion, filtering and sorting functions. They might be included in later updates. 

## The Author

Hi, My name is Hong Khay Boon, currently a fresh Bachelor graduate as Math major in Malaysia. This is my first git repo (I hope this is the correct wording) of my projects, it is primarily served as a tool for me to familiar with GitHub functions. 
I will keep updating on this software, while working on some future projects about AI and machine learning. Thank you!
