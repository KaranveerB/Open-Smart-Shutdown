# Open Smart Shutdown (WIP)
A QT based cross-platform open source tool to automate conditional shutdowns, similar to [AMP WinOFF](http://www.ampsoft.net/utilities/WinOFF.php).

This can help in various cases such as 
* Shutdown after downloading a game (based on low network and disk usage)
* Shutdown after a render is finished (based on low CPU/GPU usage)
* Shutdown after an upload finishes (based on low network usage)

This program also aims to allow you to do anything you can do with a shell in it,
whether that be monitoring an event or performing a non-shutdown command.

This can be used in cases like
* Killing an RDP service after extended inactivity
* Send a notification to a phone when a process has ended
* Anything else you can do with a shell really. If you can monitor what you want in a shell 
and you can perform the finish task in a shell, you should be able to do it in Open Smart Shutdown.

## Goals of the Project
### Compatibility
The program's defaults should work on a fresh Windows 10 and Ubuntu install (macOS is not included as I don't have a system to test on).

The program should also be easily flexible to allow easy configuration to work on whatever Windows, Linux, or macOS system you can conjure.

### Flexibility
If you can do it in a shell, you can do it better in Open Smart Shutdown.
You should also be able to montior as many events with each monitor being
highly configurable (e.g., buffers to prevent false negatives, polling rates, customizable ranges of positive values)

### Ease of Use
The program should let you use its flexibility to the fullest without fighting you.
This is a tool that should work for you so you shouldn't have to work for the tool.

You should also be able to load profiles you previously made to easily repeat previous Open Smart Shutdown tasks.
