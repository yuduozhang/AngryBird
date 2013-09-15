AngryBird
=========

An Angry Bird game developed in C++ with X library http://www.x.org/wiki/. (2000+ lines of code)

Describe how to move between levels easily.
Click on the "Next Level" button, then go to Level 2
Click on the "Restart" button, then restart the game 

Describe your enhancements.

1. In level two, I add a moving target, which makes level 2 much more 
challenging than level 1. Players can also specify the moving speed 
by ./madBird -vp number. By increasing the speed, players can experience
more challenge.

2. Add more animation, when hit the target, the target will
disappear gradually, and the points accumulated will raise
from the target gradually.

3. Add colour: use XSetForeground() function to add colour,
to make the design more beautiful.

4. Instead of reducing marks after one shot, I drew how many birds left on
the screen so that players have to use the minimum number of
birds to pass one level. This makes the game more
challenging. Players also can cheat by changing the number
of birds available in each level. For testing purpose, you
might want to increase this number. Please see ./madBird -help for
more information.

5. Display appropriate message if fail the game, or win the game.
