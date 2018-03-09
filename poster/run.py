
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
    picName = str(t.today())
    return picName;

def genCaption():
    newCaption = "<i>Squidolus</i> [" + genName() + "]"
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
    imgSeed = str(random())
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
    while running:
        schedule.run_pending()
        time.sleep(5)
    return;

#main function
postAll()
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













