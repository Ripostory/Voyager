import twitter
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

def genTwitterCaption():
    newCaption = "- Squidolus [" + getTime() + "]"
    return newCaption;

def postTumblr(image, cap, tag):
    client =  pytumblr.TumblrRestClient(
    auth.auth1,
    auth.auth2,
    auth.auth3,
    auth.auth4
    )
    client.create_photo(
        auth.blogName, 
        state="published", 
        tags=tag,
        caption=cap,
        data=filepath+image+".png"
    )
    print("posted with caption: " + cap)
    return;

def postTwitter(image, cap, tag):
    api = twitter.Api(
	    consumer_key=auth.twitterKeyApi,
	    consumer_secret=auth.twitterKeySecret,
	    access_token_key=auth.twitterKeyToken,
	    access_token_secret=auth.twitterKeyTokenSecret
	)
    status = api.PostUpdate(status=cap, media=filepath+image+".png")
    print("{0} just posted: {1}".format(status.user.name, status.text))
    return;

def generateTumblr():
    try:
        imgSeed = str(random()*2000.0-1000.0)
        imgName = genName()
        imgCaption = genCaption()
        genImage(filepath + imgName, imgSeed)
        print('rendering program finished.')
        if postingEnabled:
            postTumblr(imgName, imgCaption, defaultTags)
    except:
        print('An Error has occured while posting to Tumblr')
    return;

def generateTwitter():
    try:
        imgSeed = str(random()*2000.0-1000.0)
        imgName = genName()
        imgCaption = genTwitterCaption()
        genImage(filepath + imgName, imgSeed)
        print('rendering program finished.')
        if postingEnabled:
            postTwitter(imgName, imgCaption, defaultTags)
    except:
        print('An Error has occured while posting to Twitter')
    return;

#threaded runner that posts daily
def runPoster():
    #set schedule
    schedule.every().day.at("20:30").do(generateTumblr)
    schedule.every().day.at("08:30").do(generateTumblr)
    schedule.every().day.at("00:30").do(generateTwitter)
    schedule.every().day.at("16:30").do(generateTwitter)
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

    if textInp == "forceTumblrPost":
        #immediately trigger a post
        print("Forcing tumblr post...")
        generateTumblr()

    if textInp == "forceTwitterPost":
        #immediately trigger a post
        print("Forcing twitter post...")
        generateTwitter()
    
print("good night sweet prince")













