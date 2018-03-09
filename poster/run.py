
import pytumblr
import auth
import datetime
import time
import schedule
from subprocess import call
from random import *
from threading import Thread

#note: requires pytumblr and schedule from pip

filepath = "images/"
defaultTags = ["space","procedural generation", "voyager", "planet", "astronomy"]
postingEnabled = True;
running = True;

#date of initial departure
epoch = datetime.datetime(2017, 7, 3, 20, 11, 0, 0)

client =  pytumblr.TumblrRestClient(
    auth.auth1,
    auth.auth2,
    auth.auth3,
    auth.auth4,
)

def genImage(name, seed):
    print("generating image " + name + ".png with seed " + seed)
    call(["./Voyager", name ,seed]);
    return;

def genName():
    t = datetime.date
    h = datetime.datetime.now()
    picName = str(t.today()) + '_' + str(h.hour)
    return picName;

def getTime():
    first = epoch
    now = datetime.datetime.now()
    delta = now - first 
    newTime = str("Day:" + str(delta.days) + " Hour:" + str(delta.seconds//3600))
    return newTime;

def genCaption():
    newCaption = "<i>Squidolus</i> [" + getTime() + "]"
    return newCaption;

def post(image, cap, tag):
    client.create_photo(
        auth.blogName, 
        state="published", 
        tags=tag,
        caption=cap,
        data=filepath+image+".png"
    )
    return;

def postAll():
    imgSeed = str(random()*2000.0-1000.0)
    imgName = genName()
    imgCaption = genCaption()
    genImage(filepath + imgName, imgSeed)
    print('rendering program finished.')
    if postingEnabled:
        post(imgName, imgCaption, defaultTags)
        print("posted with caption: " + imgCaption)
    return;

#threaded runner that posts daily
def runPoster():
    #set schedule
    schedule.every().day.at("20:30").do(postAll)
    schedule.every().day.at("8:30").do(postAll)
    while running:
        schedule.run_pending()
        time.sleep(5)
    return;

#main function
thread = Thread(target = runPoster)
thread.start()
while running:
    textInp = raw_input("command:")
    
    #check command
    if textInp == "endprog":
        #end
        print("ending program...")
        running = False
        thread.join()
    
    
print("good night sweet prince")













