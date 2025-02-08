# Plan
* Create a Promodoro timer
* Override Pomodoro with an Game

The Idea of PomoRPG is to create an idle game in which the the user manages various timers. By specifying names and selecting skills, users can choose their way of playing. This game will provide you with vaious text- & configfiles.  
The most important rule is:
  
**configure and use** instead of **play**   

Commands:
```
- save                      saving both timers and characterstats
- timer [nameOfTimer]       set header timer to [nameOfTimer]
- add [newTimerName]        add a new timer to the list
- pause                     pauses everything
- pause [name]              pauses a specific timer
- stop                      same as pause
- stop [name]               same as pause [name]
- start [name]              start a specific timer
- resume [name]             same as start [name]
- exit                      exit the app (CTRL + C)
```
UI-Commands:
```
- bigclock
- charsettings
- stopwatches
- fight
- eventlist
- circle
```


# Lets RPG a bit 
* create multiple files and folders.(builded)
* user ist able to save his pomotime and use it as exp.
* a character is created ( in a new folder/file structure )
* there must be a reason to stop the clock and take a break
Example:
    * level up you char when working (timer goes up)
    * char needs to sleep/eat/fight/search/brew ...
        * this will only happen when the user is taking a break (timer goes down)

Idea:
* [ ] create a todo list
* [x] save timers in saveFile:
* [x] create new timers
* [x] set name for timers
* [ ] render last line to see input
* [ ] tap to complete
* [x] render timer in listformat
* [x] start pomoRPG with specific timer
* [ ] save addedTime[all timer added into 1 digid] in server;

